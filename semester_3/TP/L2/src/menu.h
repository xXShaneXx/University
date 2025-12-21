#pragma once
#include <iostream>
#include "library.h"

class Menu 
{
private:
    Library library;

public:
    Menu(Library& lib);
    void start();
    void displayMenu();
    int getChoice();
    void handleChoice(int choice);

    void addBook();
    void addSpecimen();
    void removeBook();
    void listBooks();
    void rentBook();
    void returnBook();
    void findBook();
    void addReader();
    void listReaders();
    void showBorrowedBooksForReader();
    void showRentalHistoryForReader();
};