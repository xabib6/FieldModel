#pragma once
#include "AbstractWell.h"
#include <iostream>
using namespace std;

void AbstractWell::ChangeWorkingStatus()
{
	if (_isWorking)
	{
		_isWorking = false;
		cout << "�������� ���������" << endl;
	}
	else
	{
		_isWorking = true;
		cout << "�������� ��������" << endl;
	}
}



void AbstractWell::PrintWellInfo()
{
	cout << GetWellType() << endl;
	cout << GetWellTotalVolume() << _resourceVolume << endl;
	cout << "���������: " << GetWellState() << endl << endl;
}

string AbstractWell::GetWellState()
{
	if (_isWorking)
		return "��������";
	else
		return "�� ��������";
}





AbstractWell::AbstractWell(int productivity, int daysWorked)
{	
	_productivity = productivity;
	_daysWorked = daysWorked;
	_isWorking = true;
	_resourceVolume = _productivity * _daysWorked;
}


