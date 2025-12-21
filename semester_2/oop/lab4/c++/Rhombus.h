#pragma once
#include "Quadrangle.h"
#include "math.h"

class Rhombus final : public Quadrangle
{
	double b;
	double a;

public:
	Rhombus(float bb, float aa): a{aa}, b{bb}
	{}


	float countArea() override
	{
		return  b * b * sin(a * 3.14 / 180.0);
	}

	float countCircuit() override
	{
		return 4 * b;
	}

	virtual std::string getName() override
	{
		return "rhombus";
	}
};

