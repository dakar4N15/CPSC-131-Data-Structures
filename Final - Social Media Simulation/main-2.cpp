#include <fstream>
#include <ios>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

#include "Person.hpp"
#include "SocialNetwork.hpp"


// Override the >> operator for std::pair so we can import Edges from connections.txt

//Overriding the >> operator for std::pair so that we can import connection info between individuals or Edges from the file connections.txt
std::istream & operator>>( std::istream & stream, std::pair<int, int> & nums )
{
// TODO - IMPLEMENT
  char delimiter = '\0';
  std::pair<int, int> pair;
  if( stream >> pair.first && stream >> std::ws >> delimiter && stream >> std::ws >> pair.second )
  {
    nums = std::move( pair );
  }
  else
  {
    stream.setstate( std::ios::failbit );
  }
  return stream;
}

int main( )
{

  /*
  Part 1 - Setting Things Up
*/

// Task 1 - Complete the operator>> overload for std::pair objects so we can properly import connections.txt below

// Task 2 - Create a SocialNetwork Object

// Task 3 - Import the people.txt file contents into your SocialNetwork object as Nodes.

// Task 4 - Import the connections.txt file contents into your SocialNetwork object as Edges.

// Task 5 - Using std::cout, tell us how many Nodes (people) and how many Edges (connections) there are in the graph.



  // TODO - Construct the graph container

  //Create a SocialNetwork object
  SocialNetwork SocialMediaGraph;


  //Importing contents from people.txt file to the just created SocialNetwork object as nodes
  std::ifstream peopleFile( "people.txt" );
  if( peopleFile.is_open() )
  {
    for( std::string line; std::getline( peopleFile, line ); /**/ )
    {
      // TODO - IMPLEMENT
      Person  person;
      std::istringstream stream( line );
      stream >> person;
      if( person.isValid() )
      {
        SocialMediaGraph.Add( person );
      }
    }
    peopleFile.close();
  }

  //Importing connection contents between individuals from connections.txt file to the SocialNetwork object as edges
  std::ifstream connectionFile( "connections.txt" );
  if( connectionFile.is_open() )
  {
    for( std::string line; std::getline( connectionFile, line ); /**/ )
    {
      // TO DO - IMPLEMENT
      std::pair<int, int> coordinates;
      std::istringstream stream( line );
      stream >> coordinates;
      if( coordinates.first != coordinates.second )
      {
        SocialMediaGraph.Connect( coordinates.first, coordinates.second );
      }
    }
    connectionFile.close();
  }


  //Output to console how many nodes and how many edges there are in the graph
  std::cout << "\nPart 1 Output:\n";
  std::cout << "\nGraph Size: " << SocialMediaGraph.graphSize() << std::endl;
  std::cout << "Edge Count: "   << SocialMediaGraph.edgeCount() << std::endl;
  std::cout << "_____________________________________________________________\n\n";


/*
  Part 2 - Output some Text
*/

// Task 1 - Output all of the people, sorted ascending by Last Name (A-Z)

// Task 2 - Output all of the people, sorted ascending by age (youngest to oldest)

// Task 3 - Output people whose age is over 33, sorted descending (oldest to youngest)

// Task 4 - Prompt the user to search for a person by inputting a first name and a last name, then attempt to find that person and display their bio.
//          If the person is not found, display a "fname lname was not found" message instead.

// Task 5 - In addition to the bio in Task 4, output the person's list of friends if they are found. 

// Task 6 - Prompt the user to search for 2 people by entering their first and last names
//          Display the degrees of separation between the two people
//          If they are directly connected, display "name1 and name2 are friends!" instead

//Create a new list of people to store all individual data along with their connections that we just extracted from people.txt and connections.txt file
std::list<Person> people = SocialMediaGraph.allPeople();

std::cout << "Part 2 Output:\n";

//Output all of the people along with their bio sorted ascending by Last Name
if( people.empty() == false )
{
  people.sort( Person::lt_lname() );
  std::cout << std::endl << " Task 1 - All people sorted by ascending last name: \n";
  for( Person p : people )
  {
    std::cout << p.Bio() << std::endl;
  }
}

//Output all of the people along with their bio sorted ascending by age
if( people.empty() == false )
{
  people.sort( Person::lt_age() );
  std::cout << std::endl << " Task 2 - All people sorted by ascending age: \n";
  for( Person p : people )
  {
    std::cout << p.Bio() << std::endl;
  }
}

//Output all of the people with their bio sorted oldest to youngest, whose age is over 33
if( people.empty() == false )
{
  people.sort( Person::gt_age() );
  std::cout << std::endl << " Task 3 - People whose age is over 33, sorted oldest to youngest: \n";
  for( Person p : people )
  {
    if( p.Age() > 33 )
    {
      std::cout << p.Bio() << std::endl;
    }
  }
}

std::string input_firstName;      //to store input of first name to search for from user  
std::string input_lastName;       //to store input of last name to search for from user

//output to console, prompt user for first name and last name of the person to search for
std::cout << std::endl << " Task 4 & 5 - Search for a person and display their bio & friends: ";
std::cout << "\nPress <Enter> after inputting each name.";
std::cout << "\nFirst Name: ";
std::getline( std::cin, input_firstName );
std::cout << "Last Name: ";
std::getline( std::cin, input_lastName );

//search for the person from the list of graph using the findPerson function and store it in search_person
std::list<Person> search_person = SocialMediaGraph.findPerson( input_firstName, input_lastName );

if( search_person.empty() == false )    //if person was found on the Graph
{
  auto search = search_person.front();          //store the person object from the list of graph to search
  std::cout << search.Bio() << std::endl;       //output the person's bio
  std::cout << search.FirstName() << "\'s friends are:\n";    //search for the person's friend(s) if found
  SocialMediaGraph.showFriends( search );                     //using the showFriends function, it will show the person's list of friends or output no friends are found to console if no friend is found
}
else
{
  std::cout << "fname lname was not found" << std::endl;  //if person was not found on the graph, output to console to let user know
}

std::string input_firstName2;   //to store input of first name of second person to search for from user
std::string input_lastName2;    //to store input of last name of second person to search for from user

//prompt user to input two first names and last names of person to search for their degrees of separation
std::cout << std::endl << " Task 6 - Find the degrees of separation between 2 people:";
std::cout << "\nEnter data for 2 people to search for...";
std::cout << "\nFirst Name of first person: ";
std::getline( std::cin, input_firstName );
std::cout << "Last Name of first person: ";
std::getline( std::cin, input_lastName );
std::cout << "\nFirst Name of second person: ";
std::getline( std::cin, input_firstName2 );
std::cout << "Last Name of second person: ";
std::getline( std::cin, input_lastName2 );

std::list<Person> search_person1 = SocialMediaGraph.findPerson( input_firstName, input_lastName );     //search for the first person from the list using findPerson function
std::list<Person> search_person2 = SocialMediaGraph.findPerson( input_firstName2, input_lastName2);    //search for the second person from the list using findPerson function

if( search_person1.empty() == false && search_person2.empty() == false )  //if both the persons are found on the graph
{
  auto person1 = search_person1.front();     //store the first person object to person1
  auto person2 = search_person2.front();    //store the second person object to person2
  int distance = SocialMediaGraph.distanceBetween( person1, person2 );    //find the distance between the 2 persons using distanceBetween function
  if( distance == 1 ) //if they are directly connected
  {
    std::cout << std::endl << input_firstName << " " << input_lastName << " and " << input_firstName2 << " " << input_lastName2 << " are friends!\n";
  }
  else                //if they are not directly connected
  {
    std::cout << std::endl << input_firstName << " " << input_lastName << " and " << input_firstName2 << " " << input_lastName2 << " have " << distance << " degrees of separation!\n";
  }
}
else  //if both persons are not found on the graph, output to console to let user know and abort
{
  std::cout << "Users not found!" << std::endl;
}

return 0;
}
