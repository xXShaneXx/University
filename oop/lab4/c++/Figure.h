#pragma once
#include<string>

class Figure
{
public:
	virtual float countArea() = 0;
	virtual float countCircuit() = 0;
	virtual std::string getName() = 0;
};
