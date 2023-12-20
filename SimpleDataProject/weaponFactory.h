#pragma once
#include <vector>
#include <map>
#include "weapon.h"

class weaponFactory
{
public:
	weaponFactory();
	weapon& getWeapon(weaponName name, int magicBonus);
	weapon getWeapon(int index);

	~weaponFactory();
private:
	void populate();
	std::vector<weapon> weapons_;
	std::map<weaponName, int> map_;
};

