#pragma once

#include <string>

class Reader
{
private:
    std::string name;
    std::string surname;
    int age;
    int id;
public:
    Reader() = default;
    Reader(std::string name, std::string surname, int age, int id);
    void show_info() const;
    int getId() const;
};