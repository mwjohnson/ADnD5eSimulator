#include "weapon.h"
#include <iostream>
#include <sstream>

weapon::weapon(weaponName name, int cost, int ddc, int ddt, int weight,
	weaponType type, weaponDamageType wdt, int magicBonus, std::initializer_list<weaponProperty> props)
	: name_(name), cost_(cost), damageDiceCount_(ddc), damageDiceType_(ddt)
	, weight_(weight), type_(type), damageType_(wdt), normalRange_(-1)
	, longRange_(-1), magicBonus_(magicBonus), properties_()
{
	for each (weaponProperty wp in props)
	{
		addProperty(wp);
	}
}

weapon::~weapon() {
	int i = 0;
}

weapon::weapon(weaponName name, int cost, int ddc, int ddt, int weight,
	weaponType type, weaponDamageType wdt, int magicBonus, int normalRange, int longRange,
	std::initializer_list<weaponProperty> props)
	: name_(name), cost_(cost), damageDiceCount_(ddc), damageDiceType_(ddt)
	, weight_(weight), type_(type), damageType_(wdt), normalRange_(normalRange)
	, longRange_(longRange), magicBonus_(magicBonus), properties_() {
	for each (weaponProperty wp in props)
	{
		addProperty(wp);
	}
}

bool weapon::hasFinesse() const {
	for (int i = 0; i < properties_.size(); i++) {
		if (properties_.at(i) == weaponProperty::finesse){
			return true;
		}
	}
	return false;
}

void weapon::print() const {
	std::cout << weaponNames[static_cast<int>(name_)] << " " << damageDiceCount_ << "d" << damageDiceType_ << "+" << magicBonus_ << "\n";
}

std::string weapon::printjson() const
{
	std::ostringstream oss;
	oss << weaponNames[static_cast<int>(name_)] << " "+ std::to_string(damageDiceCount_) <<
	"d" + std::to_string(damageDiceType_) << "+" + std::to_string(magicBonus_);
	return oss.str();
}

