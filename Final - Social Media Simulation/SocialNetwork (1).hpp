#pragma once
#include <algorithm>
#include <initializer_list>
#include <list>
#include <map>
#include <utility>
#include <vector>

#include "Person.hpp"

class SocialNetwork
{
  /*
    Private members and type definitions
  */

  // typedef allows us to rename a standard data structure for added clarity
  typedef std::pair<int, Person>    Node;    // A Node has an ID (int) and a value (Person)
  typedef std::pair<Node *, Node *> Edge;    // An Edge is a connection between 2 Nodes

  // Store the list of edges (Connections)
  std::list<Edge> EdgeList;
  // Store the actual Nodes in our graph
  std::vector<Node> Graph;

  /*
    Private Utility Functions
  */

  // 1. Create a list of Edges
  // 2. Search the EdgeList object for edges where either node relates to the person
  // 3. Add them to the new list and return it
  std::list<Edge> _findEdges( Person person )   //This function returns a new list of edge that shows connection of a person
  {
    std::list<Edge> newList;     //create a list of edges

    for( Edge i : EdgeList)      //loop through EdgeList to search for edges where note relates to the person
    {
      if( i.first->second == person || i.second->second == person )
      {
        newList.push_back( i );  //if found, add to the new list
      }
    }
    return newList;
  }

  // 1. Use a classic index-based loop on the Graph vector
  // 2. Find the Node where the value = the person provided
  // 3. When found, return the memory address
  // 4. If nothing is found, return nullptr
  Node * _nodeFor( Person person )    //this function returns the memory address of a person on the graph.
  {
    for( int i = 0; i < static_cast<int>( Graph.size() ); i++ ) //loop through the Graph vector to find the Node where the value = the person
    {
      if( Graph[i].second == person )    //if found, return the memory address
      {
        return &Graph[i];
      }
    }
    return nullptr;                      //if nothing is found, just return nullptr
  }

  // 1. Create a list of Node pointers to return
  // 2. Find the Node pointer for the person provided
  // 3. If the node is found (not nullptr) find the person's Edges
  // 4. Loop through the found edges and find the Node pointer that is NOT the person parameter
  // 5. Add the Node pointer to your return list and return that list at the end
  std::list<Node *> _friendsOf( Person person )   //This function returns a list of nodes of a person's friends
  {
    std::list<Node *> newList;            //create a new list of Node pointers to return
    Node * source = _nodeFor( person );   //Find the Node pointer for the person
    if( source != nullptr )               //if the node is found
    {
      std::list<Edge> edges = _findEdges( person );   //find the person's Edges
      for( Edge i : edges )   //loop through the found edges and find the Node pointer that is not the person parameter
      {
        Node * obj = i.first->second == person ? i.second : i.first;
        newList.push_back( obj );   //add the Node pointer to return list (newList)
      }
    }
    return newList;
  }

  // This function will calculate the minimum # of edges between 2 Nodes
  // visited, min_edges and edge_count are passed by REFERENCE so they can be modified by this function
  // This function doesn't need to return anything since the important variables are references that are modified
  // Use the Node type's first property as the index for the visited vector.
  // You can use this example as a reference: https://www.geeksforgeeks.org/minimum-number-of-edges-between-two-vertices-of-a-graph-using-dfs/
  // Modifications need to be made for this to work with our structures but it can be done!
  void _minEdges( std::vector<bool> & visited, Person pStart, Person pEnd, int & min_edges, int & edge_count )
  //This function calculates the minimum number of edges between 2 nodes, function does not return anything as parameters are passed by reference, this is a recursive function
  {
    Node * currentNode = _nodeFor( pStart ); 

    visited[currentNode->first] = true;   //for keeping track of visited nodes 

    if( pStart == pEnd )                  //if we have found the destination node, then check whether count of total number of edges is less than the minimum number of edges or not
    {
      if( min_edges > edge_count ) 
      {
        min_edges = edge_count;
      }
    }
    else                                  //if current node is not destination, recur for all the nodes adjacent to current node
    {
      std::list<Edge> pEdges = _findEdges( pStart );

      for( auto i = pEdges.begin(); i != pEdges.end(); i++ )
      {
        Edge E = *i;

        Node * N = E.first->second == pStart ? E.second : E.first;

        if( !visited[N->first] )
        {
          edge_count++;
          _minEdges( visited, N->second, pEnd, min_edges, edge_count );
        }
      }
    }
    visited[currentNode->first] = false;  //mark current node as univisited
    edge_count--;                         //decrement the count of number of edges
  }



public:
  // Default Constructor
  SocialNetwork() = default;  //default constructor

  // Constructor with a {bracket, separated} list of Person objects
  SocialNetwork( const std::initializer_list<Person> & newUsers ) //Constructor for class SocialNetwork with initialized list
  {
    for( auto && person : newUsers )
    {
      Add( person );
    }
  }

  // Use the _minEdges() function to calculate the minimum # of edges between 2 people
  // 1. Create the visited vector; initialize it with the same size as the Graph vector so it has enough space!
  // 2. Create the min_edges int and initialize it to the size of the EdgeList (which would be the maximum)
  // 3. Create the edge_count int and set it to 0 to start.
  // 4. Call the _minEdges() function with the appropriate arguments
  // 5. Return the min_edges int. Our _minEdges() function will modify it as needed since it is passed via reference.
  int distanceBetween( Person p1, Person p2 ) //This function calculate the minimum number of edges between 2 people that are passed as parameters to the function
  {
    std::vector<bool> visited( Graph.size(), false );     //Create the visited vector and initialize it with the same size as the Graph vector, for use as parameter for calling _minEdges() function
    int min_edges = static_cast<int>(EdgeList.size());    //Create min_edges integer and initialize it with the size of EdgeList, for use as a parameter for calling _minEdges() function
    int edge_count = 0;                                   //Create edge_count integer and set it to 0, for use as a parameter for calling _minEdges() function
    _minEdges( visited, p1, p2, min_edges, edge_count );  //call _minEdges function to find the minimum distance between 2 persons
    return min_edges;                                     //return min_edges integer which tells us the degrees of seperation between 2 persons
  }

  // List of people in the graph
  // 1. Just create a temporary list of people, loop through Graph and add each person to the temporary list.
  // 2. Return the temporary list
  std::list<Person> allPeople()             //This function returns a list of all the people in the Graph
  {
    std::list<Person> temp_people;         //Create a list of people
    for( Node p : Graph )                  //loop through Graph
    {
      temp_people.push_back( p.second );   //add each person in the Graph to the new list of people
    }
    return temp_people;                    //return the new list of people
  }

  // Make use of Lambda functions to find people who meet certain criteria (First/Last Name and/or Age)
  // The first parameter is a lambda function. It returns bool & needs a noexcept flag after the parentheses when you go to use it.
  // Parameter 2 is optional and will help us decide how to sort the list after it is built.
  std::list<Person> findPeople( std::function<bool( Person )> filterFunction)
  {
    // 1. Create a list to return
    // 2. Loop through the Graph
    // 3. For each node in the Graph, run the Node's Person (value) through the filterFunction()
    // 4. IF the filterFunction() returns true, add that person to the output list
    // 5. Use the sortby variable to decide how to sort the list.
    // 6. std::list has a built-in sort function. It requires use of the comparator defined in the Person object.
    std::list<Person> outList;    //create a list of person to return
    for( Node item : Graph )      //loop through the existing Graph
    {
      if( filterFunction( item.second ) )   //for each node in the Graph, run the Node's Person through the filterFunction()
      {
        outList.push_back( item.second );   //if true, add that person to the output list of person to return
      }
    }
    return outList;     //return the list of person
  }

  // Similar to findPeople but we only wnat to find 1 person
  // Use the findPeople function with a Lambda that checks for the provided first/last Names
  // The age is optional. If age < 0, ignore it. Otherwise use it in the filter & compare it to p.Age()
  // Make sure the resulting list of People is not Empty and, if not, select the first element using .front() and return it.
  // Return a list of people because you might NOT find the person, so the list will be empty!
  std::list<Person> findPerson( std::string firstName, std::string lastName, int age = -1 )
  //This function finds a person in the Graph by their firstname, lastname, and age(optional) as parameters for the function
  {
    std::list<Person> outList = {};       //create a new list of person to store the person and return

    if( age > 0)                          //if age is passed as a parameter to the function
    { 
      for( auto i : Graph )               //loop through the Graph
      {
        if( i.second.FirstName() == firstName && i.second.LastName() == lastName && i.second.Age() == age ) //if a person in the graph matches the parameters passed
        {
          outList.push_back( i.second );  //add that person to the new list to return
        }
      }
    } 
    else                                  //else if age is not passed as a parameter to the function
    {
      for( auto i : Graph )               //loop through the Graph
      {
        if( i.second.FirstName() == firstName && i.second.LastName() == lastName )  //if a person in the Graph matches the parameters passed
        {
          outList.push_back( i.second );  //add that person to the new list to return
        }
      }
    }
    return outList;                       //return the list of person found
  }

  // Use the _friendsOf() function to find everyone connected to the person
  // Output their information using std::cout (the Bio() function works well for this)
  void showFriends( Person person )       //This function outputs to console the friends of a person that is passed as parameter to this function
  {
    auto friends = _friendsOf( person );  //Create a list of person to store the node of friends that is returned from _friendsOf() function
    if( friends.empty() == false )        //if list is not empty (1 or more than 1 person found)
    {
      for( auto i : friends )             //loop through the list
      {
        std::cout << i->second.Bio() << std::endl;  //output the friends along with their bio to console
      }
    }
    else                                  //if list is empty (no friend(s) are found)
    {
      std::cout << "No friends for the particular individual found." << std::endl;  //output to console telling user that no friends are found for the particular person
    }
  }

  // Just a shortcut to be able to use showFriends() with first/last name and possibly age
  // Just use the findPerson() function and the showFriennds() function, this should only be 2 lines of code.
  void showFriends( std::string firstName, std::string lastName, int age = -1 ) //This function does the same as the previous showFriends function, but with the ability to use with first/last name and age
  {
    std::list<Person> results = findPerson( firstName, lastName, age );   //we need to find the person in the graph first before searching for friends
    if( results.empty() == false )                    //if found
    {
      Person targetPerson = results.front();         //store the person in targetPerson
      showFriends( targetPerson );                   //call the previous showFriends function to find the person's friends
    }
    std::cout << "Person Not Found" << std::endl;    //if not found, output to console to tell user that the person to search friends for is not found in the graph
  }

  // Add a Node to the graph
  // The first property should = Graph size and second = the Person to add
  void Add( Person newPerson )    //This function adds a new person to the existing Graph, returns nothing
  {
    Node new_person;              //create a new Node for later to be inserted into the Graph
    new_person.first = static_cast<int>(Graph.size());    //first property of the Node should be the Graph size
    new_person.second = newPerson;                        //second property of the Node is the info of the Person to add
    Graph.push_back( new_person );                        //add the just created person to the existing Graph using push_back
  }

  // Create an Edge between 2 people
  // 1. Find each person's node using _nodeFor
  // 2. Create an edge using std::make_pair() and add it to the EdgeList
  void Connect( Person p1, Person p2 )    //This function creates an Edge between 2 people, returns nothing
  {
    auto nodep1   = _nodeFor( p1 );         //find the first person node using _nodeFor function and store it into nodep1
    auto nodep2   = _nodeFor( p2 );         //find the second person node using _nodeFor function and store it into nodep2
    auto new_edge = std::make_pair( nodep1, nodep2 );   //create an edge between nodep1 and nodep2 using std::make_pair() function
    EdgeList.push_back( new_edge );                     //add the new edge to the existing EdgeList
  }

// Here you don't need +nodeFor you can just access the node on the graph with brackets
// Be sure to subtract 1 from these numbers because the graph vector index is 0-based!
// You'll need to use the address operator here...
  void Connect( int index1, int index2 )  
  //This function does the same as the previous Connect but instead of calling _nodeFor function to find each person's node, this function directly access it with the address operator
  {
    auto nodep1   = &Graph[index1 - 1];   //find the first person node directly with the address operator with the provided index and store it into nodep1
    auto nodep2   = &Graph[index2 - 1];   //find the second person node directly with the address operator with the provided index and store it into nodep2
    auto new_edge = std::make_pair( nodep1, nodep2 );   //create an edge between nodep1 and nodep2 using std::make_pair() function
    EdgeList.push_back( new_edge );                     //add the new edge to the existing EdgeList
  }

  // This does the exact same thing as connect but allows us to add many connections at once
  // You need a loop within a loop. The people parameter is itself a list of lists of Person objects.
  // Use a vector in loop 1 to store pairs of people found in loop 2
  // In loop 2, Use bracket[j] notation and the Connect() function to connect the 2 people found
  // Clear the vector before continuing the loop so it only ever has 2 people at once (vector has a .clear() function)
  void Connect( const std::initializer_list<std::initializer_list<Person>> & people )
  //This function does the same thing as the previous connect functions but allows to add many connections at once because an initializer list is used here
  {
    for( auto i : people )    //loop through the initializer list of person
    {
      std::vector<Person> targets;
      for( auto j : i )       //use the vector to store pairs of people found in loop 2
      {
        targets.push_back( j );  
      }
      Connect( targets[0], targets[1] );  //use Connect() function to connect the 2 people found
      targets.clear();                    //clear the vector before continuing the loop again so it only has 2 people at once
    }
  }

 // Just return the size of the Graph field. Graph is PRIVATE so your code in main.cpp
 // doesn't have any way of accessing it directly. 

 //this function returns the size of the graph
  std::size_t graphSize()
  {
    return Graph.size();
  }
// Return the size of the EdgeList field. EdgeList is PRIVATE so your code in main.cpp
// doesn't have any way of accessing it directly. 

//this function returns the size of the EdgeList
  std::size_t edgeCount()
  {
    return EdgeList.size();
  }
};
