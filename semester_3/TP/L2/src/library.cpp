#include "library.h"
#include <iostream>
#include <iomanip>
#include <ctime>


Library::Library() : nextBookId(0), nextSpecimenId(0), nextReaderId(0) {}

void Library::addReader(std::string name, std::string surname, int age)
{
    readers[nextReaderId] = Reader(name, surname, age, nextReaderId);
    nextReaderId++;
}

void Library::showReaders() const 
{
    for (const auto& reader : readers) 
        reader.second.show_info();
}

void Library::addBook(const std::string& title, const std::string& author, const std::string& genre, int year) 
{
    Book newBook(title, author, genre, year, nextBookId);
    for (const auto& [id, book] : books) 
    {
        if (book == newBook) 
        {
            std::cout << "Book already exists\n";
            return;
        }
    }
    books[nextBookId++] = newBook;
}

void Library::removeBook(int bookId) 
{
    if (books.find(bookId) != books.end())
        books.erase(bookId);
    else
        std::cout << "Book ID not found\n";
}

void Library::addSpecimen(int bookId) 
{
    auto bookIt = books.find(bookId);
    if (bookIt != books.end())
        bookIt->second.addSpecimen(nextSpecimenId++);
    else 
        std::cout << "Book ID " << bookId << " not found\n";
}

void Library::showBooks() const 
{
    for (const auto& book : books)
        book.second.show_info();
}

const Book& Library::getBookById(int bookId) const 
{
    return books.at(bookId);
}

std::vector<int> Library::findBooks(const std::string& title, const std::string& genre, int year, const std::string& author) const {
    std::vector<int> foundBooks;
    for (const auto& [id, book] : books) 
    {
        bool matches = true;
        if (!title.empty() && book.getTitle() != title) matches = false;
        if (!genre.empty() && book.getGenre() != genre) matches = false;
        if (year != 0 && book.getYear() != year) matches = false;
        if (!author.empty() && book.getAuthor() != author) matches = false;
        if (matches) foundBooks.push_back(id);
    }
    return foundBooks;
}

// Implementacja RentalRecord
void Library::RentalRecord::addRent(int bookId, int specimenId, std::time_t rentalDate) 
{
    rents.push_back({ bookId, specimenId, rentalDate });
}

void Library::RentalRecord::setReturnDate(int specimenId, std::time_t returnDate) {
    for (auto& rent : rents) 
    {
        if (rent.specimenId == specimenId && rent.returnDate == 0) 
        {
            rent.returnDate = returnDate;
            return;
        }
    }
}

Library::Rent* Library::RentalRecord::findRent(int specimenId) 
{
    for (auto& rent : rents) 
    {
        if (rent.specimenId == specimenId && rent.returnDate == 0)
            return &rent;
    }
    return nullptr;
}

// Wypożyczenie książki
bool Library::rentBook(int readerId, int bookId) 
{
    auto readerIt = readers.find(readerId);
    if (readerIt == readers.end()) 
    {
        std::cout << "Invalid reader ID.\n";
        return false;
    }

    auto bookIt = books.find(bookId);
    if (bookIt == books.end()) 
    {
        std::cout << "Invalid book ID.\n";
        return false;
    }

    Specimen* specimen = bookIt->second.getAvailableSpecimen();
    if (!specimen) 
    {
        std::cout << "No available specimens.\n";
        return false;
    }

    int specimenId = specimen->id;

    rentalRecords[readerId].addRent(bookId, specimenId, std::time(nullptr));
    specimen->isrented = true;
    std::cout << "Book rented: " << bookIt->second.getTitle() << ", Specimen ID: " << specimenId << "\n";
    return true;
}

// Zwrot książki
bool Library::returnBook(int readerId, int specimenId) 
{
    auto readerIt = rentalRecords.find(readerId);
    if (readerIt == rentalRecords.end()) 
    {
        std::cout << "Reader has no rentals.\n";
        return false;
    }

    Rent* rent = readerIt->second.findRent(specimenId);
    if (!rent) 
    {
        std::cout << "Specimen ID not found in rentals.\n";
        return false;
    }

    int bookId = rent->bookId;
    auto bookIt = books.find(bookId);
    if (bookIt == books.end()) 
    {
        std::cout << "Book ID " << bookId << " not found.\n";
        return false;
    }

    Specimen* specimen = bookIt->second.getSpecimen(specimenId);
    if (!specimen) 
    {
        std::cout << "Specimen ID " << specimenId << " not found in book.\n";
        return false;
    }

    specimen->isrented = false;
    readerIt->second.setReturnDate(specimenId, std::time(nullptr));
    std::cout << "Book returned: " << bookIt->second.getTitle() << ", Specimen ID: " << specimenId << "\n";
    return true;
}

void Library::showBorrowedBooksForReader(int readerId) const 
{
    auto it = rentalRecords.find(readerId);
    if (it == rentalRecords.end() || it->second.rents.empty()) 
    {
        std::cout << "Reader has no borrowed books.\n";
        return;
    }

    std::cout << "Borrowed books for reader ID " << readerId << ":\n";
    for (const auto& rent : it->second.rents) 
    {
        if (rent.returnDate == 0) { // Sprawdza, czy książka nie została zwrócona
            const Book& book = books.at(rent.bookId);
            std::cout << "Title: " << book.getTitle() 
                      << ", Specimen ID: " << rent.specimenId 
                      << ", Borrowed on: " << std::ctime(&rent.rentalDate);
        }
    }
}

void Library::showRentalHistoryForReader(int readerId) const 
{
    auto it = rentalRecords.find(readerId);
    if (it == rentalRecords.end() || it->second.rents.empty()) 
    {
        std::cout << "Reader has no rental history.\n";
        return;
    }

    std::cout << "Rental history for reader ID " << readerId << ":\n";
    for (const auto& rent : it->second.rents) 
    {
        const Book& book = books.at(rent.bookId);

        std::cout << "Title: " << book.getTitle()
                  << ", Specimen ID: " << rent.specimenId
                  << ", Borrowed on: " << std::put_time(std::localtime(&rent.rentalDate), "%Y-%m-%d %H:%M:%S");
        
        if (rent.returnDate != 0) 
            std::cout << ", Returned on: " << std::put_time(std::localtime(&rent.returnDate), "%Y-%m-%d %H:%M:%S");
        else 
            std::cout << ", Not yet returned";
        std::cout << '\n';
    }
}

