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

#endif /*_EDGE_*/
