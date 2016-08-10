#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <algorithm>

static const int DEBUG_MODE = 0;

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

      if ( DEBUG_MODE ) {
         std::cout << "=== PRINTING SORTED DATA" << std::endl;
         std::cout << nums;
      }

   }
}
