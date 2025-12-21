#include "reader.h"
#include <iostream>
#include <string>

Reader::Reader(std::string name, std::string surname, int age, int id):
    name(name), surname(surname), age(age), id(id) {}

void Reader::show_info() const
{
    std::cout << "ID: " << id << "\n";
    std::cout << "Name: " << name << "\n";
    std::cout << "Surname: " << surname << "\n";
    std::cout << "Age: " << age << "\n";
}

int Reader::getId() const
{
    return id;
}