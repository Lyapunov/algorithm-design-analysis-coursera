#ifndef COMMON_C2W1P1
#define COMMON_C2W1P1

struct Stuff {
   int weight;
   int length;
};

static bool readInput( std::string filename, std::vector<Stuff>& stuffs, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   // first line
   std::string line;
   if ( !std::getline( is, line ) ) {
      return false;
   }
   std::istringstream ss(line);
   int numOfRecords = 0;
   ss >> numOfRecords;

   if ( debugmode ) {
      std::cout << "=== READING " << numOfRecords << " RECORDS" << std::endl;
   }
   for ( int i = 0; i < numOfRecords; ++i ) {
      if ( !std::getline( is, line ) ) {
         return false;
      }
      std::istringstream ss(line);
      Stuff stuff;
      ss >> stuff.weight;
      ss >> stuff.length;
      stuffs.push_back( stuff );
   }
   is.close();

   return true;
}

#endif /*COMMON_C2W1P1*/
