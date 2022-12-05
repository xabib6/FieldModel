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

		cout << "Список команд: \n1)Показать все скважины. \n2)Добавить новую скважину. \n" <<
			"3)Удалить скважину. \n4)Выбрать скважину. \n5)Показать отчёт по месторождению. \n" <<
			"6)Завершить работу. " << endl;

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
			cout << "Какую скважину удалить?" << endl;
			cin >> input;
			RemoveWell(input);
			break;
		case 4:
			cout << "Какую скважину выбрать?" << endl;
			cin >> input;
			SelectWell(input);
			break;
		case 5:
			ShowFieldReport();
			break;
		case 6:
			return;
		default:
			cout << "Такой команды не существует!" << endl;
		}

	}
}


void Field::ShowAllWell()
{
	if (_wellCount != 0)
	{
		cout << "Скважин в данный момент: " << _wellCount << endl << endl;
		for (int i = 0; i < _wellCount; i++)
		{
			cout << i + 1 << ")";
			_wellList[i]->PrintWellInfo();
		}
	}
	else
		cout << "Сейчас на месторождении нет скважин. " << endl;
}

void Field::SelectWell(int selectedWell)
{
	if (_wellCount == 0)
	{
		cout << "В данный момент нет скважин. " << endl;
		return;
	}
	if (selectedWell > _wellCount || selectedWell < 1)
	{
		cout << "Скважины с таким номером не существует!" << endl;
		return;
	}
	_wellList[selectedWell - 1]->PrintWellInfo();

	int productivity, daysWorked, choose;
	cout << "1)Включить\\выключить скважину. \n2)Вернуться в меню." << endl;
	cin >> choose;

	switch (choose)
	{
	case 1:
		ProductivitySubstraction(_wellList[selectedWell - 1], _wellList[selectedWell - 1]->GetWellState() == "работает");
		_wellList[selectedWell - 1]->ChangeWorkingStatus();
		cout << endl;
		break;
	case 2:
		break;
	default:
		cout << "Такой команды не существует! " << endl;
		return;
	}
}

void Field::AddWell()
{
	int wellType, productivity, daysWorked;
	cout << "Выберите тип скважины: \n1)Газовая		\n2)Нефтяная	\n3)Нагнетательная" << endl;
	cin >> wellType;
	if (wellType != 1 && wellType != 2 && wellType != 3)
	{
		cout << "Такого типа скважины не существует!" << endl;
		return;
	}
	cout << "Введите производительность скважины, м3/сутки. " << endl;
	cin >> productivity;
	cout << "Введите количество отработанных дней. " << endl;
	cin >> daysWorked;

	AbstractWell* well;
	switch (wellType)
	{
	case 1:
		if (productivity * daysWorked > _gasVolume)
		{
			cout << "Количество добытого газа превышает остаток. Введите параметры снова." << endl;
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
			cout << "Количество добытой нефти превышает остаток. Введите параметры снова." << endl;
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

	cout << well->GetWellType() << "добавлена." << endl;
}

void Field::RemoveWell(int deleteWell)
{
	if (deleteWell > _wellCount || deleteWell < 1)
	{
		cout << "Скважины с таким номером не существует!" << endl;
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
	if (wellType == "Газовая скважина ")
	{
		_gasMining -= sign*Well->_productivity;
	}
	if (wellType == "Нефтяная скважина ")
	{
		_oilMining -= sign*Well->_productivity;
	}
	if (wellType == "Нагнетательная скважина ")
	{
		_waterInjection -= sign*Well->_productivity;
	}
}

void Field::ShowFieldReport()
{
	cout << "Газа добыто: " << _gasMined << endl;
	cout << "Объём оставшегося газа в месторождении равен: " << _gasVolume << endl;
	if (_gasMining != 0)
		cout << "Добыча газа в сутки: " << _gasMining << " | " << "Газ закончится через " << _gasVolume / _gasMining << " суток." << endl;
	else
		cout << "В данный момент нет скважин, добывающих газ." << endl;

	cout << "Нефти добыто: " << _oilMined << endl;
	cout << "Объём оставшейся нефти в месторождении равен: " << _oilVolume << endl;
	if (_oilMining != 0)
		cout << "Добыча нефти в сутки: " << _oilMining << " | " << "Нефть закончится через " << _oilVolume / _oilMining << " суток." << endl;
	else
		cout << "В данный момент нет скважин, добывающих нефть." << endl;

	cout << "Объём воды в месторождении равен: " << _waterVolume << endl;
	cout << "Закачка воды в сутки: " << _waterInjection << endl;
}

void Field::SetFieldParameters()
{
	int gasVolume, oilVolume;
	cout << "Введите объём газа" << endl;
	cin >> gasVolume;
	cout << "Введите объём нефти" << endl;
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
	return "Объём выкачанного газа равен: ";
}
string Field::GasWell::GetWellType()
{
	return "Газовая скважина ";
}

string Field::OilWell::GetWellTotalVolume()
{
	return "Объём выкачанной нефти равен: ";
}

string Field::OilWell::GetWellType()
{
	return "Нефтяная скважина ";
}

string Field::InjectionWell::GetWellTotalVolume()
{
	return "Объём закачанной воды равен: ";
}

string Field::InjectionWell::GetWellType()
{
	return "Нагнетательная скважина ";
}
