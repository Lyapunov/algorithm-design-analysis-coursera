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

DistanceVector dijkstra( const GraphAL& graph, unsigned start ) {
   // init
   DistanceVector retval( graph.n, start );
   std::set<unsigned> visited_nodes;
   visited_nodes.insert( start );
   MyHeap< int, Edge> heap( graph.n );
   for ( const auto& elem : graph.alist[ start ] ) {
      heap.insert( elem.second, elem );
      if ( DEBUG_MODE ) {
         std::cout << "--- Inserting " << elem.second << " " << elem << std::endl;
      }
   }

   // main loop
   while ( heap.size() > 0 && visited_nodes.size() < graph.n ) {
      const auto pack = heap.pop();
      const Edge& winner = pack.second;
      assert( pack.first == winner.second );

      retval.set( winner.second, retval.get( winner.first ) + winner.cost );
      visited_nodes.insert( winner.second );

      if ( DEBUG_MODE ) {
         std::cout << "--- Visiting " << winner.second << ", winner: " << winner << std::endl;
      }

      // rewiring the heap
      for ( const auto& candidate : graph.alist[ winner.second ] ) {
         if ( visited_nodes.find( candidate.second ) != visited_nodes.end() ) { 
            continue;
         }
         if ( heap.check( candidate.second ) ) {
            const auto tuple = heap.remove( candidate.second );
            const Edge& current = tuple.second;
            assert( tuple.first == current.second );

            heap.insert( candidate.second, current.cost <= candidate.cost ? current : candidate );
         } else {
            heap.insert( candidate.second, candidate );
         }
      }
   }

   if ( DEBUG_MODE ) {
      std::cout << retval << std::endl;
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
