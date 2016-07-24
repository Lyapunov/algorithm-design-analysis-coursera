#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include "kosaraju.h"

static const int DEBUG_MODE = 1;

struct Clause {
   int first  = 0;
   int second = 0;
};

void printLiteral( std::ostream& os, int literal ) {
   if ( literal < 0 ) {
      os << "neg ";
   }
   os << "x_" << ( literal > 0 ? literal : -literal );
}

std::ostream& operator<<( std::ostream& os, const Clause& clause ) {
   os << "(";
   printLiteral( os, clause.first );
   os << " OR ";
   printLiteral( os, clause.second );
   os << ")";
   return os;
}

struct Sat2 {
   unsigned n = 0;
   std::vector< Clause > clist;
};

std::ostream& operator<<( std::ostream& os, const Sat2& sat2 ) {
   os << "Sat2 (" << sat2.n << "):" << std::endl;
   os << "   ";
   for ( unsigned i = 0; i < sat2.clist.size(); ++i ) {
      os << sat2.clist[i]; 
      if ( i < sat2.clist.size() - 1 ) {
        os << " AND ";
      }
   }
   return os;
}

bool readSat2Problem( std::string filename, Sat2& sat2, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   std::string line;
   if ( !std::getline( is, line ) ) {
      return false;
   }
   std::istringstream ss(line);

   Sat2 retval;

   if ( !(ss >> retval.n ) ) {
      return false;
   }

   if ( debugmode ) {
      std::cout << "=== READING SAT2(" << retval.n << " literals)" << std::endl;
   }

   while (true) {
      if ( !std::getline( is, line ) ) {
         is.close();
         sat2 = retval;
         return true;
      }
      std::istringstream ss(line);
      Clause current;
      if ( !(ss >> current.first ) ) {
         return false;
      }
      if ( !(ss >> current.second ) ) {
         return false;
      }

      retval.clist.push_back( current );
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
      Sat2 problem;
      std::string filename = argv[1];
      if ( !readSat2Problem( filename, problem, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }
      if ( DEBUG_MODE ) {
         std::cout << problem << std::endl;
      }

   }
}
