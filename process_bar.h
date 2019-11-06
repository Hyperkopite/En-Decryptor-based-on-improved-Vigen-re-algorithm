#pragma once
#ifndef PROCESS_BAR_H
#define PROCESS_BAR_H

class process_bar
{
private:
	int length_of_bar;

public:
	process_bar(int len);
	void display(int current, int all);
};

#endif