#include <iostream>
#include "book.h"
#include <algorithm>

Book::Book(std::string title, std::string author, std::string genre, int year, int id)
    : title(title), author(author), genre(genre), year(year), id(id) {}

void Book::addSpecimen(int id)
{
    Specimen newSpecimen;
    newSpecimen.id = id;
    specimens.push_back(newSpecimen);
}

Specimen* Book::getSpecimen(int id)
{
    for (auto& specimen : specimens)
    {
        if (specimen.id == id)
        {
            return &specimen;
        }
    }
    return nullptr;
}

Specimen* Book::getAvailableSpecimen()
{
    for (auto& specimen : specimens)
    {
        if (!specimen.isrented)
        {
            return &specimen;
        }
    }
    return nullptr;
}

int Book::getId() const
{
    return id;
}

std::string Book::getTitle() const
{
    return title;
}

std::string Book::getAuthor() const
{
    return author;
}

std::string Book::getGenre() const
{
    return genre;
}

int Book::getYear() const
{
    return year;
}

void Book::show_info() const
{
    std::cout << "Title: " << title << "\n";
    std::cout << "Author: " << author << "\n";
    std::cout << "Genre: " << genre << "\n";
    std::cout << "Year: " << year << "\n";
    std::cout << "ID: " << id << "\n";
    std::cout << "Specimens: " << specimens.size() << "\n";
    std::cout << "available specinmens: " << std::count_if(specimens.begin(), specimens.end(), [](const Specimen& s){return !s.isrented;}) << std::endl;
}

bool Book::operator==(const Book& other) const
{
    if(title == other.title && author == other.author && genre == other.genre && year == other.year)
        return true;
    return false;
}