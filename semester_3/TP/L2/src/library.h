#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <ctime>
#include "reader.h"
#include "book.h"

class Library
{
private:
    std::unordered_map<int, Reader> readers;
    std::unordered_map<int, Book> books;
    int nextBookId;         // Do generowania unikalnych ID dla książek
    int nextSpecimenId;     // Do generowania unikalnych ID dla egzemplarzy
    int nextReaderId;       // Do generowania unikalnych ID dla czytelników

    struct Rent 
    {
        int bookId;
        int specimenId;
        std::time_t rentalDate;
        std::time_t returnDate = 0;
    };

    class RentalRecord 
    {
    public:
        std::vector<Rent> rents;
        void addRent(int bookId, int specimenId, std::time_t rentalDate);
        void setReturnDate(int specimenId, std::time_t returnDate);
        Rent* findRent(int specimenId);
    };

    std::unordered_map<int, RentalRecord> rentalRecords;

public:
    Library();
    void addReader(std::string name, std::string surname, int age);
    void showReaders() const;
    void showBorrowedBooksForReader(int readerId) const;
    void showRentalHistoryForReader(int readerId) const;
    void addBook(const std::string& title, const std::string& author, const std::string& genre, int year);
    void removeBook(int bookId);
    void addSpecimen(int bookId);
    void showBooks() const;
    const Book& getBookById(int bookId) const;
    std::vector<int> findBooks(const std::string& title = "", const std::string& genre = "", int year = 0, const std::string& author = "") const;
    bool rentBook(int readerId, int bookId);
    bool returnBook(int readerId, int specimenId);
};
