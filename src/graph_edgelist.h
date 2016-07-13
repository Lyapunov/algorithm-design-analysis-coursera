#ifndef GRAPH_EDGELIST
#define GRAPH_EDGELIST

#include "edge.h"

struct Graph {
   unsigned n = 0;
   unsigned m = 0;
   std::vector< Edge > edges;
};

std::ostream& operator<<( std::ostream& os, const Graph& graph ) {
   os << "Graph( " << graph.n << ", " << graph.m << std::endl;
   for ( const auto& elem : graph.edges ) {
      os << "--- --- " << elem << std::endl; 
   }
   os << ")";
   return os;
}

bool readGraph( std::string filename, Graph& graph, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   // first line
   std::string line;
   if ( !std::getline( is, line ) ) {
      return false;
   }
   std::istringstream ss(line);

   Graph retval;

   if ( !(ss >> retval.n ) ) {
      return false;
   }
   bool is_m_given = !!(ss >> retval.m );

   if ( debugmode ) {
      if ( is_m_given ) {
         std::cout << "=== READING GRAPH(" << retval.n << ", " << retval.m << ")" << std::endl;
      } else {
         std::cout << "=== READING GRAPH(" << retval.n << ")" << std::endl;
      }
   }
   while (true) {
      if ( !std::getline( is, line ) ) {
         is.close();
         if ( !is_m_given ) {
            retval.m = retval.edges.size();
         }
         bool is_good = retval.edges.size() == retval.m;
         if ( is_good ) {
            graph = retval;
         }
         return is_good;
      }
      std::istringstream ss(line);
      Edge edge;
      ss >> edge.first;
      ss >> edge.second;
      ss >> edge.cost;
      retval.edges.push_back( edge );
   }
}

#endif /*GRAPH_EDGELIST*/
