#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <algorithm>
#include <unordered_map>

#include <climits>

#include "common_c2w3p2.h"

static const int DEBUG_MODE = 0;

int knapsack( int i, int W, const std::vector< Item >& items ) {
   // need a tablet
   int tablet[i+1][W+1];

   // noob, but ...
   for ( int j = 0; j <= i; ++j ) {
      for ( int k = 0; k <= W; ++k ) {
         tablet[j][k] = 0;
      }
   }

   for ( int j = 1; j <= i; ++j ) {
      for ( int k = 1; k <= W; ++k ) {
         const Item& myItem = items[j - 1];
         int rest_capacity = k - myItem.weight;
         tablet[j][k] = rest_capacity >= 0
                        ? std::max( tablet[j-1][k], tablet[j-1][rest_capacity] + myItem.value )
                        : tablet[j-1][k];
      }
   }

   return tablet[i][W];
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
      int knapsack_size = 0;
      std::vector< Item > items;
      if ( !readInput( argv[1], knapsack_size, items, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      if ( DEBUG_MODE ) {
         std::cout << "=== W = " << knapsack_size << std::endl;
         std::cout << "=== PRINTING ITEMS" << std::endl;
         for ( const auto& elem : items ) {
            if ( DEBUG_MODE ) {
               std::cout << elem << std::endl;
            }
         }
      }

      // starting the main part
      std::cout << knapsack( items.size(), knapsack_size, items ) << std::endl;
   }
}
