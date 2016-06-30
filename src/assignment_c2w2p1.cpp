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

static const  int DEBUG_MODE = 0;

int main( int argc, const char* argv[] ) {
   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc != 2 && argc != 3 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename>" << std::endl;
      return 0;
   } else{ 
      std::string filename = argv[1];
      unsigned k = 4;
      if ( argc == 3 ) {
         std::string kstring = argv[2];
         std::stringstream ss( kstring ); 
         ss >> k;
      }

      if ( DEBUG_MODE ) {
         std::cout << "=== K is " << k << std::endl; 
      }
 
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
      Edge last;
      while ( uf.getNumberOfClusters() >= k ) {
         last = graph.edges[ graph.edges.size() - 1 ];
         if ( DEBUG_MODE ) {
            std::cout << "--- Eliminating " << last << std::endl; 
         }

         uf.doUnion( last.first - 1, last.second -1 );
         graph.edges.pop_back();
         --graph.m;
         if ( DEBUG_MODE ) {
            std::cout << "--- after union: " << std::endl; 
            std::cout << uf << std::endl;
            std::cout << uf.getNumberOfClusters() << std::endl;
         }

      }

      if ( DEBUG_MODE ) {
         std::cout << graph << std::endl; 
      }

      // the answer is the weight of the last remaining edge
      std::cout << last.cost << std::endl; 
   }
}
