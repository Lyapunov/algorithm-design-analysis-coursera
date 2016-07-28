#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "kosaraju.h"

#include <map>
#include <set>

static const int DEBUG_MODE = 0;

int main( int argc, const char* argv[] ) {
   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc != 2 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename> " << std::endl;
      return 0;
   } else{ 
      LightweightGraphAL graph;
      std::string filename = argv[1];
//      if ( !cStyleReadALGraphFromEdgeListWithoutCost( filename, graph, DEBUG_MODE ) ) {
//      if ( !fasterReadALGraphFromEdgeListWithoutCost( filename, graph, DEBUG_MODE ) ) {
      if ( !shamelessReadALGraphFromEdgeListWithoutCost( filename, graph, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }
      if ( DEBUG_MODE ) {
         std::cout << graph << std::endl;
      }

      std::vector<unsigned> result = kosaraju( graph );

      if ( DEBUG_MODE ) {
         std::cout << "=== Strongly connected compontent descriptor:" << std::endl;
         std::cout << "--- " << result << std::endl;
      }

      std::map<unsigned, unsigned> component_sizes_by_representant;
      for ( const auto& elem : result ) {
         component_sizes_by_representant[ elem ]++; 
      }

      // http://stackoverflow.com/questions/22591645/cstd-map-key-with-descending-order
      std::multiset<unsigned, std::greater<int>> component_sizes;
      for ( const auto& elem : component_sizes_by_representant ) {
         component_sizes.insert( elem.second );
      }
      unsigned counter = 0;
      for ( const auto& elem : component_sizes ) {
         std::cout << elem;

         counter++;
         if ( counter >= 5 ) {
            break;
         }
         if ( counter < component_sizes.size() ) {
            std::cout << ",";
         }
      }
      std::cout << std::endl;
   }
}
