#include <iostream>       //showpoint(), fixed()
#include <iomanip>        //setprecision()
#include <vector>
#include <memory>         //unique_ptr
#include "GroceryItem.hpp"

int main()
{
  std::cout << std::showpoint << std::fixed << std::setprecision(2)
            << "Welcome to Kroger.  Place grocery items into your shopping cart by entering each item's information.\n"
            << "  enclose string entries in quotes, separate fields with comas\n"
            << "  for example:  \"00016000306707\", \"Betty Crocker\", \"Betty Crocker Double Chocolate Chunk Cookie Mix\", 17.19\n"
            << "  Enter CTL-Z (Windows) or CTL-D (Linux) to quit\n\n";
  
  //Create an empty collection of pointers-to-grocery items
  std::vector<std::unique_ptr<GroceryItem>> shoppingCart;


  //Prompt and input each item by the user until end of file
  GroceryItem item;

  while (std::cout << "Enter UPC, Product Brand, Product Name, and Price\n", std::cin >> item)
  {
    shoppingCart.emplace_back(std::make_unique<GroceryItem>(std::move(item)));
    std::cout << "Items added to shopping cart: " << *shoppingCart.back() << "\n\n";
  }

  //Finish input items until end of file, now display them in reverse order to std::cout
  //Use constant iterators and reverse iterators
  std::cout <<"\n\nHere is an itemized list of the items in your shopping cart: \n";
  for (auto i = shoppingCart.crbegin(); i < shoppingCart.crend(); ++i)
  {
    std::cout << **i << "\n";
  }
  
  return 0;
  
}
