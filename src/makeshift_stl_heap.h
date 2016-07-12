#ifndef MAKESHIFT_STL_HEAP
#define MAKESHIFT_STL_HEAP

#include <map>

// the key is that [multi]{set,map} are invalidated only for an erased item
// so, there is no obstacle maintaining 'accessor' iterators in a separate vector
template<class Key, class Value>
class MakeshiftStlHeap
{
public:
   bool empty() const { return heap_.empty(); }

   const Value* getValue( const Key& key ) const {
      auto itAccessor = heap_accessors_.find( key );
      if ( itAccessor != heap_accessors_.end() ) {
         return &itAccessor->second->first;
      }
      return 0;
   }

   void insert( const Key& key, const Value& value ) { 
      remove( key );
      heap_accessors_[ key ] = heap_.insert( std::pair<int, int>( value, key ) );
   }

   std::pair<Value, Key> pop() {
      std::pair<Value, Key> retval = *heap_.begin();
      heap_.erase( heap_.begin() );
      heap_accessors_.erase( retval.second );
      return retval; 
   }

private:
   void remove( const Key& key ) {
      auto itAccessor = heap_accessors_.find( key );
      if ( itAccessor != heap_accessors_.end() ) {
         auto it = itAccessor->second;
         auto key = it->second;
         heap_.erase( it );
         heap_accessors_.erase( key );
      }
   }

   std::multimap<Value, Key> heap_;
   std::map< Key, typename std::multimap<Value, Key>::iterator > heap_accessors_;
};

#endif /*MAKESHIFT_STL_HEAP*/
