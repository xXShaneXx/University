#pragma once
#include "Quadrangle.h"

class Rectangle final : public Quadrangle
{
	float a;
	float b;
public:
	Rectangle(float aa, float bb): a{aa}, b{bb}
	{}

	float countArea() override
	{
		return  a * b;
	}

	float countCircuit() override
	{
		return 2 * a + 2 * b;
	}

	virtual std::string getName() override
	{
		return "rectangle";
	}
};
