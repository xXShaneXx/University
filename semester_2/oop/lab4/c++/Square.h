#pragma once
#include "Quadrangle.h"

class Square final: public Quadrangle
{
	float a;
public:
	Square(float aa) : a{aa}
	{}

	float countArea() override
	{
		return  a * a;
	}

	float countCircuit() override
	{
		return 4 * a;
	}

	virtual std::string getName() override
	{
		return "square";
	}
};

