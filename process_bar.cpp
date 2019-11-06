#include <iostream>
#include <iomanip>
#include "process_bar.h"

using namespace std;

process_bar::process_bar(int len)
{
	this->length_of_bar = len;
}

void process_bar::display(int current, int all)
{
	double ratio = double(current) / (double)all;
	cout << " [";
	for (int i = 0; i < int(ratio * length_of_bar); i++)
	{
		cout << '=';
	}
	for (int j = 0; j < length_of_bar - int(ratio * length_of_bar); j++)
	{
		cout << ' ';
	}
	cout << "] %" << fixed << setprecision(1) << ratio * 100;

	if (current == all)
	{
		cout << endl;
	}
	else
	{
		cout << '\r';
	}
}

