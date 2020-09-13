#include <iostream>
#include "13.h"

int main()
{
	Line* arr = nullptr;
	int m;
	arr = input(m);
	if (!arr)
	{
		std::cout << "Incorrect data" << std::endl;
		return 1;
	}
	output("Sourced matrix", arr);
	solve(arr);
	arr = erase(arr, m);
	return 0;
}