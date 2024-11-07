#include "menu.h"
#include "string"

Menu::Menu(Library& lib) : library(lib)
{}

void Menu::start() 
{
    int choice;
    do 
    {
        displayMenu();
        choice = getChoice();
        handleChoice(choice);
    } while (choice != 11);
}

void Menu::displayMenu() 
{
    std::cout << "Library Menu:\n";
    std::cout << "1. Add Book\n";
    std::cout << "2. Add Specimen\n";
    std::cout << "3. Remove Book\n";
    std::cout << "4. List Books\n";
    std::cout << "5. Rent Book\n";
    std::cout << "6. Return Book\n";
    std::cout << "7. Find Book\n";
    std::cout << "8. Add Reader\n";
    std::cout << "9. List Readers\n";
    std::cout << "10. Show Borrowed Books for Reader\n";
    std::cout << "11. Show Rental History for Reader\n";
    std::cout << "12. Exit\n\n";
}

int Menu::getChoice() 
{
    int choice;
    std::cout << "Enter your choice: ";
    std::cin >> choice;
    return choice;
}

void Menu::handleChoice(int choice) 
{
    switch (choice) {
        case 1:
            addBook();
            break;
        case 2:
            addSpecimen();
            break;
        case 3:
            removeBook();
            break;
        case 4:
            listBooks();
            break;
        case 5:
            rentBook();
            break;
        case 6:
            returnBook();
            break;
        case 7:
            findBook();
            break;
        case 8:
            addReader();
            break;
        case 9:
            listReaders();
            break;
        case 10:
            showBorrowedBooksForReader();
            break;
        case 11:
            showRentalHistoryForReader();
            break;
        case 12:
            std::cout << "Exiting...\n";
            break;
        default:
            std::cout << "Invalid choice. Please try again.\n";
            break;
    }
}

void Menu::addBook() 
{
    std::string title, author, genre;
    int year;
    std::cout << "Enter title: ";
    std::cin >> title;
    std::cout << "Enter author: ";
    std::cin >> author;
    std::cout << "Enter genre: ";
    std::cin >> genre;
    std::cout << "Enter year: ";
    std::cin >> year;
    library.addBook(title, author, genre, year);
}

void Menu::addSpecimen() 
{
    int bookId;
    std::cout << "Enter book ID to add specimen: ";
    std::cin >> bookId;
    library.addSpecimen(bookId);
}

void Menu::removeBook() 
{
    int bookId = -1;
    std::cout << "Enter book ID to remove: ";
    std::cin >> bookId;
    library.removeBook(bookId);
}

void Menu::listBooks() 
{
    library.showBooks();
}

void Menu::rentBook() 
{
    int readerId, bookId;
    std::cout << "Enter reader ID: ";
    std::cin >> readerId;
    std::cout << "Enter book ID: ";
    std::cin >> bookId;
    if (library.rentBook(readerId, bookId)) {
        std::cout << "Book rented successfully.\n";
    } else {
        std::cout << "Failed to rent book.\n";
    }
}

void Menu::returnBook() 
{
    int readerId, specimenId;
    std::cout << "Enter reader ID: ";
    std::cin >> readerId;
    std::cout << "Enter specimen ID: ";
    std::cin >> specimenId;
    if (library.returnBook(readerId, specimenId)) {
        std::cout << "Book returned successfully.\n";
    } else {
        std::cout << "Failed to return book.\n";
    }
}

void Menu::findBook() 
{
    std::string title, genre, author;
    int year = 0;
    std::cout << "Enter title (or leave empty): ";
    std::cin.ignore();
    std::getline(std::cin, title);
    std::cout << "Enter genre (or leave empty): ";
    std::getline(std::cin, genre);
    std::cout << "Enter year (or leave empty): ";
    std::cin >> year;
    std::cin.ignore();
    std::cout << "Enter author (or leave empty): ";
    std::getline(std::cin, author);
    auto books = library.findBooks(title, genre, year, author);

    if(books.empty()) 
    {
        std::cout << "No books found.\n";
        return;
    }

    for (const auto& bookId : books) 
    {
        auto book = library.getBookById(bookId);
        if (&book) {
            book.show_info();
        }
    }
}

void Menu::addReader() 
{
    std::string name;
    std::string surname;
    int age;
    std::cout << "Enter reader name: ";
    std::cin >> name;
    std::cout << "Enter reader surname: ";
    std::cin >> surname;
    std::cout << "Enter reader age: ";
    std::cin >> age;
    library.addReader(name, surname, age);
}

void Menu::listReaders() 
{
    library.showReaders();
}

void Menu::showBorrowedBooksForReader() 
{
    int readerId;
    std::cout << "Enter reader ID: ";
    std::cin >> readerId;
    library.showBorrowedBooksForReader(readerId);
}

void Menu::showRentalHistoryForReader() 
{
    int readerId;
    std::cout << "Enter reader ID: ";
    std::cin >> readerId;
    library.showRentalHistoryForReader(readerId);
}