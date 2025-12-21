#pragma once

#include <string>
#include <vector>

struct Specimen
{
    int id;
    bool isrented = false;
};

class Book
{
private:
    std::string title;
    std::string author;
    std::string genre;
    int year;
    int id;
    std::vector<Specimen> specimens;
public:
    Book() = default;
    Book(std::string title, std::string author, std::string genre, int year, int id);
    void addSpecimen(int id);
    Specimen* getSpecimen(int id);
    Specimen* getAvailableSpecimen();
    int getId() const;
    std::string getTitle() const;
    std::string getAuthor() const;
    std::string getGenre() const;
    int getYear() const;
    void show_info() const;
    bool operator==(const Book& other) const;
};
