#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <math.h>

#include "graph_euclidian.h"

static constexpr unsigned MAX_SIZE = 30;
static constexpr double INF_VALUE = 10e20;
static constexpr double ERROR = 1e-7;

inline double equals( double a, double b ) {
   return fabs( a - b ) < ERROR;
}

static const int DEBUG_MODE = 1;

std::ostream& operator<<( std::ostream& os, const std::vector<unsigned>& vec ) {
   os << "vector( ";
   for ( const auto& elem : vec ) {
      os << elem << " ";
   }
   os << ")";
   return os;
}

std::ostream& operator<<( std::ostream& os, const std::vector<double>& vec ) {
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

// ----- Solving TSP


double solve_tsp( const EuclidianGraph& egraph ) {
   // distance table
   if ( DEBUG_MODE ) {
      std::cout <<  "--- distances:" <<  std::endl;
   }
   std::vector< std::vector<double>> distances;
   for ( unsigned i = 0; i < egraph.n; ++i ) {
      distances.push_back( std::vector<double>( egraph.n, 0.0 ) );
      for ( unsigned j = 0; j < egraph.n; ++j ) {
         distances[i][j] = distance( egraph.coords[i], egraph.coords[j] );

      }
      if ( DEBUG_MODE ) {
         std::cout <<  distances[i] <<  std::endl;
      }
   }

   std::vector< std::vector<double> > tablets;
   {
      const unsigned permuts = BinomTablet[ egraph.n - 1 ][ 0 ];
      std::vector<double> current( permuts * egraph.n, INF_VALUE );
      assert( permuts == 1 );
      current[0] = 0;

      tablets.push_back( current );
   }

   // main loop
   for ( unsigned m = 1; m < egraph.n; ++m ) {
      std::cout <<  "--- main loop "  << m << std::endl;
      const unsigned permuts = BinomTablet[ egraph.n - 1][ m  ];
      std::cout <<  "--- permuts: "  << permuts << std::endl;
      std::vector<unsigned> curr_permut( m + 1, 0 );
      for ( unsigned i = 0; i < curr_permut.size(); ++i ) {
         curr_permut[i] = i;
      }
      std::vector<unsigned> prev_permut( m, 0 );

      std::vector<double> current( permuts * egraph.n, 0.0 );
      for ( unsigned s = 0; ; ++s ) {
         if ( DEBUG_MODE ) {
            std::cout << "--- " << s << " " << curr_permut << std::endl;
         }

         // init all related values to inf
         for ( unsigned x = 0; x < egraph.n; ++x ) {
            current[ s * egraph.n + x ] = INF_VALUE;
         }
         
         // starting from 1, because node 0 is never dropped
         for ( unsigned j = 1; j < curr_permut.size(); ++j ) {
            // creating prev_permut
            for ( unsigned x = 0; x < curr_permut.size() - 1; ++ x ) {
               prev_permut[x] = x < j ? curr_permut[x] : curr_permut[x + 1];
            }
            unsigned pnumber = permut_number( prev_permut, egraph.n );
            for ( unsigned y = 0; y < egraph.n; ++y ) {
               const double candidate = tablets[ tablets.size() - 1 ][ pnumber * egraph.n + y ] + distances[ y ][ curr_permut[j] ];
               current[ s * egraph.n + curr_permut[j] ] =
                  std::min( current[ s * egraph.n + curr_permut[j] ], candidate );
            }
         }

         //  updating permuts at the end, something like a counter
         unsigned pos = 0;
         while ( curr_permut[ curr_permut.size() - 1 - pos] >= egraph.n - pos - 1 && pos < curr_permut.size() - 1 ) {
            ++pos;
         }
         ++curr_permut[ curr_permut.size() - 1 - pos ];
         for ( unsigned i = 1; i <= pos; ++i ) {
            curr_permut[ curr_permut.size() - 1 - pos + i] = curr_permut[ curr_permut.size() - 1 - pos + i - 1] + 1;
         }
         // loop terminator
         if ( curr_permut[ 0 ] >= 1 ) {
            break;
         }
      }

      if ( DEBUG_MODE ) {
         std::cout << "=== " << current << std::endl;
      }

      tablets.push_back( current );
   }

   // preparing for return
   double retval = INF_VALUE;
   for ( unsigned i = 1; i < egraph.n; ++i ) {
      retval = std::min( retval, tablets[ tablets.size() - 1 ][ i ] + distances[i][0] ); // closing the Hamilton-cycle
   }

   // we spend O( n^2 * 2^n ) time and filled up the memory, so it makes sense to backtrace the solution

   unsigned lastNode = 1;
   for ( unsigned i = 1; i < egraph.n; ++i ) {
      if ( equals( std::min( retval, tablets[ tablets.size() - 1 ][ i ] + distances[i][0] ), retval ) ) {
         lastNode = i;
         break;
      }
   }
   std::cout << lastNode << std::endl;
   // reconstructing the path backwards
//   unsigned pnumber = 0;
   for ( unsigned m = egraph.n - 1; m > 0; --m ) {
      std::vector<unsigned> perm;
//      restore_permut( perm, egraph.n, m
   }


   return retval;
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

      const double solution = solve_tsp( egraph );
      std::cout << solution << std::endl;
   }
   return 0;
}
