#include <iostream>
#include <vector>
#include "Figure.h"
#include "Rhombus.h"
#include "Hexagon.h"
#include "Pentagon.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Square.h"
#include "Quadrangle.h"

using namespace std;

Figure* createFigure(char name, const vector<float>&parameters);

int main()
{
	char option;
	vector<float> parameters;
	Figure* figure = nullptr;
	float p;
	cin >> option;

	switch (option)
	{
	case 'o':
		cin >> p;
		parameters.push_back(p);
		break;
	case 'p':
	case 's':
		for (int i = 0; i < 2; i++)
		{
			cin >> p;
			parameters.push_back(p);
		}
		break;
	case 'c':
		for (int i = 0; i < 5; i++)
		{
			cin >> p;
			parameters.push_back(p);
		}
		break;
	}

	figure = createFigure(option, parameters);
	cout << figure->getName() << " area= " << figure->countArea() <<
		" circuit=" << figure->countCircuit() << endl;
}

Figure* createFigure(char name, const vector<float>& parameters)
{
	switch (name)
	{
	case 'o':
		return new Circle(parameters[0]);
		break;

	case 'p':
		return new Pentagon(parameters[0]);
		break;

	case 's':
		return new Hexagon(parameters[0]);
		break;

	case 'c':
		if (parameters[4] == 90.0)
		{
			if (parameters[0] == parameters[1] && parameters[1] == parameters[2])
				return new Square(parameters[0]);
			else
				return new Rectangle(parameters[0], parameters[2]);
		}
		else
			return new Rhombus(parameters[0], parameters[4]);
		break;

	default:
		break;
	}
}
