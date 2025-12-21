#include "RowOfPascalTriangle.h"
#include <iostream>

RowOfPascalTriangle::RowOfPascalTriangle(int n): size{n + 1}
{
	if (n < 0)
		throw std::invalid_argument("");

	row = new int[n + 1] {0};
	Initialize();
}

int RowOfPascalTriangle::Count(int index, int r)
{
	if (r == 0 || r == 1)
		return 1;

	if (index == 0)
		return 1;

	if (index == r)
		return 1;

	return Count(index - 1, r - 1) + Count(index, r - 1);
}

void RowOfPascalTriangle::Initialize()
{
	for (int i = 0; i < size; i++)
	{
		row[i] = Count(i, size - 1);
	}
}

int& RowOfPascalTriangle::operator[](int index)
{
	if (index < 0 || index > size - 1)
		throw std::out_of_range("Number out of range");

	return row[index];
}

RowOfPascalTriangle::~RowOfPascalTriangle()
{
	delete[] row;
}