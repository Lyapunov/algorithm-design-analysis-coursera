#ifndef BELLMAN_FORD
#define BELLMAN_FORD

#include <vector>
#include <algorithm>

#include "graph_adjacency_list.h"

std::vector<int> bellman_ford( const GraphAL& graph, int start ) {
   // init
   constexpr int infinite = 2<<28;

   const GraphAL inverted = graph.invert();

   std::vector<int> current( graph.n, 0 );
   std::vector<int> next   ( graph.n, 0 );

   // init
   for ( int i = 0; i < static_cast<int>( graph.n ); ++i ) {
      current[i] = ( i == start ? 0 : infinite );
   }

   // main loop
   for ( unsigned i = 0; i < graph.n + 1; ++i ) { // +1 run for detecting circles with negative cost
      for ( unsigned j = 0; j < graph.n; ++j ) {
         int min_candidate_cost = infinite; 
         for ( const auto& edge: inverted.alist[j] ) {
            min_candidate_cost = std::min( min_candidate_cost, current[ edge.second ] + edge.cost );
         }
         next[j] = std::min( current[j], min_candidate_cost );
      }
      current.swap( next );
   }

   // check
   for ( unsigned i = 0; i < graph.n; ++i ) {
      if ( current[i] != next[i] ) {
         return std::vector<int>();
      }
   }

   return next;
}

#endif /*BELLMAN_FORD*/
