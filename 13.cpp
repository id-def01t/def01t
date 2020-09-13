#include "13.h"
#include <iostream>
#include <vector>

Line* input(int& rm)
{
	Line* lines = nullptr;
	int m, n;	
	const char* pr = "";
	do
	{	
		std::cout << pr << std::endl;
		std::cout << "Enter a number of lines: ";
		pr = "Repeat pls";
		if (inputNum(m) < 0)
		{
			return nullptr;
		}
	} 
	while (m < 1);
	const char* pr1 = "";
	do
	{	
		std::cout << pr1 << std::endl;
		std::cout << "Enter a number of columns: ";
		pr1 = "Repeat pls";
		if (inputNum(n) < 0)
		{
			return nullptr;
		}
	} 
	while (n < 1);
	try
	{
		lines = new Line[m];
	}
	catch (std::bad_alloc& ba)
	{
		std::cout << "------ too many rows in matrix: " << ba.what() << std::endl;
		return nullptr;
	}
	const char* pr2 = "";
	for (int i = 0; i < m; i++)
	{
		lines[i].m = m;		// stroki
		lines[i].n = n;		// stolbiki
		pr2 = "";
		try
		{
			lines[i].a = new int[lines[i].n];
		}
		catch (std::bad_alloc& ba)
		{
			std::cout << "------ too many items in matrix: " << lines[i].n << ba.what() << std::endl;
			return nullptr;
		}
		std::cout << "Enter items for " << (i + 1) << " line: " << std::endl;
		for (int j = 0; j < n; j++)
		{
			if (inputNum(lines[i].a[j]) < 0)
			{
				erase(lines, i + 1);
				return nullptr;
			}
		}
	}
	rm = m;
	return lines;
}

Line* erase(Line*& lines, int m)
{
	for (int i = 0; i < m; i++)
	{
		delete[] lines[i].a;
	}
	delete[] lines;
	return nullptr;
}

void output(const char* msg, Line* a)
{
	std::cout << msg << ":\n";
	for (int i = 0; i < a->m; i++)
	{
		for (int j = 0; j < a->n; j++)
		{
			if (j == a->n - 1)
			{
				std::cout << a[i].a[j];
			}
			else
			{
				std::cout << a[i].a[j] << ", ";
			}
		}
		std::cout << std::endl;
	}
}

void solve(Line* a)
{
	a = sort(a);
	std::vector <int> vc(a->m);
	for (int t = 0; t < a->m; t++)
	{
		for (int i = 0; i < a->n - 1; i++)
		{	
			if (a[t].a[i] != a[t].a[i + 1])
			{
				vc[t]++;
			}
			if (a[t].a[0] == a[t].a[a->n - 1])
			{
				vc[t] = -1;
			}
		}
		vc[t] += 1;
	}
	std::cout << "This is the vector: ";
	for (int i = 0; i < a->m; i++)
	{
		if (i == a->m - 1)
		{
			std::cout << vc[i];
		}
		else
		{
			std::cout << vc[i] << ", ";
		}
	}
}

Line* sort(Line* a)
{
	for (int t = 0; t < a->m; t++)
	{
		for (int j = 1; j < a->n; j++)
		{
			for (int i = 0; i < a->n - 1; i++)
			{
				if (a[t].a[i] > a[t].a[i + 1])
				{
					int x = a[t].a[i];
					a[t].a[i] = a[t].a[i + 1];
					a[t].a[i + 1] = x;
				}
			}
		}
	}
	return a;
}