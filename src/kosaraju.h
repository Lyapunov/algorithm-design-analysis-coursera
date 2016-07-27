#ifndef KOSARAJU
#define KOSARAJU

#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "graph_adjacency_list.h"

#define ITERATIVE 0

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

template <class Worker, class GraphRepresentant = GraphAL>
class DfsLooper : public Worker {
public:
   DfsLooper( const GraphRepresentant& graph, const std::vector<unsigned>* startings = 0 )
    : Worker( graph.n ),
      graph_( graph ),
      visited_( graph.n, false ),
      pmStack_( ITERATIVE ? graph.n : 0 )
   {
      if ( startings ) {
         // reading backwards, amend to Kosaraju's algorithm
         for ( unsigned i = 0; i < startings->size(); ++i ) {
            unsigned elem = startings->operator[]( startings->size() - 1 - i );
            this->workerBookRepresentant( elem );
            if ( !ITERATIVE ) {
               dfs( elem );
            } else {
               iterativeDfs( elem );
            }
         }
      } else {
         for ( unsigned i = 0; i < graph.n; ++i ) {
            this->workerBookRepresentant( i );
            if ( !ITERATIVE ) {
               dfs( i );
            } else {
               iterativeDfs( i );
            }
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
      for ( unsigned i = 0; i < graph_.numOfOutEdges( subject ); ++i ) {
         dfs( graph_.ithOutEdge( subject, i ) ); 
      }
      this->workerBookSubject( subject );
   }

   // iterative solution
   void iterativeDfs( unsigned subject ) {
      if ( !visited_[subject] ) {
         visited_[subject] = true;
         pmStack_.push( std::pair<unsigned, unsigned>( subject, graph_.numOfOutEdges( subject ) ) );
      }

      std::pair< unsigned, unsigned>* pCurrent;
      pmStack_.get( &pCurrent );
      while ( !pmStack_.empty() ) {
         unsigned neighbour ;
         while ( pCurrent->second > 0 && visited_[ neighbour = graph_.ithOutEdge( pCurrent->first, pCurrent->second - 1 ) ] ) {
            --pCurrent->second;
         }

         if ( pCurrent->second > 0 ) {
            --pCurrent->second;
            visited_[neighbour] = true;
            pmStack_.push( std::pair<unsigned, unsigned>( neighbour, graph_.numOfOutEdges( neighbour ) ) );
            pmStack_.get( &pCurrent );
         } else {
            // ending!
            this->workerBookSubject( pCurrent->first );
            pmStack_.pop();
            pmStack_.get( &pCurrent );
         }
      }
   }

   const GraphRepresentant& graph_;
   std::vector<bool> visited_;
   PMStack pmStack_;
};

// Kosaraju's algorithm to find strongly connected components

template <class GraphRepresentant = GraphAL>
std::vector<unsigned> kosaraju( const GraphRepresentant& graph ) {
   const GraphRepresentant inverted = graph.invert();
   DfsLooper<EndingNumbersWorker, GraphRepresentant> looper1( inverted );
   std::vector<unsigned> magic = looper1.getEndings();
   DfsLooper<ComponentsWorker, GraphRepresentant> looper2( graph, &magic );
   return looper2.getComponents();
}

#endif /*KOSARAJU*/
