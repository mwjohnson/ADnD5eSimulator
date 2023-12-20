#include "armorFactory.h"
#include <assert.h>
#include <iostream>

void armorFactory::populate() {
	for (int magicBonus = 0; magicBonus < 6; magicBonus++) {
		armors_.push_back(armor(armorName::Nothing, 1, 0, armorType::at_none, 0, 0, magicBonus)); //For when there is no shield.
		armors_.push_back(armor(armorName::Clothing, 1, 10, armorType::at_none, 0, 1, magicBonus)); //For wizard
		// None Armor ( 0/15/30, 1/16/31 )
			//Clothing	1gp 10 + Dex modifier - - 1 lb.

		armors_.push_back(armor(armorName::Padded, 5, 11, armorType::light, 0, 8, magicBonus));
		armors_.push_back(armor(armorName::Leather, 10, 11, armorType::light, 0, 10, magicBonus));
		armors_.push_back(armor(armorName::Studded_Leather, 45, 12, armorType::light, 0, 13, magicBonus));
		//Light Armor ( 2/17/32, 3/18/33, 4/19/34 )
			//Padded			5 gp 11 + Dex modifier — Disadvantage 8 lb.
			//Leather			10 gp 11 + Dex modifier — — 10 lb.
			//Studded leather 45 gp 12 + Dex modifier — — 13 lb.

		armors_.push_back(armor(armorName::Hide, 10, 12, armorType::medium, 0, 12, magicBonus));
		armors_.push_back(armor(armorName::Chain_shirt, 50, 13, armorType::medium, 0, 20, magicBonus));
		armors_.push_back(armor(armorName::Scale_mail, 50, 14, armorType::medium, 0, 45, magicBonus));
		armors_.push_back(armor(armorName::Breastplate, 400, 14, armorType::medium, 0, 20, magicBonus));
		armors_.push_back(armor(armorName::Half_Plate, 750, 15, armorType::medium, 0, 40, magicBonus));
		//Medium Armor ( 5/20/35, 6/21/36, 7/22/37, 8/23/38, 9/24/39 )
			//Hide		10 gp 12 + Dex modifier(max 2) — — 12 lb.
			//Chain shirt 50 gp 13 + Dex modifier(max 2) — — 20 lb.
			//Scale mail	50 gp 14 + Dex modifier(max 2) — Disadvantage 45 lb.
			//Breastplate 400 gp 14 + Dex modifier(max 2) — — 20 lb.
			//Half plate	750 gp 15 + Dex modifier(max 2) — Disadvantage 40 lb.

		armors_.push_back(armor(armorName::Ring_mail, 30, 14, armorType::heavy, 0, 40, magicBonus));
		armors_.push_back(armor(armorName::Chain_mail, 75, 16, armorType::heavy, 13, 55, magicBonus));
		armors_.push_back(armor(armorName::Splint, 200, 17, armorType::heavy, 15, 60, magicBonus));
		armors_.push_back(armor(armorName::Plate, 1500, 18, armorType::heavy, 15, 65, magicBonus));
		//Heavy Armor ( 10/25/40, 11/26/41, 12/27/42, 13/28/43 )
			//Ring mail	30 gp 14 — Disadvantage 40 lb.
			//Chain mail	75 gp 16 Str 13 Disadvantage 55 lb.
			//Splint		200 gp 17 Str 15 Disadvantage 60 lb.
			//Plate		1, 500 gp 18 Str 15 Disadvantage 65 lb.

		armors_.push_back(armor(armorName::Shield, 10, 2, armorType::shield, 0, 6, magicBonus));
		//Shield ( 14/29/44 )
			//Shield 10 gp + 2 — — 6 lb
	}
}
const armor& armorFactory::getLightArmor(int magicBonus) const
{
	int magicIndex = magicBonus * magicIndexer;
	int v = utils_.getRandomUniform(2, 4);
	v += magicIndex;
	assert(armors_.at(v).getType() == armorType::light);
	return armors_.at(v);
}

const armor& armorFactory::getMediumArmor(int magicBonus) const
{
	int magicIndex = magicBonus * magicIndexer;
	int v = utils_.getRandomUniform(5, 9);
	v += magicIndex;
	assert(armors_.at(v).getType() == armorType::medium);
	return armors_.at(v);
}

const armor& armorFactory::getHeavyArmor(int magicBonus) const
{
	int magicIndex = magicBonus * magicIndexer;
	int v = utils_.getRandomUniform(10, 13);
	v += magicIndex;
	assert(armors_.at(v).getType() == armorType::heavy);
	return armors_.at(v);
}

const armor& armorFactory::getShield(int magicBonus) const
{
	int shieldIndex = 14;
	int magicIndex = shieldIndex + magicBonus * magicIndexer;
	assert(armors_.at(magicIndex).getType() == armorType::shield);
	return armors_.at(magicIndex);
}

const armor& armorFactory::getNoShield() const {
	return armors_.at(0);
}
const armor& armorFactory::getCloth(int magicBonus) const {
	int magicIndex = 1 + magicBonus * magicIndexer;
	assert(armors_.at(magicIndex).getType() == armorType::at_none);
	assert(armors_.at(magicIndex).getName() == armorName::Clothing);
	return armors_.at(magicIndex+1);
}
