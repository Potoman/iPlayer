

//#include "Player.h"

#include <iostream>
/*
*/

#include "Player.h"
/*
  */


struct Foo {
    std::string n;
    Foo() {
       std::cout << "Enter n: "; // no flush needed
       std::cin >> n;
    }
};

int main() {

  Player p;

  p.process();

  return 0;
}
