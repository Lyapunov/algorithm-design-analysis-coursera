#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <algorithm>

static const int DEBUG_MODE = 1;

static bool readInput( std::string filename, std::vector<unsigned>& numbers, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   if ( debugmode ) {
      std::cout << "=== READING " << std::endl;
   }

   std::string line;
   while ( std::getline( is, line ) ) {
      std::istringstream ss(line);
      unsigned num;
      ss >> num;
      numbers.push_back( num );
   }
   is.close();

   return true;
}

std::ostream& operator<<( std::ostream& os, const std::vector<unsigned>& rhs ) {
   os << "vector(" << std::endl;
   for ( const auto& elem : rhs ) {
      os << "   " << elem << std::endl;
   }
   os << ")" << std::endl;
   return os;
}

// poor man's merge sort
class MergeSorter {
public:
   MergeSorter( std::vector<unsigned>& nums ) : numStorage_( nums ), mergeStorage_( numStorage_.size(), 0 ) {
      mergeSort( 0, numStorage_.size() );
   }
   
private:
   void mergeSort( unsigned inc_start, unsigned exc_end ) {
      if ( exc_end <= 1 + inc_start ) {
         // at most 1 item, already sorted
         return; 
      }
      unsigned half = ( inc_start + exc_end ) / 2;
      mergeSort( inc_start, half ); 
      mergeSort( half, exc_end ); 
      // merging the results 
      merge( inc_start, half, exc_end );
   }

   void merge( unsigned inc_start, unsigned half, unsigned exc_end ) {
      unsigned p1 = inc_start;
      unsigned p2 = half;
      for ( unsigned i = 0; i < exc_end - inc_start; ++i ) {
         if ( p2 == exc_end || ( p1 < half && numStorage_[p1] < numStorage_[p2] ) ) {
            mergeStorage_[i] = numStorage_[p1]; 
            ++p1;
         } else {
            mergeStorage_[i] = numStorage_[p2]; 
            ++p2;
         }
      }
      // copy to storage
      for ( unsigned i = 0; i < exc_end - inc_start; ++i ) {
         numStorage_[i + inc_start] = mergeStorage_[i];
      }
   }

   std::vector<unsigned>& numStorage_;
   std::vector<unsigned> mergeStorage_;
};

void merge_sort( std::vector<unsigned>& numbers ) {
   MergeSorter x( numbers );
}

int main( int argc, const char* argv[] ) {
   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc != 2 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename>" << std::endl;
      return 0;
   } else{ 
      std::string filename = argv[1];
      std::vector<unsigned> nums;
      if ( !readInput( argv[1], nums, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      merge_sort( nums );
      if ( DEBUG_MODE ) {
         std::cout << "=== PRINTING SORTED DATA" << std::endl;
         std::cout << nums;
      }

   }
}
