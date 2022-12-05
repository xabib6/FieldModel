#pragma once
#include "AbstractWell.h"
#include <string>

using namespace std;

class Field
{
private:
	static Field* _instance;
	int _gasVolume;
	int _oilVolume;
	int _waterVolume;
	int _wellCount;
	int _gasMining;
	int _oilMining;
	int _waterInjection;
	int _gasMined;
	int _oilMined;


	AbstractWell** _wellList;

	void ShowAllWell();
	void SelectWell(int selectedWell);
	void AddWell();
	void RemoveWell(int deleteWell);
	void ProductivitySubstraction(AbstractWell* Well, bool substraction = true);
	void ShowFieldReport();
	void SetFieldParameters();



	class GasWell : public AbstractWell
	{
		string GetWellTotalVolume();
		string GetWellType();

	public:
		GasWell(int productivity, int daysWorked) : AbstractWell(productivity, daysWorked) {};
	};

	class OilWell : public AbstractWell
	{
		string GetWellTotalVolume();
		string GetWellType();

	public:
		OilWell(int productivity, int daysWorked) : AbstractWell(productivity, daysWorked) {};
	};

	class InjectionWell : public AbstractWell
	{
		string GetWellTotalVolume();
		string GetWellType();

	public:
		InjectionWell(int productivity, int daysWorked) : AbstractWell(productivity, daysWorked) {};
	};

public:

	Field();
	~Field();

	static Field GetInstance();
	void FieldManager();

};