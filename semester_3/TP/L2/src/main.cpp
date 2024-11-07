#include <iostream>
#include "menu.h"
#include "library.h"
using namespace std;

int main() 
{
    Library library;
    Menu menu(library);
    menu.start();
    return 0;
}