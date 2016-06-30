#ifndef UNION_FIND
#define UNION_FIND

#include <vector>
#include <ostream>
#include <sstream>
#include <cassert>

class UnionFind {
public:
   UnionFind( unsigned n, std::ostream* debugStream = nullptr ) : pointers_( n, 0 ), chains_( n, 0 ), sizes_( n, 1 ), debugStream_( debugStream ) {
      for ( unsigned i = 0; i < pointers_.size(); ++i ) {
         pointers_[i] = i;
         chains_[i]   = i;
      }
   }

   unsigned findLeader( unsigned i ) {
      unsigned retval = findLeaderInternal( i );
      if ( debugStream_ ) {
         *debugStream_ << "findLeader( " << i << " ) -> " << retval << "; ";
      }
      return retval;
   }

   void doUnion( unsigned i, unsigned j ) {
      unsigned leaderI = findLeaderInternal( i );
      unsigned leaderJ = findLeaderInternal( j );

      // managing sizes to achieve O( n log n ) speed
      if ( sizes_[ leaderI ] <= sizes_[ leaderJ ] ) {
          meltFirstIntoSecond( leaderI, leaderJ );
      } else {
          meltFirstIntoSecond( leaderJ, leaderI );
      }
      sizes_[ leaderJ ] += sizes_[ leaderI ];
      sizes_[ leaderI ] = sizes_[ leaderJ ];

      if ( debugStream_ ) {
         *debugStream_ << "doUnion( " << i << ", " << j << " ) -> ";
         debugPrint( *debugStream_ );
         *debugStream_ << "; ";
      }
   }

   void debugPrint( std::ostream& os ) const {
      os << "UnionFind( ";
      for ( const auto& elem : pointers_ ) {
         os << elem << " ";
      }
      os << ")";
   }
private:
   inline unsigned findLeaderInternal( unsigned i ) {
      assert( i < pointers_.size() );
      return pointers_[ i ];
   }

   void meltFirstIntoSecond( unsigned leaderI, unsigned leaderJ ) {
      // first, the leader of J would have a chain pointer to leaderI
      unsigned oldLeaderJChainPointer = chains_[ leaderJ ];
      chains_[ leaderJ ] = leaderI;
      
      // start to go through the items of I
      unsigned chainPointer = leaderI;
      while ( chains_[ chainPointer ] != leaderI ) {
         pointers_[ chainPointer ] = leaderJ;
         chainPointer = chains_[ chainPointer ];
      }
      pointers_[ chainPointer ] = leaderJ;

      // last chain link in I points to leaderI, update
      chains_[ chainPointer ] = oldLeaderJChainPointer;
   }

   std::vector< unsigned > pointers_;
   std::vector< unsigned > chains_;
   std::vector< unsigned > sizes_;
   mutable std::ostream* debugStream_;
};

std::ostream& operator<<( std::ostream& os, const UnionFind& uf ) {
   uf.debugPrint( os );
   return os;
}

static bool uf_test_1( bool printToStd = false) {
   std::stringstream ss;
   UnionFind uf(10, &ss);
   uf.findLeader(0);
   uf.findLeader(4);
   uf.findLeader(9);
   uf.doUnion( 2, 3 );
   uf.doUnion( 4, 5 );
   uf.doUnion( 1, 9 );
   uf.doUnion( 3, 9 );
   uf.doUnion( 7, 8 );
   uf.doUnion( 4, 6 );
   uf.doUnion( 0, 6 );
   uf.doUnion( 8, 9 );
   uf.doUnion( 0, 9 );

   std::string result = "findLeader( 0 ) -> 0; findLeader( 4 ) -> 4; findLeader( 9 ) -> 9; doUnion( 2, 3 ) -> UnionFind( 0 1 3 3 4 5 6 7 8 9 ); doUnion( 4, 5 ) -> UnionFind( 0 1 3 3 5 5 6 7 8 9 ); doUnion( 1, 9 ) -> UnionFind( 0 9 3 3 5 5 6 7 8 9 ); doUnion( 3, 9 ) -> UnionFind( 0 9 9 9 5 5 6 7 8 9 ); doUnion( 7, 8 ) -> UnionFind( 0 9 9 9 5 5 6 8 8 9 ); doUnion( 4, 6 ) -> UnionFind( 0 9 9 9 5 5 5 8 8 9 ); doUnion( 0, 6 ) -> UnionFind( 5 9 9 9 5 5 5 8 8 9 ); doUnion( 8, 9 ) -> UnionFind( 5 9 9 9 5 5 5 9 9 9 ); doUnion( 0, 9 ) -> UnionFind( 9 9 9 9 9 9 9 9 9 9 ); ";

   if ( printToStd ) {
      std::cout << ss.str() << std::endl;
      std::cout << result << std::endl;
   }

   return result == ss.str();
}

static void uf_all_test() {
   assert( uf_test_1() );
}

#endif /*UNION_FIND*/
