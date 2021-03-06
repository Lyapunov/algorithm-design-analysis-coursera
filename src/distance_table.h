#ifndef DISTANCE_TABLE
#define DISTANCE_TABLE

#include <iomanip>

class DistanceVector
{
public: 
   DistanceVector( size_t size, size_t node ) : vector_( size, 0 ) {
      for ( size_t i = 0; i < vector_.size(); ++i ) {
         vector_[i] = ( i == node ? 0 : infinite * 4 ); 
      }
   }

   inline int get( size_t i ) const {
      return vector_[i];
   }

   inline void set( size_t i, int value ) {
      // normalization
      if ( value > infinite ) { value = infinite * 4; }
      if ( value < -infinite ) { value = -infinite * 4; }

      vector_[i] = value;
   }

   std::ostream& debugPrint( std::ostream& os ) const {
      for ( const auto& value : vector_ ) {
         if ( value >= infinite ) 
         {
            os << "     +inf ";
         } else if ( value <= -infinite ) {
            os << "     -inf ";
         } else {
            os << std::setw(9) << value << " ";
         }
      }
      os << std::endl;
      return os;
   }

private:
   static constexpr int infinite = 2<<26;
   std::vector<int> vector_;
};

std::ostream& operator<<( std::ostream& os, const DistanceVector& dv ) {
   os << "DistanceVector(" << std::endl;
   dv.debugPrint( os );
   os << ")";
   return os;
}

class DistanceTable
{
public: 
   DistanceTable( size_t size ) : size_( size ) {
      tablet_ = new int*[size_];
      for ( size_t i = 0; i < size_; ++i ) {
         tablet_[i] = new int[size_];
      }
      init_tablet_values();
   }

   DistanceTable( const DistanceTable& ) = delete;

   DistanceTable( DistanceTable&& other ) {
      swap( other );
   }

   ~DistanceTable() {
      for ( size_t i = 0; i < size_; ++i ) {
         delete tablet_[i];
      }
      delete tablet_;
   }

   inline int get( size_t i, size_t j ) const {
      return tablet_[i][j];
   }

   inline void set( size_t i, size_t j, int value ) {
      // normalization
      if ( value > infinite ) { value = infinite * 4; }
      if ( value < -infinite ) { value = -infinite * 4; }

      tablet_[i][j] = value;
   }

   void swap( DistanceTable& other ) {
      size_t size   = size_;
      int**  tablet = tablet_;
      size_         = other.size_;
      tablet_       = other.tablet_;
      other.size_   = size;
      other.tablet_ = tablet;
   }

   std::ostream& debugPrint( std::ostream& os ) const {
      for ( size_t j = 0; j < size_; ++j ) {
         for ( size_t i = 0; i < size_; ++i ) {
            int& value = tablet_[i][j];
            if (  value >= infinite ) 
            {
               os << "     +inf ";
            } else if ( value <= -infinite ) {
               os << "     -inf ";
            } else {
               os << std::setw(9) << value << " ";
            }
         }
         os << std::endl;
      }
      return os;
   }

private:
   void init_tablet_values() {
      for ( size_t i = 0; i < size_; ++i ) {
         for ( size_t j = 0; j < size_; ++j ) {
            tablet_[i][j] = ( i == j ? 0 : infinite * 4 ); 
         }
      }
   }

   static constexpr int infinite = 2<<26;
   size_t size_;
   int**  tablet_;
};

std::ostream& operator<<( std::ostream& os, const DistanceTable& dt ) {
   os << "DistanceTable(" << std::endl;
   dt.debugPrint( os );
   os << ")";
   return os;
}

#endif /*DISTANCE_TABLE*/
