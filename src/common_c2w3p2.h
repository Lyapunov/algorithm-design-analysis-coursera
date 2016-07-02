#ifndef COMMON_C2W1P1
#define COMMON_C2W1P1

struct Item {
   int weight;
   int value;
};

std::ostream& operator<<( std::ostream& os, const Item& elem ) {
   os << "--- Item( weight:" << elem.weight << ", value:" << elem.value << ")"; 
   return os;
}

static bool readInput( std::string filename, int& knapsack_size, std::vector<Item>& items, int debugmode = 0 )
{
   std::ifstream is;
   is.open( filename.c_str() );

   // first line
   std::string line;
   if ( !std::getline( is, line ) ) {
      return false;
   }
   std::istringstream ss(line);
   if ( !( ss >> knapsack_size ) ) {
      return false;
   }
   int number_of_items = 0;
   if ( !( ss >> number_of_items ) ) {
      return false;
   }

   if ( debugmode ) {
      std::cout << "=== READING " << number_of_items << " RECORDS" << std::endl;
   }
   for ( int i = 0; i < number_of_items; ++i ) {
      if ( !std::getline( is, line ) ) {
         return false;
      }
      std::istringstream ss(line);
      Item item;
      ss >> item.value;
      ss >> item.weight;
      items.push_back( item );
   }
   is.close();

   return true;
}

#endif /*COMMON_C2W1P1*/
