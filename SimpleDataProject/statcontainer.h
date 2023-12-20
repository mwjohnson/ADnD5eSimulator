#pragma once
#include <iostream>
#include <string>
#include <cassert>
#include <sstream>


enum stats {
	strength,
	constitution,
	dexterity,
	intelligence,
	wisdom,
	charisma,
	statsCount
};

const std::string statNames[] = { "Strength", "Constitution", "Dexterity", "Intelligence", "Wisdom", "Charisma" };
//std::vector<std::string> v = { "Hello", "World" };

class statcontainer
{
public:
	statcontainer() : stats_() {}
	statcontainer(int str, int con, int dex, int intl, int wis, int chr);
	~statcontainer();

	static int statcontainer::getStatModifier(int value) {
		switch (value)
		{
		case 2:
		case 3:
			return -4;
		case 4:
		case 5:
			return -3;
		case 6:
		case 7:
			return -2;
		case 8:
		case 9:
			return -1;
		case 10:
		case 11:
			return 0;
		case 12:
		case 13:
			return 1;
		case 14:
		case 15:
			return 2;
		case 16:
		case 17:
			return 3;
		case 18:
		case 19:
			return 4;
		default:
			assert(false);
			return 0;
			break;

		}
	}

	int getStat(stats stat) const { return stats_[stat]; }
	int getModifier(stats stat) const { return statcontainer::getStatModifier(getStat(stat)); }

	void print() const {
		int i = 0;
		for (int j = 0; j < stats::statsCount;j++)
		{
			std::cout<< statNames[i] << ": " << stats_[j] << "\n";
			i++;
		}
	}

	void printjson() const {
		int i = 0;
		for (int j = 0; j < stats::statsCount; j++)
		{
			std::cout << "\"" << statNames[i] << "\": " << stats_[j] << ", ";
			i++;
		}
	}

	std::string asString() const {
		std::ostringstream oss;
		int i = 0;
		for (int j = 0; j < stats::statsCount; j++)
		{
			oss << "\"" << statNames[i] << "\": " << stats_[j] << ", ";
			i++;
		}
		return oss.str();
	}

private:
	int stats_[stats::statsCount];
};

