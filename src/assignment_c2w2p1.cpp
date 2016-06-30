#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <set>
#include <vector>
#include <algorithm>

#include "graph_edgelist.h"
#include "union_find.h"

static const  int DEBUG_MODE = 1;

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
      Graph graph;
      if ( !readGraph( argv[1], graph, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }
      std::sort(graph.edges.begin(), graph.edges.end(), [](Edge a, Edge b) {
         return ( a.cost >= b.cost );
      });

      if ( DEBUG_MODE ) {
         std::cout << graph << std::endl; 
      }

      UnionFind uf( graph.n );
      while ( uf.getNumberOfClusters() > 4 ) {
         uf.doUnion( (graph.edges.end() - 1)->first - 1, (graph.edges.end() - 1)->second -1 );
         graph.edges.pop_back();
         --graph.m;
      }

      if ( DEBUG_MODE ) {
         std::cout << graph << std::endl; 
      }

      // the answer is the weight of the last remaining edge
      std::cout << (graph.edges.end() - 1)->cost << std::endl; 
   }
}
