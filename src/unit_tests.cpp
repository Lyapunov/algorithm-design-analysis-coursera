#include <iostream>

#include "union_find.h"
#include "my_heap.h"

int main() {
   std::cout << "Testing union - find." << std::endl;
   uf_all_test();

   std::cout << "Testing my heap." << std::endl;
   my_heap_all_test();

   return 0;
}
