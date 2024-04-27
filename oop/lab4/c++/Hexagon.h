#pragma once
#include "Figure.h"
#include "math.h"

class Hexagon final : public Figure
{
	float side;

public:
	Hexagon(float s) : side{s}
	{}

	float countCircuit()
	{
		return 6 * side;
	}

	float countArea()
	{
		return 1.5 * side * side * sqrt(3);
	}

	virtual std::string getName() override
	{
		return "hexagon";
	}
};

