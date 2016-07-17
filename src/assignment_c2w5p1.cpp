#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "graph_euclidian.h"

static const int DEBUG_MODE = 1;

std::vector<unsigned>
binomials( unsigned n ) {
   std::vector<unsigned> retval;
   retval.push_back( 1 );
   for ( unsigned i = 0; i < n; ++i ) {
      retval.push_back( 0 );
      for ( unsigned j = retval.size() - 1; j > 0; --j ) {
         retval[j] = retval[j] + retval[j - 1];
      }
   }
   return retval;
}

void solve_tsp( const EuclidianGraph& egraph ) {
}

int main( int argc, const char* argv[] ) {
   // Prints each argument on the command line.
   if ( argc < 1 ) {
      return 1;
   }

   if ( argc != 2 ) {
      std::cout << "USAGE: " << basename( argv[0] ) << " <filename> " << std::endl;
      return 0;
   } else{ 
      EuclidianGraph egraph;
      std::string filename = argv[1];
      if ( !readEuclidianGraph( filename, egraph, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }
      
      if ( DEBUG_MODE ) {
         std::vector<unsigned> bs = binomials(4);
         for ( const auto& elem : bs ) {
            std::cout << elem << " ";
         }
         std::cout << std::endl;
         std::cout << egraph << std::endl;
      }

   }
   return 0;
}
