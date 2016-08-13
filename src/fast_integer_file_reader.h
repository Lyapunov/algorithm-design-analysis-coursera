#ifndef FAST_INTEGER_FILE_READER
#define FAST_INTEGER_FILE_READER

#include <cassert>
#include <limits>
#include <vector>
#include <cstdio>

// Currently it is the fastest version, 1.4 secs on the largest input
bool fastIntegerFileReader( std::string filename, std::vector<long>& retval, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   if ( debugmode ) {
      std::cout << "=== READING INTEGER FILE" << std::endl;
   }

   long item = 0;
   long sign = +1;

   bool num = false;
   char buf[2048];

   do
   {
      is.read(buf, sizeof(buf));
      int k = is.gcount();
      for (int i = 0; i < k; ++i)
      {
          switch (buf[i])
          {
              case '\r':
              case '\n':
              case ' ':
                  if ( num ) {
                     retval.push_back( item * sign );
                     item = 0;
                     sign = +1;
                     num = false;
                  }
                  break;
              case '0': case '1': case '2': case '3':
              case '4': case '5': case '6': case '7':
              case '8': case '9':
                  num = true;
                  item = 10*item + buf[i] - '0';
                  break;
              case '-': 
                  num = true;
                  sign = -1;
                  break;
              case '+': 
                  num = true;
                  sign = +1;
                  break;
              default:
                  std::cout << "Bad format '" << buf[i] << "'" << std::endl;
          }    
      }
   } while ( is );

   is.close();
   return true;
}

std::ostream& operator<<( std::ostream& os, const std::vector<long>& vector ) {
   os << "vector(";
   for ( const auto& elem : vector ) {
      os << " " << elem;
   }
   os << " )";
   return os;
}

#endif /*FAST_INTEGER_FILE_READER*/
