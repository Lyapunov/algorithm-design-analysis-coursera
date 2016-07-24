#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <map>
#include <set>

#include "graph_adjacency_list.h"

static const int DEBUG_MODE = 0;

std::ostream& operator<<( std::ostream& os, const std::vector<unsigned>& rhs ) {
   os << "vector<unsigned>( ";
   for ( const auto elem: rhs ) {
      os << elem << " ";
   }
   os << ")";
   return os;
}

class DfsLooper {
public:
   DfsLooper( const GraphAL& graph, const std::vector<unsigned>* startings = 0 )
    : graph_( graph ),
      visited_( graph.n, false ),
      ending_counter_( 0 ),
      representant_( 0 ),
      endings_( graph.n, 0 ),
      components_( graph.n, 0 )
   {
      if ( startings ) {
         // reading backwards, amend to Kosaraju's algorithm
         for ( unsigned i = 0; i < startings->size(); ++i ) {
            unsigned elem = startings->operator[]( startings->size() - 1 - i );
            representant_ = elem;
            dfs( elem );
         }
      } else {
         for ( unsigned i = 0; i < graph.n; ++i ) {
            representant_ = i;
            dfs( representant_ );
         }
      }
   }

   std::vector<unsigned> getEndings() { return endings_; }
   std::vector<unsigned> getComponents() { return components_; }

private:
   void dfs( unsigned subject ) {
      if ( visited_[subject] ) {
         return;
      }
      visited_[subject] = true;
      for ( const auto edge : graph_.alist[ subject ] ) {
         dfs( edge.second ); 
      }
      endings_[ending_counter_] = subject;
      components_[ subject ] = representant_;
      ending_counter_++;
   }

   const GraphAL& graph_;
   std::vector<bool> visited_;
   unsigned ending_counter_;
   unsigned representant_; 
   std::vector<unsigned> endings_;
   std::vector<unsigned> components_;
};

std::vector<unsigned> kosaraju( const GraphAL& graph ) {
   const GraphAL inverted = graph.invert();
   DfsLooper looper1( inverted );
   std::vector<unsigned> magic = looper1.getEndings();
   if ( DEBUG_MODE ) {
      std::cout << "--- magic: " << magic << std::endl;
   }
   DfsLooper looper2( graph, &magic );
   return looper2.getComponents();
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
      GraphAL graph;
      std::string filename = argv[1];
      if ( !readALGraphFromEdgeList( filename, graph, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }
      if ( DEBUG_MODE ) {
         std::cout << graph << std::endl;
      }

      std::vector<unsigned> result = kosaraju( graph );

      if ( DEBUG_MODE ) {
         std::cout << "=== Strongly connected compontent descriptor:" << std::endl;
         std::cout << "--- " << result << std::endl;
      }

      std::map<unsigned, unsigned> component_sizes_by_representant;
      for ( const auto& elem : result ) {
         component_sizes_by_representant[ elem ]++; 
      }

      // http://stackoverflow.com/questions/22591645/cstd-map-key-with-descending-order
      std::multiset<unsigned, std::greater<int>> component_sizes;
      for ( const auto& elem : component_sizes_by_representant ) {
         component_sizes.insert( elem.second );
      }
      unsigned counter = 0;
      for ( const auto& elem : component_sizes ) {
         std::cout << elem;

         counter++;
         if ( counter >= 5 ) {
            break;
         }
         if ( counter < component_sizes.size() ) {
            std::cout << ",";
         }
      }
      std::cout << std::endl;
   }
}
