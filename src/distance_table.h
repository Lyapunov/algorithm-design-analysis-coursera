#ifndef DISTANCE_TABLE
#define DISTANCE_TABLE

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

   ~DistanceTable() {
      for ( size_t i = 0; i < size_; ++i ) {
         delete tablet_[i];
      }
      delete tablet_;
   }

   int* operator[]( size_t pos ) {
      return tablet_[pos];
   }

   const int* operator[]( size_t pos ) const {
      return tablet_[pos];
   }

   void normalize( int& value ) {
      if ( value > infinite ) { value = infinite; }
      if ( value < -infinite ) { value = -infinite; }
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
            tablet_[i][j] = ( i == j ? 0 : infinite ); 
         }
      }
   }

   static constexpr int infinite = 2<<29;
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
