#include <string>
#include <string.h>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <set>
#include <vector>
#include <algorithm>

#include "graph_edgelist.h"
#include "distance_table.h"

static const  int DEBUG_MODE = 0;

DistanceTable floyd( const Graph& graph ) {
   DistanceTable current_table( graph.n );
   DistanceTable next_table( graph.n );

   for ( const auto& elem : graph.edges ) {
      current_table.set( elem.first - 1, elem.second - 1, elem.cost );
   }
   if ( DEBUG_MODE ) {
      std::cout << current_table << std::endl;
   }

   for (unsigned k = 0; k < graph.n; ++k ) {
      for (unsigned i = 0; i < graph.n; ++i ) {
         for (unsigned j = 0; j < graph.n; ++j ) {
            next_table.set( i, j, std::min( current_table.get(i, j), current_table.get(i, k) + current_table.get(k, j) ) );
         }
      }
      current_table.swap( next_table );
      if ( DEBUG_MODE ) {
         std::cout << current_table << std::endl;
      }
   }

   return current_table;
}

int main( int argc, const char* argv[] ) {
   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc != 2 && argc != 3 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename>" << std::endl;
      return 0;
   } else{ 
      const std::string filename = argv[1];

      Graph graph;
      if ( !readGraph( filename, graph, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      auto result = floyd( graph );
      // detecting negative cycles
      for ( unsigned k = 0; k < graph.n; ++k ) {
         if ( result.get(k, k) < 0 ) {
            std::cout << "Detected negative circle!" << std::endl;
            return 1;
         }
      }

      int minimum = result.get(0, 0);
      for (unsigned i = 0; i < graph.n; ++i ) {
         for (unsigned j = 0; j < graph.n; ++j ) {
            minimum = std::min( minimum, result.get(i, j) );
         }
      }
      std::cout << minimum << std::endl;
   }
}
