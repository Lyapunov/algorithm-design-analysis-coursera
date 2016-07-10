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

static const  int DEBUG_MODE = 1;

DistanceTable floyd( const Graph& graph ) {
   DistanceTable dt( graph.n );
   return dt;
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
   }
}
