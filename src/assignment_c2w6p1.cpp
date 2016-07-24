#include <string>
#include <string.h>

#include <iostream>
#include <fstream>
#include <sstream>

#include <cassert>

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

unsigned convLiteralNumberToVertexNumber( int literal, unsigned n ) {
   assert( literal );
   if ( literal > 0 ) {
      return static_cast<unsigned>( literal - 1 );  
   } else {
      return static_cast<unsigned>( n - 1 - literal );
   }
}

Graph convertSat2ToGraph( const Sat2& sat2 ) {
   // converting 1..n literals to a graph with vertices 0..(n-1) (positive), n..2n-1 (negative)
   Graph retval;
   retval.n = 2 * sat2.n;
   for ( const auto& elem : sat2.clist ) {
      // if the clause is x_a OR x_b, it boils down to two implications:
      // ( neg x_a => x_b ) and ( neg x_b => x_a )
      Edge a;
      a.first  = convLiteralNumberToVertexNumber( -elem.first, sat2.n ) + 1;
      a.second = convLiteralNumberToVertexNumber(  elem.second, sat2.n ) + 1;
      a.cost = 1;
      retval.edges.push_back( a );
      Edge b;
      b.first  = convLiteralNumberToVertexNumber( -elem.second, sat2.n ) + 1;
      b.second = convLiteralNumberToVertexNumber(  elem.first, sat2.n ) + 1;
      b.cost = 1;
      retval.edges.push_back( b );
   }
   retval.m = retval.edges.size();

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
      Sat2 problem;
      std::string filename = argv[1];
      if ( !readSat2Problem( filename, problem, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }
      if ( DEBUG_MODE ) {
         std::cout << problem << std::endl;
      }

      GraphAL imp = constructGraphALFromGraph( convertSat2ToGraph( problem ) );
      if ( DEBUG_MODE ) {
         std::cout << imp << std::endl;
      }

      std::vector<unsigned> components = kosaraju( imp );
      if ( DEBUG_MODE ) {
         std::cout << components << std::endl;
      }

      // not satisfiable iff a variable's vertex and the vertex of its negation are
      // in the same component
      for ( unsigned i = 0; i < problem.n; ++i ) {
         if ( components[i] == components[ problem.n + i] ) {
            std::cout << "0" << std::endl;
            return 1;
         }
      }
      std::cout << "1" << std::endl;
      return 1;
   }
}
