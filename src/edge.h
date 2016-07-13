#ifndef _EDGE_
#define _EDGE_

struct Edge {
   int first = -1;
   int second = 1;
   int cost = -1;

   bool isValid() {
      return first >= 0;
   }
};

std::ostream& operator<<( std::ostream& os, const Edge& edge ) {
   os << "Edge( " << edge.first << ", " << edge.second << ", " << edge.cost << ")";
   return os;
}

#endif /*_EDGE_*/
