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

class ComponentsWorker
{
public:
   ComponentsWorker( unsigned size )
    : representant_( 0 ),
      components_( size, 0 )
   {}

   void workerBookRepresentant( unsigned subject ) {
      representant_ = subject;
   }

   void workerBookSubject( unsigned subject ) {
      components_[ subject ] = representant_;
   }

   std::vector<unsigned> getComponents() const { return components_; }
private:
   unsigned representant_; 
   std::vector<unsigned> components_;
};

class EndingNumbersWorker
{
public:
   EndingNumbersWorker( unsigned size )
    : ending_counter_( 0 ),
      endings_( size, 0 )
   {}

   void workerBookRepresentant( unsigned subject ) {}

   void workerBookSubject( unsigned subject ) {
      endings_[ending_counter_] = subject;
      ending_counter_++;
   }

   std::vector<unsigned> getEndings() const { return endings_; }
private:
   unsigned ending_counter_;
   std::vector<unsigned> endings_;
};

template <class T>
class DfsLooper : public T {
public:
   DfsLooper( const GraphAL& graph, const std::vector<unsigned>* startings = 0 )
    : T( graph.n ),
      graph_( graph ),
      visited_( graph.n, false )
   {
      if ( startings ) {
         // reading backwards, amend to Kosaraju's algorithm
         for ( unsigned i = 0; i < startings->size(); ++i ) {
            unsigned elem = startings->operator[]( startings->size() - 1 - i );
            this->workerBookRepresentant( elem );
            dfs( elem );
         }
      } else {
         for ( unsigned i = 0; i < graph.n; ++i ) {
            this->workerBookRepresentant( i );
            dfs( i );
         }
      }
   }

private:
   void dfs( unsigned subject ) {
      if ( visited_[subject] ) {
         return;
      }
      visited_[subject] = true;
      for ( const auto edge : graph_.alist[ subject ] ) {
         dfs( edge.second ); 
      }
      this->workerBookSubject( subject );
   }

   const GraphAL& graph_;
   std::vector<bool> visited_;
};

// Kosaraju's algorithm to find strongly connected components

std::vector<unsigned> kosaraju( const GraphAL& graph ) {
   const GraphAL inverted = graph.invert();
   DfsLooper<EndingNumbersWorker> looper1( inverted );
   std::vector<unsigned> magic = looper1.getEndings();
   DfsLooper<ComponentsWorker> looper2( graph, &magic );
   return looper2.getComponents();
}

#endif /*KOSARAJU*/
