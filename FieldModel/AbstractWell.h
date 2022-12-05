#pragma once
#include <iostream>
using namespace std;

class AbstractWell
{
protected:
	int _resourceVolume;
	bool _isWorking;	
	int _daysWorked;
	
public:
	int _productivity;
	void ChangeWorkingStatus();	
	void PrintWellInfo();
	string GetWellState();
	virtual string GetWellTotalVolume() = 0;
	virtual string GetWellType() = 0;
	AbstractWell(int productivity, int daysWorked);
};