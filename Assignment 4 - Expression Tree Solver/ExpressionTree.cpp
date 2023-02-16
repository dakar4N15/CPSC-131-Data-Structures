#include <algorithm>
#include <forward_list>
#include <iostream>
#include <string>

#include "ExpressionTree.hpp"
#include "ExpressionFixer.hpp"

// The subclass TreeNode has been implemented for you
// This should provide vital clues as to how this program should work. 
ExpressionTree::TreeNode::TreeNode()
{
  this->left  = NULL;
  this->right = NULL;
};

ExpressionTree::TreeNode::TreeNode( char inSymbol, int inNumber ) : number{ inNumber }, symbol{ inSymbol }, left{ NULL }, right{ NULL } {};

std::string ExpressionTree::TreeNode::value()
{
  if( isOperator() )
  {
    return std::string( 1, symbol );
  }
  else
  {
    return std::to_string( number );
  }
};

bool ExpressionTree::TreeNode::isLeaf()
{
  return this->left == NULL && this->right == NULL;
};
bool ExpressionTree::TreeNode::isOperator()
{
  return symbol != ' ';
};
bool ExpressionTree::TreeNode::isOperand()
{
  return symbol == ' ';
};



ExpressionTree::ExpressionTree( std::string infixExpression )
{
  // IMPLEMENT
  std::string _prefix;
  _prefix = _fixer.infixToPrefix( infixExpression );
  _buildTree( _prefix );
  _originalExpression = infixExpression;
};



void ExpressionTree::_buildTree( std::string prefixExpression )
{
  for( int i = static_cast<int>( prefixExpression.length() - 1 ); i >= 0; i-- )
  {
    // Check for operator and insert a new node if you find one
    if ( _isOperator( prefixExpression[i] ))
    {
      _insert( prefixExpression[i], 0 );
    }

    // If not, check for operands: 
    else
    {
      std::string accumulatorVariable;
      for( int j = i; j >= 0; j-- )
      {
        // Check for multi-digit operands and "accumulate" them into a string
        if( _isOperand( prefixExpression[j] ))
        {
          accumulatorVariable.push_back( prefixExpression[j] );
          i--;
        }
        else
        {
          break;
        }
      }
      // Reverse the string, convert to an int and add it as a new node
      std::reverse( accumulatorVariable.begin(), accumulatorVariable.end() );
      int a = std::stoi( accumulatorVariable );
      _insert( ' ', a );
    }
  }
};

void ExpressionTree::_insert( char inSymbol, int inNumber )
{
  // Create a new TreeNode
  TreeNode *nptr = new TreeNode( inSymbol, inNumber );
  // If it's an operator, pop the top 2 nodes off the stack and
  // make them the left/right pointers
  if( inSymbol != ' ')
  {
    nptr->left  = pop();
    nptr->right = pop();
     // Then add this to the top of the stack
    push( nptr );
  }
  // Otherwise just add it right on top if it's an operand. 
  else
  {
    push( nptr );
  }
};

// function to check if operand
bool ExpressionTree::_isOperand( char ch )
{
  // Return true if the character is a number (or a letter but we aren't using variables for this assignment)
  return ch >= '0' && ch <= '9';
};
// function to check if operator
bool ExpressionTree::_isOperator( char ch )
{
  // return true if the character is one of the math symbols we can use (+-/*)
  return ch == '+' || ch == '-' || ch == '*' || ch == '/';
};

std::string ExpressionTree::_getInFix( TreeNode * ptr, std::string output )
{
  // output is an accumulator variable, we're adding to it with each pass to build the final string. 
  // Check if the ptr is NULL, if not, let's continue
  // If we're on an operator AND it's a leaf, let's manually add a "(" to the output
  // Set the output to equal the result of this function again but with the left value as the pointer
  // Add the value of this pointer (hint: it's a string function the TreeNode has)
  // Set the output to requal the result of this function AGAIN but with the right value as the pointer
  // If we're on an operator and a leaf, let's add ")" to the output again
  // If the pointer was null, just return output
  if( ptr==NULL )
  {
    return std::string( "" );
  }

  if( ptr->isOperator() )
  {
    std::string left_value  = _getInFix( ptr->left, output );
    std::string right_value = _getInFix( ptr->right, output );
    if( ptr == top() )
    {
      return left_value + ptr->symbol + right_value;
    }
    return std::string( "(" ) + left_value + ptr->symbol + right_value + std::string( ")" );
  }
  else
  {
    return std::to_string( ptr->number );
  }
}

float ExpressionTree::_doMath( char type, float left, float right )
{
  // Use the char value to determine what type of operation to do. 
  // Use the numbers provided to do that operation
  // I recommend a switch statement. If your arguments are '*', 2 and 3 then the return value should be 6. 
  float result;
  switch( type )
  {
    case '+':
      result = left + right;
      break;
    case '-':
      result = left - right;
      break;
    case '*':
      result = left * right;
      break;
    case '/':
      result = left / right;
      break;
    default:
      result = left + right;
      break;
  }
  return result;
};

float ExpressionTree::_solve( TreeNode * ptr )
{
  // Recursive Function Alert!
  // If the ptr is NULL, just return 0. 
  // If the pointer is a leaf, convert it's "number" property to a float and return it. 
  // Create a "left value" float = to the result of this function on the ptr's "left" pointer. 
  // Create a "right value" float = to the result of this function on the ptr's "right" pointer. 
  // Return the result of _doMath() using the ptr's symbol and the left/right value floats you made earlier. 
  if( ptr == NULL )
  {
    return 0;
  }
  else
  {
    if( ptr->isLeaf() )
    {
      float a = static_cast< float >( ptr->number ) ;
      return a;
    }
    else
    {
      float left_value  = _solve( ptr->left );
      float right_value = _solve( ptr->right );
      return _doMath( ptr->symbol, left_value, right_value );
    }
  }
};


std::string ExpressionTree::infixExpression()
{
  // Just return the value the user input
  return _originalExpression;
};

std::string ExpressionTree::prefixExpression()
{
  // Use the ExpressionFixer to convert the original input into prefix format
  return _fixer.infixToPrefix( _originalExpression );
};

std::string ExpressionTree::postfixExpression()
{
  // Use the ExpressionFixer to convert the original input into postfix format
  return _fixer.infixToPostfix( _originalExpression );
};

void ExpressionTree::push( TreeNode * ptr )
{
  // Push a tree pointer onto the stack
  _nodeStack.push_front( ptr );
};
ExpressionTree::TreeNode * ExpressionTree::pop()
{
  // Pop a tree pointer off the stack and return it
  TreeNode *frnt = _nodeStack.front();
  _nodeStack.pop_front();
  return frnt;
};

ExpressionTree::TreeNode * ExpressionTree::top()
{
  // Return a reference to the top pointer on the stack
  return _nodeStack.front();
};

std::string ExpressionTree::getInFix()
{
  // Just call the _getInfix() function with the top node and an empty string as the arguments. 
  return _getInFix( top(), " " );
};

float ExpressionTree::solve()
{
  // Just return the _solve() function with the top node as the argument. 
  return _solve( top() );
};
