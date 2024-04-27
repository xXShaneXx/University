#include <iostream>
#include <string>
#include "RowOfPascalTriangle.h"

using namespace std;

int main(int argc, char* argv[])
{
	try
	{
		RowOfPascalTriangle row(stoi(argv[1]));

		for (int i = 2; i < argc; i++)
		{
			try
			{
				cout << argv[i] << " - ";
				cout << row[stoi(argv[i])] << endl;
			}
			catch(out_of_range& e)
			{
				cout << e.what() << endl;
			}
			catch (invalid_argument& e)
			{
				cout << "Incorrect input" << endl;
			}
		}
	}
	catch (invalid_argument& e)
	{
		cout << argv[1] << " - " << "Incorrect row number" << endl;
	}
}
