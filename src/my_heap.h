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

   void insert( const Key& key, const Value& value ) {
      assert( keys_.find( key ) == keys_.end() );
      content_.push_back( std::make_pair( key, value ) );
      const unsigned position = content_.size() - 1;
      keys_[ key ] = position;
      bubble_up( position );
   }

   std::pair< Key, Value > pop() {
      if ( content_.size() == 0 ) {
         return std::pair<Key, Value>(); // empty
      }
      return remove( 0 ); 
   }

   std::pair<Key, Value> remove( const Key& key ) {
      assert( keys_.find( key ) != keys_.end() );
      return remove( keys_[ key ] ); 
   }

   void debugPrint( std::ostream& os ) const {
      os << "MyHeap( ";
      for ( const auto& elem : content_ ) {
         os << " " << elem.second << "[" << elem.first << "]";
      }
      os << " )";
   }

private:
   std::pair< Key, Value > remove( unsigned position ) {
      const unsigned last_position = content_.size() - 1;
      swap_positions( position, last_position ); 

      const auto retval = content_[ last_position ];
      content_.pop_back();
      keys_.erase( retval.first );

      bubble_down( position );
      return retval;
   }

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

      keys_[ content_[position1].first ] = position1;
      keys_[ content_[position2].first ] = position2;
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
   std::string result = "MyHeap(  );MyHeap(  1[Anna] 5[Eric] 20[Tobias] 21[Ulrich] 18[Richard] );MyHeap(  1[Anna] 3[Cecile] 2[Bob] 5[Eric] 18[Richard] 20[Tobias] 17[Peter] 21[Ulrich] 7[Gamov] );Pop: 1[Anna];Pop: 2[Bob];Pop: 3[Cecile];Pop: 5[Eric];Pop: 7[Gamov];MyHeap(  17[Peter] 18[Richard] 20[Tobias] 21[Ulrich] );Pop: 17[Peter];Pop: 18[Richard];Pop: 20[Tobias];Pop: 21[Ulrich];Pop: 0[];";

   std::stringstream ss;
   MyHeap< std::string, int > myHeap; 
   ss << myHeap << ";";
   myHeap.insert( "Ulrich", 21 );
   myHeap.insert( "Eric", 5 );
   myHeap.insert( "Tobias", 20 );
   myHeap.insert( "Anna", 1 );
   myHeap.insert( "Richard", 18 );
   ss << myHeap << ";";
   myHeap.insert( "Peter", 17 );
   myHeap.insert( "Bob", 2 );
   myHeap.insert( "Gamov", 7 );
   myHeap.insert( "Cecile", 3 );
   ss << myHeap << ";";

   for (int i = 0; i < 5; ++i ) {
      const auto& elem = myHeap.pop();
      ss << "Pop: " << elem.second << "[" << elem.first << "];";
   }
   ss << myHeap << ";";

   bool notZero = true;
   do {
      const auto& elem = myHeap.pop();
      notZero = ( elem.first != "" );
      ss << "Pop: " << elem.second << "[" << elem.first << "];";
   } while ( notZero );

   if ( printToStd ) {
      std::cout << ss.str();
   }
   return ss.str() == result;
}

static bool my_heap_test_2( bool printToStd = false) {
   std::string result = "MyHeap(  1[Anna] 3[Cecile] 2[Bob] 5[Eric] 17[Q] 20[Tobias] 16[Peter] 21[Ulrich] 7[Gamov] 18[Richard] 19[Samuel] );Remove: 3[Cecile];MyHeap(  1[Anna] 5[Eric] 2[Bob] 7[Gamov] 17[Q] 20[Tobias] 16[Peter] 21[Ulrich] 19[Samuel] 18[Richard] );";

   std::stringstream ss;
   MyHeap< std::string, int > myHeap; 
   myHeap.insert( "Ulrich", 21 );
   myHeap.insert( "Eric", 5 );
   myHeap.insert( "Tobias", 20 );
   myHeap.insert( "Anna", 1 );
   myHeap.insert( "Richard", 18 );
   myHeap.insert( "Peter", 16 );
   myHeap.insert( "Bob", 2 );
   myHeap.insert( "Gamov", 7 );
   myHeap.insert( "Cecile", 3 );
   myHeap.insert( "Q", 17 );
   myHeap.insert( "Samuel", 19 );
   ss << myHeap << ";";

   const auto& elem = myHeap.remove( "Cecile" );
   ss << "Remove: " << elem.second << "[" << elem.first << "];";
   ss << myHeap << ";";

   if ( printToStd ) {
      std::cout << ss.str();
   }
   return ss.str() == result;
}

void my_heap_all_test() {
   assert( my_heap_test_1() );
   assert( my_heap_test_2() );
}

#endif /*MY_HEAP*/
