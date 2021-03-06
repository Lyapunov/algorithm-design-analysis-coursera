#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>
#include <cassert>
#include <math.h>
#include <map>
#include <algorithm>

#include "graph_euclidian.h"

static constexpr unsigned MAX_SIZE = 30;
static constexpr EuclidianFloat INF_VALUE = 10e7;
static constexpr EuclidianFloat ERROR = 1e-5;

inline EuclidianFloat equals( EuclidianFloat a, EuclidianFloat b ) {
   return fabs( a - b ) < ERROR;
}

static const int DEBUG_MODE = 0;

std::ostream& operator<<( std::ostream& os, const std::vector<unsigned>& vec ) {
   os << "[ ";
   for ( const auto& elem : vec ) {
      os << elem << " ";
   }
   os << "]";
   return os;
}

std::ostream& operator<<( std::ostream& os, const std::vector<EuclidianFloat>& vec ) {
   os << "[ ";
   for ( const auto& elem : vec ) {
      os << elem << " ";
   }
   os << "]";
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

unsigned permut_number_with_skipping( const std::vector<unsigned>& vec, unsigned n, unsigned skipped_pos = 0, unsigned pos = 0, unsigned subject = 0 ) 
{
   if ( pos == skipped_pos ) {
      return permut_number_with_skipping( vec, n, skipped_pos, pos + 1, subject );
   }

   const bool skipping = static_cast<bool>( skipped_pos > pos );
   if ( pos >= vec.size() || subject + vec.size() - 1 - pos - skipping >= n - 1 || subject >= n ) {
      return 0;
   }

   if ( vec[pos] == subject ) {
      return permut_number_with_skipping( vec, n, skipped_pos, pos + 1, subject + 1 );
   } else {
      return BinomTablet[n - 1 - subject][vec.size() - pos - 1 - skipping] + permut_number_with_skipping( vec, n, skipped_pos, pos, subject + 1 ); 
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

EuclidianFloat solve_tsp( const EuclidianGraph& egraph, std::vector<unsigned>& best_travel ) {
   // distance table
   if ( DEBUG_MODE ) {
      std::cout <<  "--- distances:" <<  std::endl;
   }
   std::vector< std::vector<EuclidianFloat>> distances = egraph.get_distance_table();

   std::vector< std::vector<EuclidianFloat> > tablets;
   {
      const unsigned permuts = BinomTablet[ egraph.n - 1 ][ 0 ];
      std::vector<EuclidianFloat> current( permuts * egraph.n, INF_VALUE );
      assert( permuts == 1 );
      current[0] = 0;

      tablets.push_back( current );
   }

   // main loop
   for ( unsigned m = 1; m < egraph.n; ++m ) {
      const unsigned permuts = BinomTablet[ egraph.n - 1][ m  ];
      if ( DEBUG_MODE ) {
         std::cout <<  "=== === main loop "  << m << std::endl;
         std::cout <<  "--- permuts: "  << permuts << std::endl;
      }
      std::vector<unsigned> curr_permut( m + 1, 0 );
      for ( unsigned i = 0; i < curr_permut.size(); ++i ) {
         curr_permut[i] = i;
      }

      std::vector<EuclidianFloat> current( permuts * egraph.n, 0.0 );
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
            unsigned previous_s = permut_number_with_skipping( curr_permut, egraph.n, j );
            for ( unsigned y = 0; y < egraph.n; ++y ) {
               const EuclidianFloat candidate = tablets[ tablets.size() - 1 ][ previous_s * egraph.n + y ] + distances[  curr_permut[j] ][ y ];
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
   EuclidianFloat retval = INF_VALUE;
   for ( unsigned i = 1; i < egraph.n; ++i ) {
      retval = std::min( retval, tablets[ tablets.size() - 1 ][ i ] + distances[i][0] ); // closing the Hamilton-cycle
   }

   // we spent O( n^2 * 2^n ) time and filled up the memory, backtracing the solution won't hurt

   // reconstructing the path backwards
   std::vector<unsigned> best_path;
   {
      if ( DEBUG_MODE ) {
         std::cout <<  "=== === reconstructing optimal path " << std::endl;
      }

      unsigned last_node = 1;
      for ( unsigned i = 1; i < egraph.n; ++i ) {
         if ( equals( tablets[ tablets.size() - 1 ][ i ] + distances[i][0], retval ) ) {
            last_node = i;
            break;
         }
      }

      if ( DEBUG_MODE ) {
         std::cout << "Adding node " << last_node << std::endl;
      }
      best_path.push_back( last_node );

      unsigned s = 0; // at the last round, there is only 1 set, which is numbered by 0
      for ( unsigned m = egraph.n - 1; m > 0; --m ) {
         std::vector<unsigned> curr_permut;
         restore_permut( curr_permut, egraph.n, m + 1, s ); // restoring the set
         const EuclidianFloat min_value = tablets[m][ s * egraph.n + last_node ];
         if ( DEBUG_MODE ) {
            std::cout << "--- " << curr_permut << " " << last_node << " " << min_value << std::endl;
         }
         bool found = false;
         unsigned next_s = 0;
         unsigned next_last_node = 0;

         std::vector<unsigned> prev_permut( m, 0 );
         for ( unsigned j = 1; j < curr_permut.size(); ++j ) {
            if ( curr_permut[j] != last_node ) {
               continue;
            }
            // creating prev_permut
            for ( unsigned x = 0; x < curr_permut.size() - 1; ++ x ) {
               prev_permut[x] = x < j ? curr_permut[x] : curr_permut[x + 1];
            }
            unsigned pnumber = permut_number( prev_permut, egraph.n );
            if ( DEBUG_MODE ) {
               std::cout << "=== " << pnumber << std::endl;
            }
            for ( unsigned y = 0; y < egraph.n; ++y ) {
               const EuclidianFloat candidate = tablets[ m - 1 ][ pnumber * egraph.n + y ] + distances[ y ][ curr_permut[j] ];
               if ( DEBUG_MODE ) {
                  std::cout << "=== === " << candidate << std::endl;
               }
               if ( equals( min_value, candidate ) ) {
                  found = true;
                  next_s = pnumber;
                  next_last_node = y;
               }
            }
         }

         assert(found);
         last_node = next_last_node;
         s = next_s;
         if ( DEBUG_MODE ) {
            std::cout << "Adding node " << last_node << std::endl;
         }
         best_path.push_back( last_node );
      }

      if ( DEBUG_MODE ) {
         std::cout << "Reconstructed best travel is " << best_path << std::endl;
      }
   }

   best_travel = best_path;
   return retval;
}

// --- Trying to speed up TSP solver with heuristics
struct EuclidianTspReduction {
   std::map<unsigned, unsigned> reductions; 
   std::map<unsigned, unsigned> inv_reductions; 
   EuclidianGraph rgraph; // reduced graph 
   std::map<unsigned, int> helper;
};

EuclidianTspReduction reduceEuclidianTspProblem( const EuclidianGraph& egraph ) {
   EuclidianTspReduction retval;

   for ( unsigned i = 0; i < egraph.n; ++i ) {

      // finding nearest neighbour
      EuclidianFloat nearest = ( i == 0 ) ? 1 : 0;
      for ( unsigned x = 0; x < egraph.n; ++x ) {
         if ( x == i ) {
            continue;
         }
         if ( distance( egraph.coords[i], egraph.coords[x] ) < distance( egraph.coords[i], egraph.coords[nearest] ) ) {
            nearest = x;
         }
      }

      EuclidianFloat minDist = distance( egraph.coords[i], egraph.coords[nearest] );
      EuclidianFloat minImprovement = INF_VALUE; 

      for ( unsigned x = 0; x < egraph.n; ++x ) {
         for ( unsigned y = x + 1; y < egraph.n; ++y ) {
            if ( x == i || y == i || x == nearest || y == nearest ) {
               continue;
            }
            minImprovement = std::min( minImprovement, distance( egraph.coords[x], egraph.coords[i] )
                                                       + distance( egraph.coords[i], egraph.coords[y] )
                                                       - distance( egraph.coords[x], egraph.coords[y] ) );
         }
      }
      if ( DEBUG_MODE ) {
          std::cout << "--- Point " << i << " : " << minDist << " " << minImprovement << std::endl;
      }

      if ( 2 * minDist < minImprovement && retval.reductions.find( nearest ) == retval.reductions.end() ) {
         if ( DEBUG_MODE ) {
            std::cout << "--- Point " << i << " can be reduced with " << nearest << std::endl;
         }
         retval.reductions[ i ] = nearest;
         retval.inv_reductions[ nearest ] = i;
      }
   }

   // filling the reduced graph only if there is a reduction
   if ( !retval.reductions.empty() ) {
      // reducing and filling a helper
      retval.rgraph.n = egraph.n - retval.reductions.size();
      for ( unsigned i = 0; i < egraph.n; ++i ) {
         if ( retval.reductions.find( i ) != retval.reductions.end() ) {
            retval.helper[ i ] = -1;
         } else {
            retval.rgraph.coords.push_back( egraph.coords[i] );
            retval.helper[ retval.rgraph.coords.size() - 1 ] = i;
         }
      }
   }

   return retval;
}

EuclidianFloat solve_tsp_tricky( const EuclidianGraph& egraph, std::vector<unsigned>& best_travel ) {
   // can be reduced?
   EuclidianTspReduction reduced = reduceEuclidianTspProblem( egraph );

   if ( reduced.reductions.empty() ) {
      // the easy case
      return solve_tsp( egraph, best_travel );
   } else {

      // using the solver as an internal step
      std::vector<unsigned> reduced_travel;
      solve_tsp_tricky( reduced.rgraph, reduced_travel );

      // creating candidates based on the solution
      for ( unsigned i = 0; i < reduced_travel.size(); ++i ) {
         reduced_travel[i] = reduced.helper[ reduced_travel[i] ];
      }

      std::vector< std::vector<unsigned> > best_travels;
      best_travels.push_back( reduced_travel );
      for ( const auto& elem : reduced.inv_reductions ) {
         unsigned num = best_travels.size();
         for ( unsigned i = 0; i < num; ++i ) {
            best_travels.push_back( best_travels[i] );
            auto& bt = best_travels[i];
            auto& bt2 = best_travels[best_travels.size() - 1];

            const auto it = std::find( bt.begin(), bt.end(), elem.first );
            assert( it != best_travels[i].end() );
            bt.insert( it, elem.second );

            const auto it2 = std::find( bt2.begin(), bt2.end(), elem.first );
            assert( it2 != bt2.end() );
            bt2.insert( it2 + 1, elem.second );
         }
      }

      std::vector< std::vector<EuclidianFloat>> distances = egraph.get_distance_table();

      unsigned mini = 0;
      EuclidianFloat minlen = calculate_path_distance( best_travels[mini], distances );

      unsigned num = best_travels.size();
      for ( unsigned i = 0; i < num; ++i ) {
         const EuclidianFloat len = calculate_path_distance( best_travels[i], distances );
         if ( DEBUG_MODE ) {
            std::cout << len << " " << best_travels[i] << std::endl;
         }
         if ( len < minlen ) {
            minlen = len;
            mini   = i;
         }
      }

      best_travel = best_travels[mini];      
      return minlen;
   }

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
         std::cout << "-- Test 3, skipping" << std::endl;
         std::vector<unsigned> test1, test2, test3, test4, test5, test6;
         restore_permut( test1, 8, 3, permut_number_with_skipping( {0,1,3,7}, 8, 1 ) );
         std::cout << test1 << std::endl;
         restore_permut( test2, 8, 3, permut_number_with_skipping( {0,1,3,7}, 8, 2 ) );
         std::cout << test2 << std::endl;
         restore_permut( test3, 8, 3, permut_number_with_skipping( {0,1,3,7}, 8, 3 ) );
         std::cout << test3 << std::endl;
         restore_permut( test4, 8, 3, permut_number_with_skipping( {0,5,6,7}, 8, 1 ) );
         std::cout << test4 << std::endl;
         restore_permut( test5, 8, 3, permut_number_with_skipping( {0,5,6,7}, 8, 2 ) );
         std::cout << test5 << std::endl;
         restore_permut( test6, 8, 3, permut_number_with_skipping( {0,5,6,7}, 8, 3 ) );
         std::cout << test6 << std::endl;
      }

      std::vector<unsigned> best_travel;
      const EuclidianFloat solution = solve_tsp_tricky( egraph, best_travel );
      std::cout << solution << std::endl;
      std::cout << best_travel << std::endl;
   }
   return 0;
}
