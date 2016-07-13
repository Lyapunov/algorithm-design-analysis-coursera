#ifndef DIJKSTRA
#define DIJKSTRA

#include <map>
#include <vector>
#include <algorithm>

#include "graph_adjacency_list.h"
#include "makeshift_stl_heap.h"
#include "my_heap.h"

//
// The homegrown version. Fast and furious.
//
std::vector<int> dijkstra( const GraphAL& graph, int start ) {
   // init
   constexpr int infinite = 2<<28;

   std::vector<int> retval( graph.n, 0);
   MyHeap<int, int> heap( graph.n );

   for ( int i = 0; i < static_cast<int>( graph.n ); ++i ) {
      retval[i] = ( i == start ? 0 : infinite );
      heap.insert( i, retval[i] );
   }

   // main loop
   while ( !heap.empty() ) {
      const auto winnerPair = heap.pop();
      const auto winner_node  = winnerPair.first;
      const auto winner_value = winnerPair.second;

      retval[ winner_node ] = winner_value;

      // rewiring the heap
      for ( const auto& candidate : graph.alist[ winner_node ] ) {
         auto i = candidate.second;
         const auto heapPair = heap.remove( i );
         if ( heapPair.first >= 0 ) {
            heap.insert( i, std::min( heapPair.second, std::min( winner_value + candidate.cost, infinite ) ) );
         }
      }
   }

   return retval;
}

//
// The simpler version, with the makeshift (map + multimap) heap.
// Easier to read.
//
std::vector<int> dijkstra_makeshift_heap( const GraphAL& graph, int start ) {
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
// The raw, only-stl-containers version.
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

std::vector<int> dijkstra_stl( const Graph& graph, int start ) {
   const GraphAL graphAL = constructGraphALFromGraph( graph );
   return dijkstra( graphAL, start );
}

#endif /*DIJKSTRA*/
