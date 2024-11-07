#pragma once

class RowOfPascalTriangle
{
	int* row;
	const int size;

	int Count(int index, int r);

public:
	RowOfPascalTriangle(int n);
	
	void Initialize();

	~RowOfPascalTriangle();

	int& operator[](int index);
};
