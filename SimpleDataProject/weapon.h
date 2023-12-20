#pragma once
#include "Utils.h"

class weapon
{
public:
	weapon() : name_(weaponName::None), cost_(0), damageDiceCount_(0), damageDiceType_(0), weight_(0)
		, type_(weaponType::weaponTypeCount), damageType_(weaponDamageTypeCount), normalRange_(-1),
		longRange_(-1), magicBonus_(0), properties_() {}

	weapon(weaponName name, int cost, int ddc, int ddt, int weight, weaponType type, weaponDamageType wdt, int magicBonus)
		: name_(name), cost_(cost), damageDiceCount_(ddc), damageDiceType_(ddt), weight_(weight)
		, type_(type), damageType_(wdt), normalRange_(-1), longRange_(-1), magicBonus_(magicBonus), properties_() {}

	weapon(weaponName name, int cost, int ddc, int ddt, int weight,
		weaponType type, weaponDamageType wdt, int magicBonus, std::initializer_list<weaponProperty> props);
		

	weapon(weaponName name, int cost, int ddc, int ddt, int weight,
		weaponType type, weaponDamageType wdt, int magicBonus, 
		int normalRange, int longRange, std::initializer_list<weaponProperty> props);

	~weapon();

	void addProperty(weaponProperty wp) { properties_.push_back(wp); }
	const std::vector<weaponProperty>& getProperties() { return properties_; }

	void addBonus(int bonus) { magicBonus_ = bonus; }

	int getDamageDiceCount() const { return damageDiceCount_; }
	int getDamageDiceType() const { return damageDiceType_; }
	int getNormalRange() const { return normalRange_; }
	int getLongRange() const { return longRange_; }
	weaponName getName() const { return name_; }
	int getBonus() const { return magicBonus_; }

	bool hasFinesse() const;

	void print() const;

	std::string printjson() const;

private:
	std::vector<weaponProperty> properties_;
	weaponName name_;
	weaponType type_;
	weaponDamageType damageType_;
	int cost_;
	int damageDiceCount_;
	int damageDiceType_;
	int weight_;
	int normalRange_;
	int longRange_;
	int magicBonus_;
};



