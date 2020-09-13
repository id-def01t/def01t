#pragma once
#include <iostream>

struct Line
{
	int n, m;	// m kolvo strok n kolvo stolbov
	int* a;		// massiv elmov stroki
};

template <class T>

int inputNum(T& a)
{
	std::cin >> a;
	if (!std::cin.good())	// proverka na norm vvod
	{
		return -1;
	}
	return 1;
}

Line* input(int&);
Line* erase(Line*&, int);
void output(const char*, Line*);
void solve(Line*);
Line* sort(Line*);