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

unsigned long tabletKey( int i, int W ) {
   return static_cast<unsigned long>(W) * UINT_MAX + static_cast<unsigned>(i);
}

int knapsack( int i, int W, const std::vector< Item >& items, std::unordered_map<unsigned long, int>& tablet ) {
   // trivial case
   if ( i <= 0 || W <= 0 ) {
      return 0;
   }

   // cache
   const auto& found = tablet.find( tabletKey( i, W ) );
   if ( found != tablet.end() ) {
      return found->second;
   }

   // max capacity isn't enough to take the ith object:
   const Item& myItem = items[i - 1];
   int rest_capacity = W - myItem.weight;

   // main case
   int retval = rest_capacity >= 0
                ? std::max( knapsack( i - 1, W, items, tablet ), knapsack( i - 1, rest_capacity, items, tablet ) + myItem.value )
                : knapsack( i - 1, W, items, tablet );
   tablet[ tabletKey( i, W ) ] = retval;

   if ( DEBUG_MODE ) {
      std::cout << "knapsack( size:" << i << ", max capacity: " << W << " ) = " << retval << std::endl;
   }
   return retval;
}

int knapsack( int i, int W, const std::vector< Item >& items  ) {
   std::unordered_map<unsigned long, int> tablet; 

   return knapsack( i, W, items, tablet );
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

      // decreasig order by value
      std::sort(items.begin(), items.end(), [](Item a, Item b) {
         return a.value >= b.value;
      });

      if ( DEBUG_MODE ) {
         int total = 0;
         std::cout << "=== PRINTING SORTED ITEMS" << std::endl;
         for ( const auto& elem : items ) {
            if ( DEBUG_MODE ) {
               std::cout << elem << std::endl;
               total += elem.value;
            }
         }
         std::cout << "--- i = " << items.size() << ", W = " << knapsack_size << std::endl;
         std::cout << "--- totalvalue = " << total << std::endl;
      }

      // starting the main part
      std::cout << knapsack( items.size(), knapsack_size, items ) << std::endl;
   }
}
