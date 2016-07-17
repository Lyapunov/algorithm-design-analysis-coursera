#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "graph_euclidian.h"

static constexpr unsigned MAX_SIZE = 30;

static const int DEBUG_MODE = 1;

std::ostream& operator<<( std::ostream& os, const std::vector<unsigned>& vec ) {
   os << "vector( ";
   for ( const auto& elem : vec ) {
      os << elem << " ";
   }
   os << ")";
   return os;
}

// ----- Calculating binomial coefficients

std::vector< std::vector< unsigned >> BinomTablet;

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

void
init_binom_tablet() {
   for ( unsigned i = 0; i < MAX_SIZE; ++i ) {
      BinomTablet.push_back( binomials( i ) );
   }
}

// ----- Permutation handling

// vec is ordered in an ascending order, contains a k items of {0, 1 .. (n-1)}, without repetition
unsigned permut_number( const std::vector<unsigned>& vec, unsigned n, unsigned pos = 0, unsigned subject = 0 ) 
{
   if ( pos >= vec.size() || subject + vec.size() - 1 - pos >= n - 1 || subject >= n ) {
      return 0;
   }

   if ( vec[pos] == subject ) {
      return permut_number( vec, n, pos + 1, subject + 1 );
   } else {
      return BinomTablet[n - 1 - subject][vec.size() - pos - 1] + permut_number( vec, n, pos, subject + 1 ); 
   }
}

void restore_permut( std::vector<unsigned>& retval, unsigned n, unsigned k, unsigned number, unsigned pos = 0, unsigned subject = 0 )
{
   if ( subject >= n || pos >= k ) {
      return;
   }

   if ( number < BinomTablet[n - 1 - subject][k - pos - 1] ) {
      retval.push_back( subject );
      restore_permut( retval, n, k, number, pos + 1, subject + 1 );
   } else {
      restore_permut( retval, n, k, number - BinomTablet[n - 1 - subject][k - pos - 1], pos, subject + 1 );
   }
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
      
      init_binom_tablet();

      if ( DEBUG_MODE ) {
         std::cout << "-- Initialize binomial coefficients" <<  std::endl;
         std::cout << egraph << std::endl;
         std::cout << "-- Testing permutation handling." << std::endl;
         std::cout << "-- Test 1, 4 chooses 3" << std::endl;
         std::cout << permut_number( {0,1,2}, 4 ) << std::endl;
         std::cout << permut_number( {0,1,3}, 4 ) << std::endl;
         std::cout << permut_number( {0,2,3}, 4 ) << std::endl;
         std::cout << permut_number( {1,2,3}, 4 ) << std::endl;
         std::cout << "-- Test 2, 6 chooses 3" << std::endl;
         for ( int i = 0; i < 20; ++i ) {
            std::vector<unsigned> test1;
            restore_permut( test1, 6, 3, i );
            std::cout << test1 << std::endl;
         }
      }

   }
   return 0;
}
