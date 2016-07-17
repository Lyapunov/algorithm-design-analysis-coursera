#ifndef GRAPH_EDGELIST
#define GRAPH_EDGELIST

#include <utility>
#include <vector>

struct EuclidianGraph {
   unsigned n = 0;
   std::vector< std::pair< double, double> > coords;
};

std::ostream& operator<<( std::ostream& os, const EuclidianGraph& graph ) {
   os << "EuclidianGraph( " << graph.n << std::endl;
   for ( const auto& elem : graph.coords ) {
      os << "--- --- " << elem.first << " " << elem.second <<  std::endl; 
   }
   os << ")";
   return os;
}

bool readEuclidianGraph( std::string filename, EuclidianGraph& graph, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   // first line
   std::string line;
   if ( !std::getline( is, line ) ) {
      return false;
   }
   std::istringstream ss(line);

   EuclidianGraph retval;

   if ( !(ss >> retval.n ) ) {
      return false;
   }

   if ( debugmode ) {
      std::cout << "=== READING GRAPH(" << retval.n << ")" << std::endl;
   }

   for ( unsigned i = 0; i < retval.n; ++i ) {
      if ( !std::getline( is, line ) ) {
         is.close();
         return false;
      }
      std::istringstream ss(line);
      std::pair<double, double> coord;
      ss >> coord.first;
      ss >> coord.second;
      retval.coords.push_back( coord );
   }
   graph = retval;
   return true;
}

#endif /*GRAPH_EDGELIST*/
