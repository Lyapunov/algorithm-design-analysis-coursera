#ifndef MY_HEAP
#define MY_HEAP

#include <vector>
#include <map>
#include <algorithm>
#include <initializer_list>

#include <ostream>
#include <sstream>
#include <cassert>


template < class Key, class Value >
class MyHeap
{
public:
   MyHeap() {}
   MyHeap( size_t n ) : content_(), keys_( n, -1 ) { content_.reserve( n ); }

   bool empty() { return !content_.size(); }

   std::pair< Key, Value > pop() {
      if ( empty() ) {
         return std::pair<Key, Value>(-1, -1); // empty
      }
      return remove_internal( 0 ); 
   }

   void insert( const Key& key, const Value& value ) {
      if ( keys_[ key ] >= 0 ) {
         return;
      }
      content_.push_back( std::make_pair( key, value ) );
      const unsigned position = content_.size() - 1;
      keys_[ key ] = position;
      bubble_up( position );
   }

   std::pair<Key, Value> remove( const Key& key ) {
      if ( keys_[ key ] == -1 ) {
         return std::pair<Key, Value>(-1, -1); // empty
      }
      return remove_internal( keys_[ key ] ); 
   }

   const void updateIfExistsAndLess( const Key& key, const Value& value ) {
      int position = keys_[ key ];
      if ( position >= 0 ) {
         if ( content_[ position ].second > value ) {
            content_[ position ].second = value;
            bubble_up( position );
         }
      }
   }

   void debugPrint( std::ostream& os ) const {
      os << "MyHeap( ";
      for ( const auto& elem : content_ ) {
         os << " " << elem.second << "[" << elem.first << "]";
      }
      os << " )";
   }

private:
   std::pair< Key, Value > remove_internal( unsigned position ) {
      const unsigned last_position = content_.size() - 1;
      swap_positions( position, last_position ); 

      const auto retval = content_[ last_position ];
      content_.pop_back();
      keys_[ retval.first ] = -1;

      bubble_down( position );
      return retval;
   }

   void bubble_up( unsigned position ) {
      if ( position == 0 ) {
         return;
      }

      unsigned parent_position = ( position - 1 ) >> 1; // parent position

      if ( content_[position].second < content_[parent_position].second ) {
         swap_positions( position, parent_position ); 
         bubble_up( parent_position );
      }
   }

   void bubble_down( unsigned parent_position ) {
      unsigned candidate  = ( parent_position << 1 ) + 1; // left child

      if ( candidate >= content_.size() ) {
         return;
      }

      const auto* cont = &content_[ candidate ];

      if ( candidate + 1 < content_.size() ) {
         if ( ( cont + 1 )->second < cont->second ) {
            ++cont;
            ++candidate;
         }
      }

      if ( cont->second < content_[ parent_position ].second ) {
         swap_positions( candidate, parent_position ); 
         bubble_down( candidate );
      }
   }

   inline void swap_positions( unsigned position1, unsigned position2 ) {
      std::swap( content_[position1], content_[position2] );

      keys_[ content_[position1].first ] = position1;
      keys_[ content_[position2].first ] = position2;
   }

   std::vector< std::pair< Key, Value >> content_;
   std::vector< Key >  keys_;
};

template < class Key, class Value >
std::ostream& operator<<( std::ostream& os, const MyHeap<Key, Value>& mh ) {
   mh.debugPrint( os );
   return os;
}

static bool my_heap_test_1( bool printToStd = false) {
   std::string result = "MyHeap(  );MyHeap(  1[111] 5[99] 20[777] 21[88] 18[333] );MyHeap(  1[111] 3[666] 2[555] 5[99] 18[333] 20[777] 17[444] 21[88] 7[888] );Pop: 1[111];Pop: 2[555];Pop: 3[666];Pop: 5[99];Pop: 7[888];MyHeap(  17[444] 18[333] 20[777] 21[88] );Pop: 17[444];Pop: 18[333];Pop: 20[777];Pop: 21[88];Pop: -1[-1];";

   std::stringstream ss;
   MyHeap< int, int > myHeap(1000);
   ss << myHeap << ";";
   myHeap.insert( 88, 21 );
   myHeap.insert( 99, 5 );
   myHeap.insert( 777, 20 );
   myHeap.insert( 111, 1 );
   myHeap.insert( 333, 18 );
   ss << myHeap << ";";
   myHeap.insert( 444, 17 );
   myHeap.insert( 555, 2 );
   myHeap.insert( 888, 7 );
   myHeap.insert( 666, 3 );
   ss << myHeap << ";";

   for (int i = 0; i < 5; ++i ) {
      const auto& elem = myHeap.pop();
      ss << "Pop: " << elem.second << "[" << elem.first << "];";
   }
   ss << myHeap << ";";

   bool notZero = true;
   do {
      const auto& elem = myHeap.pop();
      notZero = ( elem.first != -1 );
      ss << "Pop: " << elem.second << "[" << elem.first << "];";
   } while ( notZero );

   if ( printToStd ) {
      std::cout << ss.str() << std::endl;
      std::cout << "---" << std::endl;
      std::cout << result << std::endl;
   }
   return ss.str() == result;
}

static bool my_heap_test_2( bool printToStd = false) {
   std::string result = "MyHeap(  1[111] 3[666] 2[555] 5[99] 18[333] 20[777] 16[444] 21[88] 17[888] 19[222] );Remove: 3[666];MyHeap(  1[111] 5[99] 2[555] 17[888] 18[333] 20[777] 16[444] 21[88] 19[222] );";

   std::stringstream ss;
   MyHeap< int, int > myHeap(1000);
   myHeap.insert( 88, 21 );
   myHeap.insert( 99, 5 );
   myHeap.insert( 777, 20 );
   myHeap.insert( 111, 1 );
   myHeap.insert( 333, 18 );
   myHeap.insert( 444, 16 );
   myHeap.insert( 555, 2 );
   myHeap.insert( 888, 17 );
   myHeap.insert( 666, 3 );
   myHeap.insert( 888, 17 );
   myHeap.insert( 222, 19 );
   ss << myHeap << ";";

   const auto& elem = myHeap.remove( 666 );
   ss << "Remove: " << elem.second << "[" << elem.first << "];";
   ss << myHeap << ";";

   if ( printToStd ) {
      std::cout << ss.str() << std::endl;
      std::cout << "---" << std::endl;
      std::cout << result << std::endl;
   }
   return ss.str() == result;
}

void my_heap_all_test() {
   assert( my_heap_test_1() );
   assert( my_heap_test_2() );
}

#endif /*MY_HEAP*/
