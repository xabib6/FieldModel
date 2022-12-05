#pragma once
#include "Field.h"
#include <string>
#include <iostream>

Field* Field::_instance = nullptr;

Field Field::GetInstance()
{
	if (_instance == nullptr)
	{
		_instance = new Field();
	}
	return *_instance;
}

void Field::FieldManager()
{
	int input;
	while (true)
	{
		cout << "=====================================" << endl;

		cout << "������ ������: \n1)�������� ��� ��������. \n2)�������� ����� ��������. \n" <<
			"3)������� ��������. \n4)������� ��������. \n5)�������� ����� �� �������������. \n" <<
			"6)��������� ������. " << endl;

		cout << "=====================================" << endl << endl;

		cin >> input;
		switch (input)
		{
		case 1:
			ShowAllWell();
			break;
		case 2:
			AddWell();
			break;
		case 3:
			cout << "����� �������� �������?" << endl;
			cin >> input;
			RemoveWell(input);
			break;
		case 4:
			cout << "����� �������� �������?" << endl;
			cin >> input;
			SelectWell(input);
			break;
		case 5:
			ShowFieldReport();
			break;
		case 6:
			return;
		default:
			cout << "����� ������� �� ����������!" << endl;
		}

	}
}


void Field::ShowAllWell()
{
	if (_wellCount != 0)
	{
		cout << "������� � ������ ������: " << _wellCount << endl << endl;
		for (int i = 0; i < _wellCount; i++)
		{
			cout << i + 1 << ")";
			_wellList[i]->PrintWellInfo();
		}
	}
	else
		cout << "������ �� ������������� ��� �������. " << endl;
}

void Field::SelectWell(int selectedWell)
{
	if (_wellCount == 0)
	{
		cout << "� ������ ������ ��� �������. " << endl;
		return;
	}
	if (selectedWell > _wellCount || selectedWell < 1)
	{
		cout << "�������� � ����� ������� �� ����������!" << endl;
		return;
	}
	_wellList[selectedWell - 1]->PrintWellInfo();

	int productivity, daysWorked, choose;
	cout << "1)��������\\��������� ��������. \n2)��������� � ����." << endl;
	cin >> choose;

	switch (choose)
	{
	case 1:
		ProductivitySubstraction(_wellList[selectedWell - 1], _wellList[selectedWell - 1]->GetWellState() == "��������");
		_wellList[selectedWell - 1]->ChangeWorkingStatus();
		cout << endl;
		break;
	case 2:
		break;
	default:
		cout << "����� ������� �� ����������! " << endl;
		return;
	}
}

void Field::AddWell()
{
	int wellType, productivity, daysWorked;
	cout << "�������� ��� ��������: \n1)�������		\n2)��������	\n3)��������������" << endl;
	cin >> wellType;
	if (wellType != 1 && wellType != 2 && wellType != 3)
	{
		cout << "������ ���� �������� �� ����������!" << endl;
		return;
	}
	cout << "������� ������������������ ��������, �3/�����. " << endl;
	cin >> productivity;
	cout << "������� ���������� ������������ ����. " << endl;
	cin >> daysWorked;

	AbstractWell* well;
	switch (wellType)
	{
	case 1:
		if (productivity * daysWorked > _gasVolume)
		{
			cout << "���������� �������� ���� ��������� �������. ������� ��������� �����." << endl;
			AddWell();
			return;
		}
		well = new GasWell(productivity, daysWorked);
		_gasVolume -= productivity * daysWorked;
		_gasMined += productivity * daysWorked;
		break;
	case 2:
		if (productivity * daysWorked > _oilVolume)
		{
			cout << "���������� ������� ����� ��������� �������. ������� ��������� �����." << endl;
			AddWell();
			return;
		}
		well = new OilWell(productivity, daysWorked);
		_oilVolume -= productivity * daysWorked;
		_oilMined += productivity * daysWorked;
		break;
	case 3:
		well = new InjectionWell(productivity, daysWorked);
		_waterVolume += productivity * daysWorked;
		break;
	default:
		return;
	}
	ProductivitySubstraction(well, false);
	AbstractWell** newWellList = new AbstractWell * [_wellCount + 1];
	for (int i = 0; i < _wellCount; i++)
	{
		newWellList[i] = _wellList[i];
	}
	newWellList[_wellCount] = well;

	_wellCount++;
	delete[] _wellList;
	_wellList = newWellList;

	cout << well->GetWellType() << "���������." << endl;
}

void Field::RemoveWell(int deleteWell)
{
	if (deleteWell > _wellCount || deleteWell < 1)
	{
		cout << "�������� � ����� ������� �� ����������!" << endl;
		return;
	}
	ProductivitySubstraction(_wellList[deleteWell-1]);

	int removal = 0;
	AbstractWell** newWellList = new AbstractWell * [_wellCount - 1];
	for (int i = 0; i < _wellCount - 1; i++)
	{
		removal = 0;
		if (i == deleteWell - 1)
			removal = 1;
		newWellList[i] = _wellList[i+removal];
	}

	_wellCount--;
	delete[] _wellList;
	_wellList = newWellList;
}

void Field::ProductivitySubstraction(AbstractWell* Well, bool substraction)
{
	int sign = substraction ? 1 : -1;
	string wellType = Well->GetWellType();
	if (wellType == "������� �������� ")
	{
		_gasMining -= sign*Well->_productivity;
	}
	if (wellType == "�������� �������� ")
	{
		_oilMining -= sign*Well->_productivity;
	}
	if (wellType == "�������������� �������� ")
	{
		_waterInjection -= sign*Well->_productivity;
	}
}

void Field::ShowFieldReport()
{
	cout << "���� ������: " << _gasMined << endl;
	cout << "����� ����������� ���� � ������������� �����: " << _gasVolume << endl;
	if (_gasMining != 0)
		cout << "������ ���� � �����: " << _gasMining << " | " << "��� ���������� ����� " << _gasVolume / _gasMining << " �����." << endl;
	else
		cout << "� ������ ������ ��� �������, ���������� ���." << endl;

	cout << "����� ������: " << _oilMined << endl;
	cout << "����� ���������� ����� � ������������� �����: " << _oilVolume << endl;
	if (_oilMining != 0)
		cout << "������ ����� � �����: " << _oilMining << " | " << "����� ���������� ����� " << _oilVolume / _oilMining << " �����." << endl;
	else
		cout << "� ������ ������ ��� �������, ���������� �����." << endl;

	cout << "����� ���� � ������������� �����: " << _waterVolume << endl;
	cout << "������� ���� � �����: " << _waterInjection << endl;
}

void Field::SetFieldParameters()
{
	int gasVolume, oilVolume;
	cout << "������� ����� ����" << endl;
	cin >> gasVolume;
	cout << "������� ����� �����" << endl;
	cin >> oilVolume;
	
	_gasVolume = gasVolume;
	_oilVolume = oilVolume;
}



Field::Field()
{
	SetFieldParameters();
	_wellCount = 0;
	_wellList = new AbstractWell * [_wellCount];
}

Field::~Field()
{
	delete[] _wellList;
}

string Field::GasWell::GetWellTotalVolume()
{
	return "����� ����������� ���� �����: ";
}
string Field::GasWell::GetWellType()
{
	return "������� �������� ";
}

string Field::OilWell::GetWellTotalVolume()
{
	return "����� ���������� ����� �����: ";
}

string Field::OilWell::GetWellType()
{
	return "�������� �������� ";
}

string Field::InjectionWell::GetWellTotalVolume()
{
	return "����� ���������� ���� �����: ";
}

string Field::InjectionWell::GetWellType()
{
	return "�������������� �������� ";
}
