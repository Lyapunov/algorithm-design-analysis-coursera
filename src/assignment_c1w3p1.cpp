#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "graph_adjacency_list.h"
#include "union_find.h"

static const int DEBUG_MODE = 1;

int main( int argc, const char* argv[] ) {
   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc != 2 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename> " << std::endl;
      return 0;
   } else{ 
      GraphAL graph_al;
      std::string filename = argv[1];
      if ( !readALGraphWithoutCosts( filename, graph_al, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }
      Graph graph = graph_al.undirectedEdgeList();
      if ( DEBUG_MODE ) {
         std::cout << graph_al << std::endl;
         std::cout << graph << std::endl;
      }
      
   }
}
