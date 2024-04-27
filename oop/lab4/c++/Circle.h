#pragma once
#include "Figure.h"

class Circle final: public Figure
{
	float r;
public:
	Circle(float rr):r{rr}
	{}

	float countCircuit()
	{
		return 2 * 3.14 * r;
	}

	float countArea() 
	{
		return 3.14 * r * r;
	}

	virtual std::string getName() override
	{
		return "circle";
	}
};

