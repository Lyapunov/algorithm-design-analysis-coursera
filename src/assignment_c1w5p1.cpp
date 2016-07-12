#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "dijkstra.h"

static const int DEBUG_MODE = 0;

int main( int argc, const char* argv[] ) {
   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc < 4 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename> <start node> <end node> [<end node2> .. ] " << std::endl;
      return 0;
   } else{ 
      GraphAL graph;
      std::string filename = argv[1];
      const int start_node = atoi(argv[2]);
      std::vector<int> end_nodes;
      for ( int i = 3; i < argc; ++i ) {
         const int end_node = atoi(argv[i]);
         end_nodes.push_back( end_node );
      }
      if ( !readGraph( filename, graph, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      if ( DEBUG_MODE ) {
         std::cout << graph << std::endl;
      }

      std::vector<int> result = dijkstra_stl( graph, start_node );
      int total = 0;
      for ( const auto& elem : end_nodes ) {
         total += result[ elem ];
      }
      std::cout << total << std::endl;
   }
}
