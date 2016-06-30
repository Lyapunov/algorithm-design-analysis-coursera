#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <set>
#include <vector>
#include <algorithm>
#include <bitset>

#include "union_find.h"

static const int DEBUG_MODE = 0;
static constexpr int MAX_BITS = 30;

struct InterestingPosition
{
   InterestingPosition( unsigned des, unsigned par ) : descriptor( des ), parent( par ) {}

   unsigned descriptor;
   unsigned parent;
};

static bool readInput( std::string filename, unsigned& bits, std::vector<unsigned>& vertices, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   // first line
   std::string line;
   if ( !std::getline( is, line ) ) {
      return false;
   }
   std::istringstream ss(line);
   int numOfNodes = 0;
   ss >> numOfNodes;
   ss >> bits;
   assert ( bits >= 1 );
   assert ( bits <= MAX_BITS );

   if ( debugmode ) {
      std::cout << "=== READING " << numOfNodes << " NODES" << std::endl;
   }
   for ( int i = 0; i < numOfNodes; ++i ) {
      if ( !std::getline( is, line ) ) {
         return false;
      }
      std::istringstream ss(line);
      unsigned result = 0;
      for ( unsigned j = 0; j < bits; ++j ) {
         unsigned bit;
         ss >> bit;
         result = ( result << 1 ) + bit;
      }
      vertices.push_back( result );
   }
   is.close();

   return true;
}

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
      
      // basic structures
      unsigned bits;
      std::vector<unsigned> vertices;
      if ( !readInput( argv[1], bits, vertices, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      if ( DEBUG_MODE ) {
         for ( const auto& elem : vertices ) {
            // http://stackoverflow.com/questions/7349689/c-how-to-print-using-cout-the-way-a-number-is-stored-in-memory
            std::bitset<MAX_BITS> outelem( elem );
            std::cout << outelem << std::endl;
         }
      }
      
      // union find:
      UnionFind uf( vertices.size() );

      // wunderwaffe
      std::vector<InterestingPosition> placesOfInterest;
      placesOfInterest.reserve( 2 * bits * vertices.size() );
      for ( unsigned i = 0; i < vertices.size(); ++i ) {
         placesOfInterest.push_back( InterestingPosition( vertices[i], i ) );
         unsigned mask = 1;
         for ( unsigned j = 0; j < bits; ++j ) {
            placesOfInterest.push_back( InterestingPosition( vertices[i] ^ mask, i ) );
            mask = ( mask << 1 );
         }
      }

      std::sort(placesOfInterest.begin(), placesOfInterest.end(), [](InterestingPosition a, InterestingPosition b) {
         return ( a.descriptor < b.descriptor );
      });

      // greedy algorithm that elliminates all edges which have the distance 1 or 2
      for ( unsigned i = 0; i < placesOfInterest.size() - 1; ++i ) {
         if ( placesOfInterest[i].descriptor == placesOfInterest[i+1].descriptor ) {
            uf.doUnion( placesOfInterest[i].parent, placesOfInterest[i+1].parent );
         }
      }
      
      std::cout << uf.getNumberOfClusters() << std::endl;
   }
}
