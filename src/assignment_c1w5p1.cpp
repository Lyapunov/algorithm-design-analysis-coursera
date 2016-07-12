#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <set>
#include <vector>
#include <algorithm>

#include "graph_adjacency_list.h"
#include "makeshift_stl_heap.h"

static const int DEBUG_MODE = 0;

//
// The simpler version, with the makeshift (map + multimap) heap.
// Easier to read, but ~20% slower. ( Because of invoking find() twice. )
//
std::vector<int> dijkstra( const GraphAL& graph, int start ) {
   // init
   constexpr int infinite = 2<<28;

   std::vector<int> retval( graph.n, 0);
   MakeshiftStlHeap<int, int> heap;

   for ( int i = 0; i < static_cast<int>( graph.n ); ++i ) {
      retval[i] = ( i == start ? 0 : infinite );
      heap.insert( i, retval[i] );
   }

   // main loop
   while ( !heap.empty() ) {
      const auto winnerPair = heap.pop();
      const auto winner_value = winnerPair.first;
      const auto winner_node  = winnerPair.second;

      retval[ winner_node ] = winner_value;

      // rewiring the heap
      for ( const auto& candidate : graph.alist[ winner_node ] ) {
         auto i = candidate.second;
         if ( const int* previous_value = heap.getValue( i ) ) {
            heap.insert( i, std::min( *previous_value, std::min( winner_value + candidate.cost, infinite ) ) );
         }
      }
   }

   return retval;
}

//
// The raw but slightly faster, only-stl-containers version.
//
std::vector<int> dijkstra_stl( const GraphAL& graph, int start ) {
   // init
   constexpr int infinite = 2<<28;

   std::vector<int> retval( graph.n, 0);
   std::multimap<int, int> heap;
   std::map< int, std::multimap<int, int>::iterator > heap_accessors;

   // the key is that [multi]{set,map} are invalidated only for an erased item
   // so, there is no obstacle maintaining 'accessor' iterators in a separate vector
   for ( int i = 0; i < static_cast<int>( graph.n ); ++i ) {
      retval[i] = ( i == start ? 0 : infinite );
      heap_accessors[i] = heap.insert( std::pair<int, int>( retval[i], i ) );
   }

   // main loop
   while ( heap.size() > 0 ) {
      const auto winner_value = heap.begin()->first;
      const auto winner_node  = heap.begin()->second;
      retval[ winner_node ] = winner_value;

      heap.erase( heap.begin() );
      heap_accessors.erase( winner_node );

      // rewiring the heap
      for ( const auto& candidate : graph.alist[ winner_node ] ) {
         auto i = candidate.second;
         auto itAccessor = heap_accessors.find( i );
         if ( itAccessor != heap_accessors.end() ) {
            auto it = itAccessor->second;
            auto previous_value = it->first;
            heap.erase( it );
            heap_accessors[i] = heap.insert( std::pair<int, int>( std::min( previous_value, std::min( winner_value + candidate.cost, infinite ) ), i ) );
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

      std::vector<int> result = dijkstra_stl( graph, start_node );
      int total = 0;
      for ( const auto& elem : end_nodes ) {
         total += result[ elem ];
      }
      std::cout << total << std::endl;
   }
}
