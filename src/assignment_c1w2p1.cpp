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
   os << "vector(";
   for ( const auto& elem : rhs ) {
      os << " " << elem;
   }
   os << " )";
   return os;
}

void swap( unsigned& a, unsigned& b ) {
   unsigned tmp = a;
   a = b;
   b = tmp;
}

// partition algorithm from the course material.
// 'left' is inclusive, 'right' is exclusive.
unsigned partition( std::vector<unsigned>& input, unsigned left_index, unsigned right_index ) {
   unsigned* left  = &input[left_index];
   unsigned* right = &input[left_index] + right_index - left_index;

   unsigned pivot = *left; // pivot is the first item

   unsigned* i = left + 1;
   for ( unsigned* j = left + 1; j < right; ++j ) {
      if ( *j < pivot ) {
         swap( *i, *j );
         ++i;
      }
   }
   swap( *left, *( i - 1 ) );
   return left_index + ( i - 1 - left );
}

// 'left' is inclusive, 'right' is exclusive.
long quick_sort( std::vector<unsigned>& input, unsigned left_index, unsigned right_index ) {
   if ( right_index <= left_index + 1 ) {
      return 0;
   }

   if ( DEBUG_MODE ) {
      std::cout << "quick_sort (" << left_index << ", " << right_index << ") , cost:" << ( right_index - left_index - 1 ) << std::endl;
   }
   unsigned pivot_position = partition( input, left_index, right_index );

   long comparsions = right_index - left_index - 1; // we know this about the current pivoting algorith
   comparsions += quick_sort( input, left_index, pivot_position );
   comparsions += quick_sort( input, pivot_position + 1, right_index );
   
   return comparsions;
}

long quick_sort( std::vector<unsigned>& input ) {
   return quick_sort( input, 0, input.size() );
}

unsigned middle_position( std::vector<unsigned>& input, unsigned left_index, unsigned right_index ) {
   if ( right_index <= left_index ) {
      std::cout << "ERROR in middle_position computation." << std::endl;
      return left_index;
   }
   unsigned k = ( right_index - left_index - 1) / 2;
   return left_index + k;
}

bool is_first_in_the_middle( unsigned* pos1, unsigned *pos2, unsigned *pos3 ) {
   return *pos1 <= std::max( *pos2, *pos3 ) && *pos1 >= std::min( *pos2, *pos3 );
}

unsigned* middle_of_three( unsigned* pos1, unsigned *pos2, unsigned *pos3 ) {
   if ( is_first_in_the_middle( pos1, pos2, pos3 ) ) {
      return pos1;
   }
   if ( is_first_in_the_middle( pos2, pos1, pos3 ) ) {
      return pos2;
   }
   return pos3;
}

void test_middle_position() {
   std::vector<unsigned> example3( { 8, 2, 4, 5, 7, 1 } );
   for ( unsigned y = 0; y < example3.size(); ++y ) {
      unsigned z = y +1;
      std::cout << " middle_position of " << example3 << " [0," << z << "[ is " << middle_position( example3, 0, z ) << std::endl;
   }
}

void test_middle_of_three() {
   std::vector<unsigned> example3( { 8, 2, 4, 5, 6, 7, 1, 3, 2 } );
   for ( unsigned y = 0; y < example3.size()-2; ++y ) {
      std::cout << " middle of (" << example3[y] << ", " << example3[y+1] << ", " << example3[y+2] << ") is :"
         << *middle_of_three( &example3[y], &example3[y+1], &example3[y+2] ) << std::endl;
   }
}

int main( int argc, const char* argv[] ) {
   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc != 2 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename>" << std::endl;
      return 0;
   } else {
      std::string filename = argv[1];
      std::vector<unsigned> nums;
      if ( !readInput( argv[1], nums, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      if ( DEBUG_MODE ) {
         std::cout << "=== TESTS:" << std::endl;
         test_middle_position();
         test_middle_of_three();
         std::cout << std::endl;
         std::cout << "=== INPUT:" << std::endl;
         std::cout << nums << std::endl << std::endl;
      }

      long comparsions = quick_sort( nums );

      if ( DEBUG_MODE ) {
         std::cout << std::endl << "=== OUTPUT:" << std::endl;
         std::cout << nums << std::endl << std::endl;
      }

      std::cout << comparsions << std::endl;
   }
}
