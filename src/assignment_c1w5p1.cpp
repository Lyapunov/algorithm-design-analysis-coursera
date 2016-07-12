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

static const int DEBUG_MODE = 0;

DistanceVector dijkstra( const GraphAL& graph, int start ) {
   // init
   constexpr int infinite = 2<<28;

   DistanceVector retval( graph.n, start );

   MyHeap< int, int > heap( graph.n );
   for ( int i = 0; i < static_cast<int>( graph.n ); ++i ) {
      heap.insert( i, i == start ? 0 : infinite  );
   }

   // main loop
   while ( heap.size() > 0 ) {
      const auto winner = heap.pop();

      retval.set( winner.first, winner.second );

      // rewiring the heap
      for ( const auto& candidate : graph.alist[ winner.first ] ) {
         if ( heap.check( candidate.second ) ) {
            const auto tuple = heap.remove( candidate.second );
            heap.insert( candidate.second, std::min( tuple.second, std::min( winner.second + candidate.cost, infinite ) ) );
         }
      }
   }

   return retval;
}

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

      DistanceVector result = dijkstra( graph, start_node );
      int total = 0;
      for ( const auto& elem : end_nodes ) {
         total += result.get( elem );
      }
      std::cout << total << std::endl;
   }
}
