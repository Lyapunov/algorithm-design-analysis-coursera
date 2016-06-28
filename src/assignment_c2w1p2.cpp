#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <algorithm>

#include "common_c2w1p1.h"

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
      std::string filename = argv[1];
      std::vector< Stuff > stuffs;
      if ( !readInput( argv[1], stuffs, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      std::sort(stuffs.begin(), stuffs.end(), [](Stuff a, Stuff b) {
         return ( static_cast<double>(b.weight) / static_cast<double>(b.length) ) < ( static_cast<double>(a.weight) / static_cast<double>(a.length) );
      });

      if ( DEBUG_MODE ) {
         std::cout << "=== PRINTING SORTED DATA" << std::endl;
      }

      long completition = 0;
      long cost = 0;
      for ( const auto& elem : stuffs ) {
         completition += elem.length;
         cost += completition * elem.weight;
         if ( DEBUG_MODE ) {
            std::cout << "--- --- " << elem.weight << " " << elem.length << "; completition : " << completition << "; totalcost: " << cost << std::endl; 
         }
      }
      std::cout << cost << std::endl;
   }
}
