#ifndef KOSARAJU
#define KOSARAJU

#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "graph_adjacency_list.h"

std::ostream& operator<<( std::ostream& os, const std::vector<unsigned>& rhs ) {
   os << "vector<unsigned>( ";
   for ( const auto elem: rhs ) {
      os << elem << " ";
   }
   os << ")";
   return os;
}

class DfsLooper {
public:
   DfsLooper( const GraphAL& graph, const std::vector<unsigned>* startings = 0 )
    : graph_( graph ),
      visited_( graph.n, false ),
      ending_counter_( 0 ),
      representant_( 0 ),
      endings_( graph.n, 0 ),
      components_( graph.n, 0 )
   {
      if ( startings ) {
         // reading backwards, amend to Kosaraju's algorithm
         for ( unsigned i = 0; i < startings->size(); ++i ) {
            unsigned elem = startings->operator[]( startings->size() - 1 - i );
            representant_ = elem;
            dfs( elem );
         }
      } else {
         for ( unsigned i = 0; i < graph.n; ++i ) {
            representant_ = i;
            dfs( representant_ );
         }
      }
   }

   std::vector<unsigned> getEndings() { return endings_; }
   std::vector<unsigned> getComponents() { return components_; }

private:
   void dfs( unsigned subject ) {
      if ( visited_[subject] ) {
         return;
      }
      visited_[subject] = true;
      for ( const auto edge : graph_.alist[ subject ] ) {
         dfs( edge.second ); 
      }
      endings_[ending_counter_] = subject;
      components_[ subject ] = representant_;
      ending_counter_++;
   }

   const GraphAL& graph_;
   std::vector<bool> visited_;
   unsigned ending_counter_;
   unsigned representant_; 
   std::vector<unsigned> endings_;
   std::vector<unsigned> components_;
};

// Kosaraju's algorithm to find strongly connected components

std::vector<unsigned> kosaraju( const GraphAL& graph ) {
   const GraphAL inverted = graph.invert();
   DfsLooper looper1( inverted );
   std::vector<unsigned> magic = looper1.getEndings();
   DfsLooper looper2( graph, &magic );
   return looper2.getComponents();
}

#endif /*KOSARAJU*/
