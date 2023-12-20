#pragma once
#include <vector>
#include "armor.h"
class armorFactory
{
public:
	armorFactory(utils& utils) : armors_(), utils_(utils) { populate(); }

	const armor& getArmor(int index) const { return armors_.at(index); }
	const armor& getLightArmor(int magicBonus) const;
	const armor& getMediumArmor(int magicBonus) const;
	const armor& getHeavyArmor(int magicBonus) const;
	const armor& getShield(int magicBonus) const;
	const armor& getNoShield() const;
	const armor& getCloth(int magicBonus) const;

private:
	const int magicIndexer = 15;
	void populate();
	std::vector<armor> armors_;
	utils& utils_;
};

