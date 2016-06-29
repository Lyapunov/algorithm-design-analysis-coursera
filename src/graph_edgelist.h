#ifndef GRAPH_EDGELIST
#define GRAPH_EDGELIST

struct Edge {
   int first = -1;
   int second = 1;
   int cost = -1;

   bool isValid() {
      return first >= 0;
   }
};

struct Graph {
   int n = 0;
   int m = 0;
   std::vector< Edge > edges;
};

std::ostream& operator<<( std::ostream& os, Edge edge ) {
   os << "Edge( " << edge.first << ", " << edge.second << ", " << edge.cost << ")";
   return os;
}

std::ostream& operator<<( std::ostream& os, Graph graph ) {
   os << "Graph( " << graph.n << ", " << graph.m << std::endl;
   for ( const auto& elem : graph.edges ) {
      os << "--- --- " << elem << std::endl; 
   }
   os << ")";
   return os;
}

static bool readGraph( std::string filename, Graph& graph, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   // first line
   std::string line;
   if ( !std::getline( is, line ) ) {
      return false;
   }
   std::istringstream ss(line);

   Graph retval;

   ss >> retval.n;
   ss >> retval.m;

   if ( debugmode ) {
      std::cout << "=== READING GRAPH(" << retval.n << ", " << retval.m << ")" << std::endl;
   }
   for ( int i = 0; i < retval.m; ++i ) {
      if ( !std::getline( is, line ) ) {
         return false;
      }
      std::istringstream ss(line);
      Edge edge;
      ss >> edge.first;
      ss >> edge.second;
      ss >> edge.cost;
      retval.edges.push_back( edge );
   }
   is.close();
   graph = retval;

   return true;
}

#endif /*GRAPH_EDGELIST*/
