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

// poor man's stack
class PMStack {
public:
   PMStack( const unsigned n )
    : storage_( n, std::pair<unsigned, unsigned>(0, 0 ) ),
      pos_( 0 )
   {}
   
   void push( const std::pair<unsigned, unsigned>&& value ) {
      storage_[ pos_ ] = value;
      ++pos_;
   }

   bool pop() {
      if ( pos_ > 0 ) {
         --pos_;
         return true;
      }
      return false;
   }
   
   bool empty() {
      return pos_ == 0;
   }

   bool get( std::pair<unsigned, unsigned>** ptr ) {
      if ( pos_ > 0 ) {
         *ptr = &storage_[ pos_ - 1 ];
         return true;
      }
      return false;
   }

private:
   std::vector<std::pair<unsigned, unsigned>> storage_;
   unsigned pos_;
};

template <class T>
class DfsLooper : public T {
public:
   DfsLooper( const GraphAL& graph, const std::vector<unsigned>* startings = 0 )
    : T( graph.n ),
      graph_( graph ),
      visited_( graph.n, false ),
      pmStack_( graph.n )
   {
      if ( startings ) {
         // reading backwards, amend to Kosaraju's algorithm
         for ( unsigned i = 0; i < startings->size(); ++i ) {
            unsigned elem = startings->operator[]( startings->size() - 1 - i );
            this->workerBookRepresentant( elem );
//            dfs( elem );
            iterativeDfs( elem );
         }
      } else {
         for ( unsigned i = 0; i < graph.n; ++i ) {
            this->workerBookRepresentant( i );
//            dfs( i );
            iterativeDfs( i );
         }
      }
   }

private:
   // recursive solutiion
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

   // iterative solution
   void iterativeDfs( unsigned subject ) {
      if ( !visited_[subject] ) {
         visited_[subject] = true;
         pmStack_.push( std::pair<unsigned, unsigned>( subject, graph_.alist[ subject ].size() ) );
      }

      std::pair< unsigned, unsigned>* pCurrent;
      pmStack_.get( &pCurrent );
      while ( !pmStack_.empty() ) {
         if ( pCurrent->second > 0 ) {
            unsigned neighbour = graph_.alist[ pCurrent->first ][ --pCurrent->second ].second;
            if ( !visited_[neighbour] ) {
               visited_[neighbour] = true;
               pmStack_.push( std::pair<unsigned, unsigned>( neighbour, graph_.alist[ neighbour ].size() ) );
               pmStack_.get( &pCurrent );
            }
         } else {
            // ending!
            this->workerBookSubject( pCurrent->first );
            pmStack_.pop();
            pmStack_.get( &pCurrent );
         }
      }
   }

   const GraphAL& graph_;
   std::vector<bool> visited_;
   PMStack pmStack_;
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
