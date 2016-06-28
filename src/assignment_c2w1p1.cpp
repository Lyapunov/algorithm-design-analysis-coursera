#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <algorithm>

static const int DEBUG_MODE = 1;

struct Stuff {
   int weight;
   int length;
};

static bool readInput( std::string filename, std::vector<Stuff>& stuffs )
{
   std::ifstream is;
   is.open( filename.c_str() );

   // first line
   std::string line;
   if ( !std::getline( is, line ) ) {
      return false;
   }
   std::istringstream ss(line);
   int numOfRecords = 0;
   ss >> numOfRecords;

   if ( DEBUG_MODE ) {
      std::cout << "=== READING " << numOfRecords << " RECORDS" << std::endl;
   }
   for ( int i = 0; i < numOfRecords; ++i ) {
      if ( !std::getline( is, line ) ) {
         return false;
      }
      std::istringstream ss(line);
      Stuff stuff;
      ss >> stuff.weight;
      ss >> stuff.length;
      stuffs.push_back( stuff );
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
      std::vector< Stuff > stuffs;
      if ( !readInput( argv[1], stuffs ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      std::sort(stuffs.begin(), stuffs.end(), [](Stuff a, Stuff b) {
         return (b.weight - b.length) == (a.weight - a.length)
                ? b.weight < a.weight 
                : (b.weight - b.length) < (a.weight - a.length);
      });

      if ( DEBUG_MODE ) {
         std::cout << "=== PRINTING SORTED DATA" << std::endl;
      }

      long completition = 0;
      long cost = 0;
      for ( const auto& elem : stuffs ) {
         completition += elem.length;
         cost += completition * elem.weight;
         if ( DEBUG_MODE ) {
            std::cout << "--- --- " << elem.weight << " " << elem.length << "; completition : " << completition << "; totalcost: " << cost << std::endl; 
         }
      }
      std::cout << cost << std::endl;
   }
}
