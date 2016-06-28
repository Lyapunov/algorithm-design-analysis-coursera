#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <set>
#include <vector>
#include <algorithm>

struct Edge {
   int first = -1;
   int second = 1;
   int cost = -1;

   bool isValid() {
      return first >= 0;
   }
};

struct Graph {
   int n = 0;
   int m = 0;
   std::vector< Edge > edges;
};

std::ostream& operator<<( std::ostream& os, Edge edge ) {
   os << "Edge( " << edge.first << ", " << edge.second << ", " << edge.cost << ")";
   return os;
}

std::ostream& operator<<( std::ostream& os, Graph graph ) {
   os << "Graph( " << graph.n << ", " << graph.m << std::endl;
   for ( const auto& elem : graph.edges ) {
      os << "--- --- " << elem << std::endl; 
   }
   os << ")";
   return os;
}

static std::vector< Edge > slow_prim( const Graph& graph, int debug_mode = 0 ) {
   std::vector< Edge > spanning_tree_edges;
   std::set< int > A;

   // A graph with less than 2 vertices has no spanning tree
   if ( graph.n < 2 ) {
      return spanning_tree_edges;
   }

   // Taking the first edge is somehow special
   Edge firstEdge = graph.edges[0];
   for ( const auto& elem : graph.edges ) {
      if ( elem.cost < firstEdge.cost ) {
         firstEdge = elem;
      }
   }
   if ( debug_mode ) {
      std::cout << "=== The first edge is " << firstEdge << std::endl;
   }
   A.insert( firstEdge.first );
   A.insert( firstEdge.second );
   spanning_tree_edges.push_back( firstEdge );

   while ( A.size() < static_cast<unsigned>( graph.n ) ) {
      // Taking the next edge
      Edge nextEdge;
      for ( const auto& elem : graph.edges ) {
         if ( A.count( elem.first ) + A.count( elem.second ) == 1 
              && ( !nextEdge.isValid() || elem.cost < nextEdge.cost ) ) {
            nextEdge = elem;
         }
      }
      if ( debug_mode ) {
         std::cout << "=== The next edge is " << nextEdge << std::endl;
      }
      A.insert( nextEdge.first );
      A.insert( nextEdge.second );
      spanning_tree_edges.push_back( nextEdge );
   }

   return spanning_tree_edges;
}

static bool readInput( std::string filename, Graph& graph, int debugmode = 0 )
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

   ss >> retval.n;
   ss >> retval.m;

   if ( debugmode ) {
      std::cout << "=== READING GRAPH(" << retval.n << ", " << retval.m << ")" << std::endl;
   }
   for ( int i = 0; i < retval.m; ++i ) {
      if ( !std::getline( is, line ) ) {
         return false;
      }
      std::istringstream ss(line);
      Edge edge;
      ss >> edge.first;
      ss >> edge.second;
      ss >> edge.cost;
      retval.edges.push_back( edge );
   }
   is.close();
   graph = retval;

   return true;
}

static const int DEBUG_MODE = 0;

int main( int argc, const char* argv[] ) {
   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc != 2 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename>" << std::endl;
      return 0;
   } else{ 
      std::string filename = argv[1];
      Graph graph;
      if ( !readInput( argv[1], graph, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }
      if ( DEBUG_MODE ) {
         std::cout << graph << std::endl; 
      }
      std::vector< Edge > spanning_tree_edges = slow_prim( graph, DEBUG_MODE );
      long total_cost = 0;
      for ( const auto& elem : spanning_tree_edges ) {
         total_cost += elem.cost; 
      }
      // the result
      std::cout << total_cost << std::endl;
   }
}
