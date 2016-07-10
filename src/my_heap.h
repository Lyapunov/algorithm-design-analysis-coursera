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
   MyHeap( size_t n ) { content_.reserve( n ); }

   void insert( Key key, Value value ) {
      content_.push_back( std::make_pair( key, value ) );
      bubble_up( content_.size() - 1 );
   }

   std::pair< Key, Value > pop() {
      if ( content_.size() == 0 ) {
         return std::pair<Key, Value>(); // empty
      }
      swap_positions( 0, content_.size() - 1 ); 
      bubble_down( 0 );
   }

   void debugPrint( std::ostream& os ) const {
      os << "MyHeap( ";
      for ( const auto& elem : content_ ) {
         os << " " << elem.second << "[" << elem.first << "]";
      }
      os << " )";
   }

private:
   void bubble_up( unsigned position ) {
      if ( position == 0 ) {
         return;
      }

      unsigned parent_position = get_parent_position( position );

      if ( content_[position].second < content_[parent_position].second ) {
         swap_positions( position, parent_position ); 
         bubble_up( parent_position );
      }
   }

   void bubble_down( unsigned parent_position ) {
      unsigned left_child  = get_child_position( parent_position, 0 );
      unsigned right_child = get_child_position( parent_position, 1 );

      bool left_valid  = is_valid_position( left_child );
      bool right_valid = is_valid_position( right_child );

      if ( !left_valid && !right_valid ) {
         return;
      }

      unsigned candidate = left_child; 
      if ( right_valid ) {
         if ( content_[ left_child ].second > content_[ right_child ].second ) {
            candidate = right_child;
         }
      }

      if ( content_[ candidate ].second < content_[ parent_position ].second ) {
         swap_positions( candidate, parent_position ); 
         bubble_down( candidate );
      }
   }

   inline void swap_positions( unsigned position1, unsigned position2 ) {
      std::iter_swap( begin( content_ ) + position1, begin( content_ ) + position2 );
   }

   inline bool is_valid_position( unsigned position ) {
      return position < content_.size();
   }

   static inline unsigned get_parent_position( unsigned position ) {
      if ( position == 0 ) {
         assert( false );
      }
      return ( position - 1 ) >> 1;
   }

   static inline unsigned get_child_position( unsigned position, bool isRightChild ) {
      return ( position << 1 ) + 1 + isRightChild;
   }

   std::vector< std::pair< Key, Value >> content_;
   std::map< Key, unsigned >  keys_;
};

template < class Key, class Value >
std::ostream& operator<<( std::ostream& os, const MyHeap<Key, Value>& mh ) {
   mh.debugPrint( os );
   return os;
}

static bool my_heap_test_1( bool printToStd = false) {
   MyHeap< std::string, int > myHeap; 
   if ( printToStd ) {
      std::cout << myHeap << std::endl;
   }
   return true;
}

void my_heap_all_test() {
   assert( my_heap_test_1( true ) );
}

#endif /*MY_HEAP*/
