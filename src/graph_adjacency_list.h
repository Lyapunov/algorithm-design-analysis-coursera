#ifndef GRAPH_ADJACENCY_LIST
#define GRAPH_ADJACENCY_LIST

#include <cassert>

#include "edge.h"
#include "graph_edgelist.h"

struct GraphAL {
   unsigned n = 0;
   std::vector< std::vector<Edge> > alist;
   GraphAL invert() const {
      GraphAL retval;
      retval.n = this->n;
      for ( unsigned i = 0; i < this->n; ++i ) {
         retval.alist.push_back( std::vector<Edge>() );
      }
      for ( const auto& inlist : this->alist ) {
         for ( const auto& elem : inlist ) {
            Edge inverted;
            inverted.first  = elem.second;
            inverted.second = elem.first;
            inverted.cost   = elem.cost;

            retval.alist[ inverted.first ].push_back( inverted );
         }
      }
      return retval;
   }
};

GraphAL constructGraphALFromGraph( const Graph& graph ) {
   GraphAL retval;
   retval.n = graph.n;
   for ( unsigned i = 0; i < graph.n; ++i ) {
      retval.alist.push_back( std::vector<Edge>() );
   }
   for ( const auto& edge : graph.edges ) {
      retval.alist[edge.first].push_back( edge );
   }
   return retval;
}

std::ostream& operator<<( std::ostream& os, const GraphAL& graph ) {
   os << "GraphAL( " << graph.n << std::endl;
   for ( unsigned i = 0; i < graph.alist.size(); ++i ) {
      os << "--- " << i << ": ";
      for ( const auto& elem : graph.alist[i] ) {
         os << elem.second << "[" << elem.cost << "] ";
      }
      os << std::endl;
   }
   os << ")";
   return os;
}

static bool readGraph( std::string filename, GraphAL& graph, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   // first line
   std::string line;
   if ( !std::getline( is, line ) ) {
      return false;
   }
   std::istringstream ss(line);

   GraphAL retval;

   if ( !(ss >> retval.n ) ) {
      return false;
   }

   if ( debugmode ) {
      std::cout << "=== READING GRAPH(" << retval.n << ")" << std::endl;
   }

   int number = 0;
   int loopNumber = 0;
   while (true) {
      if ( !std::getline( is, line ) ) {
         is.close();
         graph = retval;
         return true;
      }
      std::istringstream ss(line);
      ++loopNumber;
      ss >> number;
      if ( loopNumber != number ) {
         return false;
      }
      retval.alist.push_back( std::vector<Edge>() );
      while ( ss ) {
         Edge edge;
         edge.first = number;
         std::string trash;
         ss >> edge.second;
         std::getline(ss, trash, ',');
         ss >> edge.cost;
         if ( !ss ) {
            continue;
         }
         --edge.first;
         --edge.second;
         retval.alist[number - 1].push_back( edge );
      }
   }
}

#endif /*GRAPH_ADJACENCY_LIST*/
