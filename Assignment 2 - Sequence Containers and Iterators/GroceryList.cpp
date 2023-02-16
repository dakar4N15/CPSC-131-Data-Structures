#include <algorithm>                                                                // find(), move(), move_backward(), equal(), swap(), lexicographical_compare()
#include <cmath>                                                                    // min()
#include <cstddef>                                                                  // size_t
#include <initializer_list>
#include <iomanip>                                                                  // setw()
#include <iterator>                                                                 // distance(), next()
#include <stdexcept>                                                                // logic_error
#include <string>

#include "GroceryItem.hpp"
#include "GroceryList.hpp"



// As a rule, I strongly recommend avoiding macros, unless there is a compelling reason - this is such a case. This really does need
// to be a macro and not a function due to the way the preprocessor expands the source code location information.  It's important to
// have these expanded where they are used, and not here. But I just can't bring myself to writing this, and getting it correct,
// everywhere it is used.  Note:  C++20 will change this technique with the introduction of the source_location class. Also note the
// usage of having the preprocessor concatenate two string literals separated only by whitespace.  Update:  As of clang 14, still no
// support for source_location.  Rummer has it that they may never implement it. :(  In the meantime ...
#define exception_location "\n detected in function \"" + std::string(__func__) +  "\""    \
                           "\n at line " + std::to_string( __LINE__ ) +                    \
                           "\n in file \"" __FILE__ "\""












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Constructors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Initializer List Constructor
GroceryList::GroceryList( const std::initializer_list<GroceryItem> & initList )
{
  for( auto && groceryItem : initList )   insert( groceryItem, Position::BOTTOM );

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Queries
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// size() const
std::size_t GroceryList::size() const
{
  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  ///////////////////////// TO-DO (1) //////////////////////////////
    /// All the containers are the same size, so pick one and return the size of that.  Since the forward_list has to calculate the
    /// size on demand, stay away from using that one.

  //this function returns the size of the grocery list
  //I choose to use array, its the simplest one, as it has a private variable that keeps track the size (number of elements) of the array
  //This function will return the size of the array, and the size also reflects the size of the four containers(they have the same size)
  return _gList_array_size;

  /////////////////////// END-TO-DO (1) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Accessors
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// find() const
std::size_t GroceryList::find( const GroceryItem & groceryItem ) const
{
  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  ///////////////////////// TO-DO (2) //////////////////////////////
    /// Locate the grocery item in this grocery list and return the zero-based position of that grocery item.  If the grocery item
    /// does not exist, return the size of this grocery list as an indicator the grocery item does not exist.  The grocery item will
    /// be in the same position in all the containers (array, vector, list, and forward_list) so pick just one of those to search.
    /// The STL provides the find() function that is a perfect fit here, but you may also write your own loop.

    //go through a loop to find and locate the grocery item in the grocery list until item is found
    //if item is found, return the zero-based position of that grocery item
    for (std::size_t a=0; a<_gList_array_size; a++)
    {
      if (_gList_array[a] == groceryItem) return a;
    }

    //if grocery item is not found, return the size of the grocery list
    return _gList_array_size;
  /////////////////////// END-TO-DO (2) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Modifiers
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// insert( position )
void GroceryList::insert( const GroceryItem & groceryItem, Position position )
{
  // Convert the TOP and BOTTOM enumerations to an offset and delegate the work
  if     ( position == Position::TOP    )  insert( groceryItem, 0      );
  else if( position == Position::BOTTOM )  insert( groceryItem, size() );
  else                                     throw std::logic_error( "Unexpected insertion position" exception_location );  // Programmer error.  Should never hit this!
}



// insert( offset )
void GroceryList::insert( const GroceryItem & groceryItem, std::size_t offsetFromTop )        // insert provided grocery item at offsetFromTop, which places it before the current grocery item at offsetFromTop
{
  // Validate offset parameter before attempting the insertion.  std::size_t is an unsigned type, so no need to check for negative
  // offsets, and an offset equal to the size of the list says to insert at the end (bottom) of the list.  Anything greater than the
  // current size is an error.
  if( offsetFromTop > size() )   throw InvalidOffset_Ex( "Insertion position beyond end of current list size" exception_location );


  /**********  Prevent duplicate entries  ***********************/
  ///////////////////////// TO-DO (3) //////////////////////////////
    /// Silently discard duplicate items from getting added to the grocery list.  If the to-be-inserted grocery item is already in
    /// the list, simply return.
    ///
    /// Remember, you already have a function that tells you if the to-be-inserted grocery item is already in the list, so use it.
    /// Don't implement it again.

    //use the find() function to locate if the item is already in the list, if item already existed in the list, return.
    //find() function loops through array contents of grocery items, it will return the position of the item in the array if
    //item is found on the list, if item is not found, the function will return array size. Hence, if the find() function does not
    //return the size of the array, means it is a duplicated item, and this function will silently discard it and return
    if (find(groceryItem) != size()) return;

  /////////////////////// END-TO-DO (3) ////////////////////////////


  // Inserting into the grocery list means you insert the grocery item into each of the containers (array, vector, list, and
  // forward_list). Because the data structure concept is different for each container, the way a grocery item gets inserted is a
  // little different for each.  You are to insert the grocery item into each container such that the ordering of all the containers
  // is the same.  A check is made at the end of this function to verify the contents of all four containers are indeed the same.


  { /**********  Part 1 - Insert into array  ***********************/
    ///////////////////////// TO-DO (4) //////////////////////////////
      /// Hint:  "Vector.hpp" in our Sequence Container Implementation Examples shows you how to do this.  (wink wink)
      /// https://github.com/TBettens/Data_Structure_Implementation_Examples/tree/latest/Sequence%20Container%20Implementation%20Examples/Vector
      ///
      /// Unlike the other containers, an std::array has fixed capacity and fixed size (and size is always == capacity) hence has no
      /// insert() function, so you have to write it yourself. Insert into the array by shifting all the items at and after the
      /// insertion point (offsetFromTop) to the right opening a gap in the array that can be populated with the given grocery item.
      /// Remember that arrays have fixed capacity and cannot grow, so make sure there is room in the array for another grocery item
      /// before you start by verifying _gList_array_size (the number of valid elements) is less than _gList_array.size() (the
      /// capacity).  If not, throw CapacityExceeded_ex. Also remember that you must keep track of the number of valid grocery items
      /// in your array, so don't forget to adjust _gList_array_size.
      ///
      /// Open a hole to insert new grocery item by shifting to the right everything at and after the insertion point.
      /// For example:  a[8] = a[7];  a[7] = a[6];  a[6] = a[5];  and so on.
      /// std::move_backward will be helpful, or write your own loop.

      //verify there is room in the array to insert a new item
      if (_gList_array_size >= _gList_array.size())           //if array is full, throw CapacityExceeded_ex to cancel insertion
      {
        throw CapacityExceeded_Ex("Cannot fit another grocery item" exception_location);
      }

      //insert into array at offsetFromTop
      //first, shift the array elements from the end of the list to one forward, and do the same to the elements before it until offsetfromtop, to make space
      //for new item to be inserted in offsetfromtop
      for (auto a = _gList_array_size; a > offsetFromTop; a--)
      {
        _gList_array[a] = std::move (_gList_array[a-1]);
      }
      //after shifting the array contents, insert the new item into the array, in this case in offsetfromtop
      _gList_array[offsetFromTop] = groceryItem;
      //finally, update _gList_array_size, to keep track the number of valid elements
      _gList_array_size++;

    /////////////////////// END-TO-DO (4) ////////////////////////////
  } // Part 1 - Insert into array




  { /**********  Part 2 - Insert into vector  **********************/
    ///////////////////////// TO-DO (5) //////////////////////////////
      /// The vector STL container std::vector has an insert function, which can be directly used here.  But that function takes a
      /// pointer (or more accurately, an iterator) that points to the grocery item to insert before.  You need to convert the
      /// zero-based offset from the top (the index) to an iterator by advancing _gList_vector.begin() offsetFromTop times.  The STL
      /// has a function called std::next() that does that, or you can use simple pointer arithmetic to calculate it.  "Vector.hpp"
      /// in our Sequence Container Implementation Examples also shows you how convert index to iterator, and iterator to index.
      ///
      /// Behind the scenes, std::vector::insert() shifts to the right everything at and after the insertion point, just like you
      /// did for the array above.

      //inserting new groceryitem into vector at position offsetfromtop
      _gList_vector.insert (_gList_vector.begin() + offsetFromTop, groceryItem);

    /////////////////////// END-TO-DO (5) ////////////////////////////
  } // Part 2 - Insert into vector




  { /**********  Part 3 - Insert into doubly linked list  **********/
    ///////////////////////// TO-DO (6) //////////////////////////////
      /// The doubly linked list STL container std::list has an insert function, which can be directly used here.  But that function
      /// takes a pointer (or more accurately, an iterator) that points to the grocery item to insert before.  You need to convert
      /// the zero-based offset from the top (the index) to an iterator by advancing _gList_dll.begin() offsetFromTop times.  The
      /// STL has a function called std::next() that does that, or you can write your own loop.

      //insert new groceryItem into doubly linked list at position offsetFromTop
      //unlike vector, doubly linked list is a bidirectional iterator, so we must advance the pointer using std::next() instead
      //of directly +
      _gList_dll.insert (std::next (_gList_dll.begin(), offsetFromTop), groceryItem);

    /////////////////////// END-TO-DO (6) ////////////////////////////
  } // Part 3 - Insert into doubly linked list




  { /**********  Part 4 - Insert into singly linked list  **********/
    ///////////////////////// TO-DO (7) //////////////////////////////
      /// The singly linked list STL container std::forward_list has an insert function, which can be directly used here.  But that
      /// function inserts AFTER the grocery item pointed to, not before like the other containers.  A singly linked list cannot
      /// look backwards, only forward.  You need to convert the zero-based offset from the top (the index) to an iterator by
      /// advancing _gList_sll.before_begin() offsetFromTop times.  The STL has a function called std::next() that does that, or you
      /// can write your own loop.

      //insert new groceryItem into singly linked list at position offsetFromTop
      //singly linked list is a forward iterator, it cannot look backwards, only forward, so we need to advance the pointer
      //_gList_sll.before_begin() offsetFromTop times using the function std::next()
      _gList_sll.insert_after (std::next (_gList_sll.before_begin(), offsetFromTop), groceryItem);

    /////////////////////// END-TO-DO (7) ////////////////////////////
  } // Part 4 - Insert into singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
} // insert( const GroceryItem & groceryItem, std::size_t offsetFromTop )



// remove( groceryItem )
void GroceryList::remove( const GroceryItem & groceryItem )
{
  // Delegate to the version of remove() that takes an index as a parameter
  remove( find( groceryItem ) );
}



// remove( offset )
void GroceryList::remove( std::size_t offsetFromTop )
{
  // Removing from the grocery list means you remove the grocery item from each of the containers (array, vector, list, and
  // forward_list). Because the data structure concept is different for each container, the way a grocery item gets removed is a
  // little different for each.  You are to remove the grocery item from each container such that the ordering of all the containers
  // is the same.  A check is made at the end of this function to verify the contents of all four containers are indeed the same.

  if( offsetFromTop >= size() )   return;                                           // no change occurs if (zero-based) offsetFromTop >= size()


  { /**********  Part 1 - Remove from array  ***********************/
    ///////////////////////// TO-DO (8) //////////////////////////////
      /// Hint:  "Vector.hpp" in our Sequence Container Implementation Examples shows you how to do this.  (wink wink)
      ///
      ///
      /// Close the hole created by shifting to the left everything at and after the remove point.
      /// For example:  a[5] = a[6];  a[6] = a[7];  a[7] = a[8];  and so on
      ///
      /// std::move() will be helpful, or write your own loop.  Also remember that you must keep track of the number of valid grocery items
      /// in your array, so don't forget to adjust _gList_array_size.

      //remove a groceryItem from array at offsetFromTop
      //overwrite the space of groceryItem to be removed by the element to its right, and do the same to the next (move elements in the right to left)
      //until the end of the array.
      for (auto a = offsetFromTop; a < _gList_array_size -1; a++)
      {
        _gList_array[a] = std::move (_gList_array[a+1]);
      }

      //update _gList_array_size, to keep track the number of valid elements
      _gList_array_size--;
      //make sure the now unused array element at the end is vacant
      _gList_array[_gList_array_size] = {};

    /////////////////////// END-TO-DO (8) ////////////////////////////
  } // Part 1 - Remove from array




  { /**********  Part 2 - Remove from vector  **********************/
    ///////////////////////// TO-DO (9) //////////////////////////////
      /// The vector STL container std::vector has an erase function, which can be directly used here.  But that function takes a
      /// pointer (or more accurately, an iterator) that points to the grocery item to be removed.  You need to convert the
      /// zero-based offset from the top (the index) to an iterator by advancing _gList_vector.begin() offsetFromTop times.  The STL
      /// has a function called std::next() that does that, or you can use simple pointer arithmetic to calculate it.  "Vector.hpp"
      /// in our Sequence Container Implementation Examples also shows you how convert index to iterator, and iterator to index.
      ///
      /// Behind the scenes, std::vector::erase() shifts to the left everything after the insertion point, just like you did for the
      /// array above.

      //remove a groveryItem from the vector at offsetFromTop
      //use the built-in erase function
      _gList_vector.erase (_gList_vector.begin() + offsetFromTop);

    /////////////////////// END-TO-DO (9) ////////////////////////////
  } // Part 2 - Remove from vector




  { /**********  Part 3 - Remove from doubly linked list  **********/
    ///////////////////////// TO-DO (10) //////////////////////////////
      /// The doubly linked list STL container std::list has an erase function, which can be directly used here.  But that function
      /// takes a pointer (or more accurately, an iterator) that points to the grocery item to remove.  You need to convert the
      /// zero-based offset from the top (the index) to an iterator by advancing _gList_dll.begin() offsetFromTop times.  The STL
      /// has a function called std::next() that does that, or you can write your own loop.

      //remove a groceryItem from the doubly linked list at offsetFromTop
      //doubly linked list is a bidirectional iterator, so unlike vector which is a random-access iterator, to move the pointer 
      //to the location of groceryItem we want to remove, we must use the std::next function
      _gList_dll.erase (std::next (_gList_dll.begin(), offsetFromTop));

    /////////////////////// END-TO-DO (10) ////////////////////////////
  } // Part 3 - Remove from doubly linked list




  {/**********  Part 4 - Remove from singly linked list  **********/
    ///////////////////////// TO-DO (11) //////////////////////////////
      /// The singly linked list STL container std::forward_list has an erase function, which can be directly used here.  But that
      /// function erases AFTER the grocery item pointed to, not the one pointed to like the other containers.  A singly linked list
      /// cannot look backwards, only forward.  You need to convert the zero-based offset from the top (the index) to an iterator by
      /// advancing _gList_sll.before_begin() offsetFromTop times.  The STL has a function called std::next() that does that, or you
      /// can write your own loop.

      //remove a groceryItem from the singly linked list at offSetFromTop
      //singly linked list is a forward iterator, so it cannot look backwards, only forward, so we need to advance _gList_sll.before_begin()
      //using std::next function until the desired location which is at offsetFromTop.
      _gList_sll.erase_after (std::next (_gList_sll.before_begin(), offsetFromTop));

    /////////////////////// END-TO-DO (11) ////////////////////////////
  } // Part 4 - Remove from singly linked list


  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
} // remove( std::size_t offsetFromTop )



// moveToTop()
void GroceryList::moveToTop( const GroceryItem & groceryItem )
{
  ///////////////////////// TO-DO (12) //////////////////////////////
    /// If the grocery item exists, then remove and reinsert it.  Otherwise, do nothing.
    /// Remember, you already have functions to do all this.

    //this function moves a groceryItem to the top of the list by removing it first and reinsert to the top

    //use the find() function to verify groceryItem exists and find the position of the groceryItem
    auto position = find(groceryItem);

    //if the groceryItem exist, which means find() function will not return the size, because if it
    //returns size, it means find() function has loop through the list and it cannot find the groceryItem.
    //hence, if the groceryItem exist somewhere in the list, it will first be removed then reinsert to the
    //top using the insert function.
    if (position != size())
    {
      remove (position);
      insert (groceryItem, Position::TOP);
    } 

  /////////////////////// END-TO-DO (12) ////////////////////////////
}



// operator+=( initializer_list )
GroceryList & GroceryList::operator+=( const std::initializer_list<GroceryItem> & rhs )
{
  ///////////////////////// TO-DO (13) //////////////////////////////
    /// Append (aka concatenate) the right hand side (rhs) to the bottom of this list by repeatedly inserting at the bottom of this
    /// grocery list. The input type is just a container of grocery items accessible with iterators just like all the other
    /// containers.  The constructor above gives an example.  Remember to add that grocery item at the bottom of each container
    /// (array, vector, list, and forward_list) of this grocery list, and that you already have a function that does that.

    //append the parameter rhs to the bottom of the grocery list repeatedly using the insert() function. The input type for this
    //function, or the parameter rhs is a container of grocery items that could be accessed with iterators. 
    for (auto a = rhs.begin(); a != rhs.end(); a++)
    {
      insert (*a, Position::BOTTOM);
    }

  /////////////////////// END-TO-DO (13) ////////////////////////////

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
  return *this;
}



// operator+=( GroceryList )
GroceryList & GroceryList::operator+=( const GroceryList & rhs )
{
  ///////////////////////// TO-DO (14) //////////////////////////////
    /// Append (aka concatenate) the right hand side (rhs) to the bottom of this list by repeatedly inserting at the bottom of this
    /// grocery list. All the rhs containers (array, vector, list, and forward_list) contain the same information, so pick just one
    /// to traverse. Walk the container you picked inserting its grocery items to the bottom of this grocery list. Remember to add
    /// that grocery item at the bottom of each container (array, vector, list, and forward_list) of this grocery list, and that you
    /// already have a function that does that.

    //append the rhs by inserting to the bottom of the grocery list using insert() function repeatedly. Since all the containers contain
    //the same information, in this case I chose the forward_list to traverse, and insert its grocery items to the bottom of the grocery list.
    //the insert function does the same to all the containers, so we do not need to do anything anymore.
    for (auto a : rhs._gList_sll)
    {
      insert (a, Position::BOTTOM);
    }

  /////////////////////// END-TO-DO (14) ////////////////////////////

  // Verify the internal grocery list state is still consistent amongst the four containers
  if( !containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );
  return *this;
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Relational Operators
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// operator<=>
std::weak_ordering GroceryList::operator<=>( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  ///////////////////////// TO-DO (15) //////////////////////////////
    /// Find the common extent.  That is, if one list has 20 grocery items, and the other has only 13, then the common extent is 13
    /// grocery items. We can compare only the first 13 grocery items of each list.  Find the first grocery item in the common
    /// extent that is different using three-way comparison (e.g., gItem1 <=> gItem2  !=  0) and return the results.  If no
    /// differences were found in the common extent, then the list with the smaller size is less than the other.  In that case,
    /// return the results of three-way comparing the sizes.
    ///
    ///
    /// The content of all the grocery lists's containers is the same - so pick an easy one to walk.

    //compare two grocery lists using the spaceship operator. we can only compare the lists based on the number of items can be found
    //If one list has more items than the other, then we can only compare up to the items(the least)
    //set the limit variable to the least number of grocery items between the two lists
    std::size_t limit = std::min (size(), rhs.size());

    for (std::size_t a = 0; a < limit; a++)
    {
      std::weak_ordering compare = _gList_array[a] <=> rhs._gList_array[a];
      if (compare != 0 ) return compare;
    }

    return size() <=> rhs.size();

  /////////////////////// END-TO-DO (15) ////////////////////////////
}



// operator==
bool GroceryList::operator==( GroceryList const & rhs ) const
{
  if( !containersAreConsistant() || !rhs.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  ///////////////////////// TO-DO (16) //////////////////////////////
    /// Two lists are different if their sizes are different, or one of their grocery items is different.  Otherwise the lists are equal.
    ///
    /// Comparing the sizes is quick (constant time), so check that first.
    ///
    ///
    /// Walk the list looking for grocery items that don't match.  The content of all the grocery lists's containers is the same -
    /// so pick an easy one to walk.

    //this function compares if two grocery lists are equal or not
    //first, we compare the size between the two grocery lists, if they are not equal, means they are not the same, so return false, otherwise continue...
    if (size() != rhs.size()) return false;

    //Second, we compare the each groceryItems from the two grocery lists by going through a loop, if one of them is not the same, 
    //means the lists are not the same, so return false
    for (std::size_t a = 0; a < (_gList_array.size() -1); a++)
    {
      if (_gList_array[a] != rhs._gList_array[a]) return false;
    }

    //if both the size and contents of the two grocery lists are equal, return true
    return true;

  /////////////////////// END-TO-DO (16) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Private member functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// containersAreConsistant() const
bool GroceryList::containersAreConsistant() const
{
  // Sizes of all containers must be equal to each other
  if(    _gList_array_size != _gList_vector.size()
      || _gList_array_size != _gList_dll.size()
      || _gList_array_size !=  gList_sll_size() ) return false;

  // Element content and order must be equal to each other
  auto current_array_position   = _gList_array .cbegin();
  auto current_vector_position  = _gList_vector.cbegin();
  auto current_dll_position     = _gList_dll   .cbegin();
  auto current_sll_position     = _gList_sll   .cbegin();

  auto end = _gList_vector.cend();
  while( current_vector_position != end )
  {
    if(    *current_array_position != *current_vector_position
        || *current_array_position != *current_dll_position
        || *current_array_position != *current_sll_position ) return false;

    // Advance the iterators to the next element in unison
    ++current_array_position;
    ++current_vector_position;
    ++current_dll_position;
    ++current_sll_position;
  }

  return true;
}



// gList_sll_size() const
std::size_t GroceryList::gList_sll_size() const
{
  ///////////////////////// TO-DO (17) //////////////////////////////
    /// Some implementations of a singly linked list maintain the size (number of elements in the list).  std::forward_list does
    /// not. The size of singly linked list must be calculated on demand by walking the list from beginning to end counting the
    /// number of elements visited.  The STL's std::distance() function does that, or you can write your own loop.

    //this function returns the number of elements in the forward list or singly linked list, to do this we use
    //the built-in distance function that basically counts the number of elements visited.
    return std::distance (_gList_sll.cbegin(), _gList_sll.cend());

  /////////////////////// END-TO-DO (17) ////////////////////////////
}












///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Non-member functions
//
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// operator<<
std::ostream & operator<<( std::ostream & stream, const GroceryList & groceryList )
{
  if( !groceryList.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  // For each grocery item in the provided grocery list, insert the grocery item into the provided stream.  Each grocery item is
  // inserted on a new line and preceded with its index (aka offset from top)
  unsigned count = 0;
  for( auto && groceryItem : groceryList._gList_sll )   stream << '\n' << std::setw(5) << count++ << ":  " << groceryItem;

  return stream;
}



// operator>>
std::istream & operator>>( std::istream & stream, GroceryList & groceryList )
{
  if( !groceryList.containersAreConsistant() )   throw GroceryList::InvalidInternalState_Ex( "Container consistency error" exception_location );

  ///////////////////////// TO-DO (18) //////////////////////////////
    /// Extract until end of file grocery items from the provided stream and insert them at the bottom of the provided grocery list.
    /// Be sure to extract grocery items and not individual fields such as product name or UPC.

  // this function extracts grocery items from the provided stream and insert them to the bottom of the groceryList
  GroceryItem groceryItem;

  while (stream >> groceryItem)
  {
    groceryList.insert (groceryItem, GroceryList::Position::BOTTOM);
  }
  /////////////////////// END-TO-DO (18) ////////////////////////////

  return stream;
}
