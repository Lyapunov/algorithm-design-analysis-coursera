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

static constexpr int DEBUG_MODE = 0;

unsigned long tabletKey( int i, int W ) {
   return ( static_cast<unsigned long>(W) << 32 ) | static_cast<unsigned>(i);
}

int knapsack( int i, int W, const Item* ith_item, std::unordered_map<unsigned long, int>& tablet ) {
   const Item& myItem = *ith_item;
   if ( W < myItem.weight ) {
      return 0;
   }

   const auto myKey = tabletKey( i, W );

   // cache
   const auto& found = tablet.find( myKey );
   if ( found != tablet.end() ) {
      return found->second;
   }

   // main case
   const int retval = std::max( knapsack( i - 1, W, ith_item - 1, tablet ), knapsack( i - 1, W - myItem.weight, ith_item - 1, tablet ) + myItem.value );
   tablet.emplace( myKey, retval );

   if ( DEBUG_MODE ) {
      std::cout << "knapsack( size:" << i << ", max capacity: " << W << " ) = " << retval << std::endl;
   }
   return retval;
}

int knapsack( int i, int W, std::vector< Item >& items ) {
   std::unordered_map<unsigned long, int> tablet; 
   tablet.reserve( 20000000 );

   // decreasig order by weight
   std::sort(items.begin(), items.end(), [](Item a, Item b) {
      return a.weight >= b.weight;
   });

   // fake item to simplify zero check
   Item fake;
   fake.weight = INT_MAX;
   fake.value  = 0;
   items.insert( items.begin(), fake );

   if ( DEBUG_MODE ) {
      int total = 0;
      std::cout << "--- i = " << items.size() << ", W = " << W << std::endl;
      std::cout << "=== PRINTING SORTED ITEMS" << std::endl;
      for ( const auto& elem : items ) {
         if ( DEBUG_MODE ) {
            std::cout << elem << std::endl;
            total += elem.value;
         }
      }
      std::cout << "--- totalvalue = " << total << std::endl;
   }

   return knapsack( i, W, &items[i], tablet );
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
      items.reserve(4000);
      if ( !readInput( argv[1], knapsack_size, items, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      // starting the main part
      std::cout << knapsack( items.size(), knapsack_size, items ) << std::endl;
   }
}
