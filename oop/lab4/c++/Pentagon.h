#pragma once
#include "Figure.h"
#include "math.h"

class Pentagon final : public Figure
{
	float side;

public:
	Pentagon(float s) : side{s}
	{}

	float countCircuit()
	{
		return 5 * side;
	}

	float countArea()
	{
		return sqrt(25 + 10 * sqrt(5)) * side * side / 4;
	}

	virtual std::string getName() override
	{
		return "pentagon";
	}
};

