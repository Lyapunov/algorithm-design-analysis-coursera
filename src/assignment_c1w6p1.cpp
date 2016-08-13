#include <string>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "fast_integer_file_reader.h"

static const int DEBUG_MODE = 0;

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

      std::sort( input.begin(), input.end() );

      long lower = -10000; // inclusive
      long upper = 10000;  // inclusive
      unsigned first = input.size() - 1;

      long counter = 0;

      // lower <= x + y < upper
      for ( unsigned i = 0; i < input.size() - 1; ++i ) {
         if ( input[i] + input[i + 1] > upper ) {
            break;
         }
         // if input[i] + input[j] > upper, then input[i+1] + input[j] > upper, because of 
         // the ordered input
         while ( input[i] + input[first] > upper && first > i ) {
            --first;
         }
         unsigned j = first;
         while ( input[i] + input[j] >= lower && j > i ) {
            if ( DEBUG_MODE ) {
               std::cout << input[i] << " " << input[j] << " " << ( input[i] + input[j] ) << std::endl;
            }
            if ( input[i] != input[j] ) {
               ++counter;
            }
            --j;
         }
      }
      std::cout << counter << std::endl;
   }
}
