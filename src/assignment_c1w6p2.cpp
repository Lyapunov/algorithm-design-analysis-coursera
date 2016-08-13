#include <string>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "fast_integer_file_reader.h"

static const int DEBUG_MODE = 1;

int main( int argc, const char* argv[] ) {
   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc != 2 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename>" << std::endl;
      return 0;
   } else{ 
      std::vector<long> input;
      input.reserve(1100000);
      std::string filename = argv[1];
      if ( !fastIntegerFileReader( filename, input, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      if ( DEBUG_MODE ) {
         std::cout << input << std::endl;
         std::cout << input.size() << std::endl;
      }
   }
}
