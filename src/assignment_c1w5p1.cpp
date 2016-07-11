#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <set>
#include <vector>
#include <algorithm>

#include "graph_adjacency_list.h"
#include "distance_table.h"
#include "my_heap.h"

static const int DEBUG_MODE = 1;

DistanceVector dijkstra( const GraphAL& graph, unsigned start ) {
   // init
   DistanceVector retval( graph.n, start );
   std::set<unsigned> visited_nodes;
   visited_nodes.insert( start );
   MyHeap< unsigned, Edge> heap( graph.n );
   for ( const auto& elem : graph.alist[ start ] ) {
      heap.insert( elem.second, elem );
   }

   // main loop
   while ( heap.size() > 0 && visited_nodes.size() < graph.n ) {
      std::pair< unsigned, Edge > pack = heap.pop();
      const Edge& edge = pack.second;
      retval.set( edge.second, retval.get( edge.first ) + edge.cost );
      visited_nodes.insert( edge.second );
      // rewiring the heap
      for ( const auto& candidate : graph.alist[ edge.second ] ) {
         std::pair< unsigned, Edge > tuple = heap.remove( candidate.second );
         const Edge& current = tuple.second;
         heap.insert( candidate.second, current.cost <= candidate.cost ? current : candidate );
      }
   }

   return retval;
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
      GraphAL graph;
      std::string filename = argv[1];
      if ( !readGraph( filename, graph, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      if ( DEBUG_MODE ) {
         std::cout << graph << std::endl;
      }
   }
}
