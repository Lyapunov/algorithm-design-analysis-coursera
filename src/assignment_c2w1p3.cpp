#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <set>
#include <vector>
#include <algorithm>

#include "graph_edgelist.h"

static std::vector< Edge > slow_prim( const Graph& graph, int debug_mode = 0 ) {
   std::vector< Edge > spanning_tree_edges;
   std::set< int > A;

   // A graph with less than 2 vertices has no spanning tree
   if ( graph.n < 2 ) {
      return spanning_tree_edges;
   }

   // Taking the first edge is somehow special
   Edge firstEdge = graph.edges[0];
   for ( const auto& elem : graph.edges ) {
      if ( elem.cost < firstEdge.cost ) {
         firstEdge = elem;
      }
   }
   if ( debug_mode ) {
      std::cout << "=== The first edge is " << firstEdge << std::endl;
   }
   A.insert( firstEdge.first );
   A.insert( firstEdge.second );
   spanning_tree_edges.push_back( firstEdge );

   while ( A.size() < static_cast<unsigned>( graph.n ) ) {
      // Taking the next edge
      Edge nextEdge;
      for ( const auto& elem : graph.edges ) {
         if ( A.count( elem.first ) + A.count( elem.second ) == 1 
              && ( !nextEdge.isValid() || elem.cost < nextEdge.cost ) ) {
            nextEdge = elem;
         }
      }
      if ( debug_mode ) {
         std::cout << "=== The next edge is " << nextEdge << std::endl;
      }
      A.insert( nextEdge.first );
      A.insert( nextEdge.second );
      spanning_tree_edges.push_back( nextEdge );
   }

   return spanning_tree_edges;
}

static const int DEBUG_MODE = 0;

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
      if ( DEBUG_MODE ) {
         std::cout << graph << std::endl; 
      }
      std::vector< Edge > spanning_tree_edges = slow_prim( graph, DEBUG_MODE );
      long total_cost = 0;
      for ( const auto& elem : spanning_tree_edges ) {
         total_cost += elem.cost; 
      }
      // the result
      std::cout << total_cost << std::endl;
   }
}
