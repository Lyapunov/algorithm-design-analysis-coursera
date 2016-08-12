#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <climits>

#include "graph_adjacency_list.h"
#include "union_find.h"

static const int DEBUG_MODE = 0;

std::vector<unsigned> random_permut( unsigned size ) {
   std::vector<unsigned> shuffle( size, 0 ); 
   for ( unsigned i = 0; i < size; ++i ) {
      shuffle[i] = i;
   }
   for ( unsigned i = 0; i < size - 1; ++i ) {
      unsigned j = i + ( rand() % ( size - i ) ); 
      unsigned tmp = shuffle[j];
      shuffle[j] = shuffle[i];
      shuffle[i] = tmp;
   }
   return shuffle;
}

std::ostream& operator<<( std::ostream& os, const std::vector<unsigned>& rhs ) {
   os << "vector(";
   for ( const auto& elem : rhs ) {
      os << " " << elem;
   }
   os << " )";
   return os;
}

unsigned get_size_of_one_random_cut( const Graph& graph ) {
   std::vector<unsigned> shuffle = random_permut( graph.m );
   UnionFind uf( graph.n );
   unsigned i = 0;
   while ( uf.getNumberOfClusters() > 2 && i < graph.m ) {
      const Edge& edge = graph.edges[ shuffle[i] ];
      uf.doUnion( edge.first, edge.second );
      ++i;
   }
   if ( uf.getNumberOfClusters() != 2 ) {
      // maybe the graph is not connected?
      return UINT_MAX;
   }
   
   unsigned cut_size = 0;
   for ( unsigned j = 0; j < graph.m; ++j ) {
      const Edge& edge = graph.edges[ j ];
      if ( uf.findLeader( edge.first ) != uf.findLeader( edge.second ) ) {
         ++cut_size;
      }
   }

   return cut_size;
}

int main( int argc, const char* argv[] ) {
   srand(time(NULL));

   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc != 2 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename> " << std::endl;
      return 0;
   } else{ 
      GraphAL graph_al;
      std::string filename = argv[1];
      if ( !readALGraphWithoutCosts( filename, graph_al, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }
      Graph graph = graph_al.undirectedEdgeList();
      if ( DEBUG_MODE ) {
         std::cout << graph_al << std::endl;
         std::cout << graph << std::endl;
      }

      unsigned cut_size = UINT_MAX;

      // note: not necesserily enough if the graph has very few cuts.
      //       definitely enough for the assignment's problem
      for ( long attempts = 0; attempts < graph.n * graph.n ; ++attempts ) {
         cut_size = std::min( cut_size, get_size_of_one_random_cut( graph ) );
      }
      std::cout << cut_size << std::endl;
   }

}
