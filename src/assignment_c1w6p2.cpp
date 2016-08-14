#include <string>
#include <string.h>

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>
#include <set>

#include "fast_integer_file_reader.h"

static const int DEBUG_MODE = 0;

// poor man uses set instead of heap
template <class Type, class Comparator = std::less<Type>>
class SimpleHeap {
public:
   void push( Type item ) {
      data.insert( item );
   }
   Type pop() {
      Type retval = *data.begin();
      data.erase( data.begin() );
      return retval;
   }
   Type head() const {
      return *data.begin();
   }
   bool empty() const {
      return data.empty();
   }
   Type size() const {
      return data.size();
   }
   void debugPrint( std::ostream& os ) const {
      os << "[ ";
      bool first = true;
      for ( const auto& elem : data ) {
         if ( first ) {
            os << elem << "|";
            first = false;
         } else {
            os << elem << " ";
         }
      }
      os << "]";
   }
private:
   std::set<Type, Comparator> data;
};

template <class Type, class Comparator >
std::ostream& operator<<( std::ostream& os, const SimpleHeap<Type, Comparator>& heap ) {
   os << "heap";
   heap.debugPrint( os );
   return os;
}

template <class Type>
class MedianMaintainer {
public:
   Type getMedian() const {
      if ( smallerItems.size() == greaterItems.size()
           || smallerItems.size() > greaterItems.size() ) {
         return smallerItems.head(); 
      }
      return greaterItems.head(); 
   }
   void push( Type item ) {
      if ( greaterItems.empty() || item < greaterItems.head() ) {
         smallerItems.push( item );
      } else {
         greaterItems.push( item );
      }
      rebalance();    
   }
   void rebalance() {
      while ( smallerItems.size() > greaterItems.size() + 1 ) {
         greaterItems.push( smallerItems.pop() );
      }
      while ( greaterItems.size() > smallerItems.size() + 1 ) {
         smallerItems.push( greaterItems.pop() );
      }
   }
   void debugPrint( std::ostream& os ) const {
      os << smallerItems;
      os << " ";
      os << getMedian();
      os << " ";
      os << greaterItems;
   }
private:
   SimpleHeap<Type, std::greater<Type>> smallerItems;
   SimpleHeap<Type, std::less<Type>>    greaterItems;
};

template <class Type>
std::ostream& operator<<( std::ostream& os, const MedianMaintainer<Type>& maint ) {
   os << "median_maint(";
   maint.debugPrint(os);
   os << " )";
   return os;
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
      std::vector<long> input;
      input.reserve(20000);
      std::string filename = argv[1];
      if ( !fastIntegerFileReader( filename, input, DEBUG_MODE ) ) {
         std::cerr << "ERROR during attempting to read file " << filename << std::endl;
         return 1;
      }

      if ( DEBUG_MODE ) {
         std::cout << input << std::endl;
         std::cout << input.size() << std::endl;
      }

      MedianMaintainer<long> mm;
      long answer = 0;
      for ( const auto& elem : input ) {
         mm.push( elem );
         if ( DEBUG_MODE ) {
            std::cout << mm << std::endl;
         }
         answer = ( answer + mm.getMedian() ) % 10000;
      }
      std::cout << answer << std::endl;
   }
}
