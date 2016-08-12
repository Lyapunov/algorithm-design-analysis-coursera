#ifndef GRAPH_ADJACENCY_LIST
#define GRAPH_ADJACENCY_LIST

#include <cassert>
#include <limits>
#include <vector>
#include <cstdio>

#include "edge.h"
#include "graph_edgelist.h"

struct GraphAL {
   unsigned n = 0;
   std::vector< std::vector<Edge> > alist;
   GraphAL invert() const {
      GraphAL retval;
      retval.n = this->n;
      retval.alist = std::vector< std::vector<Edge> >( retval.n, std::vector<Edge>() );
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
   void init( unsigned n ) {
      alist = std::vector< std::vector<Edge> >( n );
   }
   void addEdge( unsigned startv, unsigned endv, unsigned cost ) {
      unsigned maxi = std::max( startv, endv );
      if ( alist.size() < maxi ) {
         std::vector< std::vector<Edge> > extension( maxi - alist.size(), std::vector<Edge>() );
         alist.insert( alist.end(), extension.begin(), extension.end() );
         n = maxi;
      }

      Edge edge;
      edge.first  = startv;
      edge.second = endv;
      edge.cost   = cost;
      --edge.first;
      --edge.second;
      alist[ edge.first ].push_back( edge );
   }
   unsigned numOfOutEdges( unsigned subject ) const {
      return this->alist[ subject ].size();
   }
   unsigned ithOutEdge( unsigned subject, unsigned i ) const {
      return this->alist[ subject ][ i ].second;
   }
};

struct LightweightGraphAL {
   unsigned n = 0;
   std::vector< std::vector<unsigned> > alist;
   LightweightGraphAL invert() const {
      LightweightGraphAL retval;
      retval.n = this->n;
      retval.alist = std::vector< std::vector<unsigned> >( retval.n, std::vector<unsigned>() );
      for ( unsigned i = 0; i < this->n; ++i ) {
         const auto& inlist = this->alist[i];
         for ( const auto& elem : inlist ) {
            retval.alist[ elem ].push_back( i );
         }
      }
      return retval;
   }
   void init( unsigned n ) {
      alist = std::vector< std::vector<unsigned> >( n );
   }
   void addEdge( unsigned startv, unsigned endv, unsigned cost ) {
      unsigned maxi = std::max( startv, endv );
      if ( alist.size() < maxi ) {
         std::vector< std::vector<unsigned> > extension( maxi - alist.size(), std::vector<unsigned>() );
         alist.insert( alist.end(), extension.begin(), extension.end() );
         n = maxi;
      }

      alist[ startv - 1 ].push_back( endv - 1 );
   }
   unsigned numOfOutEdges( unsigned subject ) const {
      return this->alist[ subject ].size();
   }
   unsigned ithOutEdge( unsigned subject, unsigned i ) const {
      return this->alist[ subject ][ i ];
   }

};

GraphAL constructGraphALFromGraph( const Graph& graph ) {
   GraphAL retval;
   retval.n = graph.n;
   for ( unsigned i = 0; i < graph.n; ++i ) {
      retval.alist.push_back( std::vector<Edge>() );
   }
   for ( const auto& edge : graph.edges ) {
      Edge transformed;
      transformed.first  = edge.first  - 1;
      transformed.second = edge.second - 1;
      transformed.cost   = edge.cost;
      retval.alist[transformed.first].push_back( transformed );
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

std::ostream& operator<<( std::ostream& os, const LightweightGraphAL& graph ) {
   os << "LightweightGraphAL( " << graph.n << std::endl;
   for ( unsigned i = 0; i < graph.alist.size(); ++i ) {
      os << "--- " << i << ": ";
      for ( const auto& elem : graph.alist[i] ) {
         os << elem << " ";
      }
      os << std::endl;
   }
   os << ")";
   return os;
}

bool readGraph( std::string filename, GraphAL& graph, int debugmode = 0 )
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

bool readALGraphWithoutCosts( std::string filename, GraphAL& graph, int debugmode = 0 )
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
         if ( !( ss >> edge.second ) ) {
            continue;
         }
         edge.cost = 1;
         --edge.first;
         --edge.second;
         retval.alist[number - 1].push_back( edge );
      }
   }
}

template < class GraphRepresentant = GraphAL >
bool readALGraphFromEdgeList( std::string filename, GraphRepresentant& graph, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   if ( debugmode ) {
      std::cout << "=== READING GRAPH()" << std::endl;
   }

   GraphRepresentant retval;
   std::string line;
   while (true) {
      if ( !std::getline( is, line ) ) {
         is.close();
         if ( debugmode ) {
            std::cout << "=== READ " << retval.n << " vertices" << std::endl;
         }
         graph = retval;
         return true;
      }
      std::istringstream ss(line);
      unsigned startv = 0;
      unsigned endv   = 0;
      int cost   = 1;
      if ( !( ss >> startv ) ) {
         return false;
      }
      if ( !( ss >> endv ) ) {
         return false;
      }
      ss >> cost;
      retval.addEdge( startv, endv, cost );
   }
}

// ----------------- Attempts to speed up input file reading

// They say it is fast, but isn't, 2.6 secs on the largest input
template < class GraphRepresentant = GraphAL >
bool cStyleReadALGraphFromEdgeListWithoutCost( std::string filename, GraphRepresentant& graph, int debugmode = 0 )
{
   FILE* file = fopen( filename.c_str(), "r" );

   if ( !file ) {
      return false;
   }

   if ( debugmode ) {
      std::cout << "=== READING GRAPH()" << std::endl;
   }

   GraphRepresentant retval;
   unsigned startv = 0;
   unsigned endv = 0;

   while ( fscanf( file, "%u %u", &startv, &endv ) != EOF ) {
      retval.addEdge( startv, endv, 1 );
   }
   fclose( file );

   if ( debugmode ) {
      std::cout << "=== READ " << retval.n << " vertices" << std::endl;
   }
   graph = retval;
   return true;
}

// Slightly faster, 2.3 secs on the largest input
template < class GraphRepresentant = GraphAL >
bool fasterReadALGraphFromEdgeListWithoutCost( std::string filename, GraphRepresentant& graph, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   if ( debugmode ) {
      std::cout << "=== READING GRAPH()" << std::endl;
   }

   GraphRepresentant retval;
   std::string line;
   while (true) {
      if ( !is ) {
         is.close();
         if ( debugmode ) {
            std::cout << "=== READ " << retval.n << " vertices" << std::endl;
         }
         graph = retval;
         return true;
      }
      unsigned startv = 0;
      unsigned endv   = 0;
      int cost   = 1;
      if ( !( is >> startv ) ) {
         continue;
      }
      if ( !( is >> endv ) ) {
         continue;
      }
      retval.addEdge( startv, endv, cost );
   }
}

// Currently it is the fastest version, 1.4 secs on the largest input
template < class GraphRepresentant = GraphAL >
bool shamelessReadALGraphFromEdgeListWithoutCost( std::string filename, GraphRepresentant& graph, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   if ( debugmode ) {
      std::cout << "=== READING GRAPH()" << std::endl;
   }

   GraphRepresentant retval;
   unsigned startv = 0;
   int item = 0;

   bool odd = true;
   bool num = true;
   char buf[2048];

   do
   {
      is.read(buf, sizeof(buf));
      int k = is.gcount();
      for (int i = 0; i < k; ++i)
      {
          switch (buf[i])
          {
              case '\r':
              case '\n':
              case ' ':
                  if ( num ) {
                     if ( odd ) {
                        startv = item;
                     } else {
                        retval.addEdge( startv, item, 1 );
                     }
                     odd = !odd;
                     item = 0;
                     num = false;
                  }
                  break;
              case '0': case '1': case '2': case '3':
              case '4': case '5': case '6': case '7':
              case '8': case '9':
                  num = true;
                  item = 10*item + buf[i] - '0';
                  break;
              default:
                  std::cout << "Bad format" << std::endl;
          }    
      }
   } while ( is );

   is.close();
   graph = retval;
   return true;
}

#endif /*GRAPH_ADJACENCY_LIST*/
