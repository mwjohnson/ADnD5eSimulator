#pragma once
#include "Utils.h"
class armor
{
public:
	armor() : name_(armorName::Nothing), cost_(0), ac_(0), type_(armorTypeCount), minimumStrength_(-1), weight_(0), magicBonus_(0) {}
	
	armor(armorName name, int cost, int ac, armorType type, int min_str, int weight, int magicBonus) :
		name_(name), cost_(cost), ac_(ac), type_(type), minimumStrength_(min_str), weight_(weight), magicBonus_(magicBonus) {}

	~armor() {}

	void addBonus(int bonus) { magicBonus_ = bonus; }
	armorName getName() const { return name_; }
	int getCost() const { return cost_; }
	int getAC() const { return ac_; }
	armorType getType() const { return type_; }
	int getMinimumStrength() const { return minimumStrength_; }
	int getWeight() const { return weight_; }
	int getMagicBonus() const { return magicBonus_; }

	int getTotalAC() const { return ac_ + magicBonus_; }
	void print() const;
	std::string printjson() const;

private:
	armorName name_;
	int cost_;
	int ac_;
	armorType type_;
	int minimumStrength_;
	int weight_;
	int magicBonus_;
};

