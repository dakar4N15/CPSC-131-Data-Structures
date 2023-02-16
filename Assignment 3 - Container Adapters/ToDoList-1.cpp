/*
It is your job to implement this file in its entirety. 

The ToDoList.hpp file and main.cpp file have all the information you need.

Good luck.
*/
#include "ToDoList.hpp"
#include <iostream>
#include <queue>
#include <stack>

//Private
void ToDoList::_announce( std::string verb, std::string & msg)  //output what is being done to console
{
    std::cout << verb << "Task: [" << msg << "]\n";
}

void ToDoList::_warn( std::string verb )    //warn the user of incorrect input to console
{
    std::cout << "Nothing to " << verb << "!\n";
}


//Public

//default constructor
ToDoList::ToDoList() = default;

//List-initialized constructor
ToDoList::ToDoList( const std::initializer_list<std::string> & tasks )
{
    for (auto i : tasks)
    {
        Add (i);
    }
}

//Actional
void ToDoList::Add( std::string newItem )   //add a new item to the todo list
{
    _todo.push( newItem );
}

void ToDoList::DoNext()     //Do the next thing on the ToDo list
{
    //check if todo list is empty, if empty, tell the user that there is nothing to do next and return
    if( _todo.empty())
    {
        _warn ( "do" );
        return;
    }
    //add current thing at the front of the ToDo list to the undo list, so that the user can undo it if desired
    _undo.push( _todo.front() );
    //make sure redo list is empty, because no undo statement is initiated
    _redo = std::stack<std::string>();
    //output to console that already did the action on the front of ToDoList
    _announce( "Did", _todo.front());
    //delete the current action at the front of ToDo list so that we can move on to the next item
    _todo.pop();
}

void ToDoList::Undo()       //undo a task from the undo list and put it in the redo list
{
    //check if undo list is empty, if empty, tell the user that there is nothing to undo and return
    if ( _undo.empty() )
    {
        _warn ( "undo" );
        return;
    }
    //output to console that already undid the action on the top of the undo list
    _announce( "Undid", _undo.top() );
    //add current action at the top of the undo list to the redo list, so that the user can redo it if desired
    _redo.push( _undo.top() );
    //delete the current action at the top of the undo list
    _undo.pop();
}

void ToDoList::Redo()   //redo the last action that was undone, if accessible
{
    //check if redo list is empty, if empty, tell the user that there is nothing to redo and return
    if( _redo.empty() )
    {
        _warn ("redo");
        return;
    }
    //output to console that already redid the action that the user has previously undid, located at the top of the redo list
    _announce( "Redid", _redo.top() );
    //add the action back to undo list, so that the user can undo it again if desired
    _undo.push( _redo.top() );
    //delete the current action at the top of the redo list
    _redo.pop();
}

void ToDoList::Command(std::string command)     //accept text input to perform one of DoNext/Undo/Redo functions
{
    if( command == "DoNext" )       //if the text input is DoNext, then execute the function DoNext and return
    {
        DoNext();
        return;
    }
    if( command == "Undo" )         //if the text input is Undo, then execute the function Undo and return
    {
        Undo();
        return;
    }
    if( command == "Redo" )         //if the text input is Redo, then execute the function Redo and return
    {
        Redo();
        return;
    }
    throw std::invalid_argument( "Command Not Found" );     //if the text input is neither from those 3 functions above, throw an invalid argument exception and abort program
}


//Informational
bool ToDoList::isEmpty()       //Tell us if the todo list is empty or not, returns a boolean
{   
    return _todo.empty();      //returns true if there is no actions left to do in todo list, returns false if there is still actions left to do in todo list
}

std::size_t ToDoList::size()    //Tell us the size of the todo list
{
    return _todo.size();        //returns the number of actions left in the todo list
}

