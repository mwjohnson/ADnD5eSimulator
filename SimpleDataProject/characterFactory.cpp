#include "characterFactory.h"
#include "Utils.h"
#include <cassert>
#include "statcontainer.h"
#include <iostream>
#include <tuple>
#include <crossguid/guid.hpp>
#include "armorFactory.h"
#include "weaponFactory.h"
#include "spellFactory.h"
#include "spell.h"
#include "easylogging++.h"


void getSpells(int& spells) {
	//c,1,2,3,4,5,6,7,8,9,
	int z[200] = { 3,2,0,0,0,0,0,0,0,0, // 1
		3,3,0,0,0,0,0,0,0,0,
		3,4,2,0,0,0,0,0,0,0, // 3
		4,4,3,0,0,0,0,0,0,0,
		4,4,3,2,0,0,0,0,0,0,	// 5
		4,4,3,3,0,0,0,0,0,0,
		4,4,3,3,1,0,0,0,0,0, // 7
		4,4,3,3,2,0,0,0,0,0,
		4,4,3,3,3,1,0,0,0,0, // 9
		5,4,3,3,3,2,0,0,0,0,
		5,4,3,3,3,2,1,0,0,0, // 11
		5,4,3,3,3,2,1,0,0,0,
		5,4,3,3,3,2,1,1,0,0, //13
		5,4,3,3,3,2,1,1,0,0,
		5,4,3,3,3,2,1,1,1,0, // 15
		5,4,3,3,3,2,1,1,1,0,
		5,4,3,3,3,2,1,1,1,1, //17
		5,4,3,3,3,2,1,1,1,1,
		5,4,3,3,3,3,2,1,1,1, //19
		5,4,3,3,3,3,2,2,1,1 };
	std::copy(z, z + 200, &spells);
}


characterClass characterFactory::getClass() {
	return static_cast<characterClass>(utils_.getRandomUniform(0, characterClass::characterClassCount - 1));
}

character* characterFactory::GenerateCharacter()
{
	const int length = stats::statsCount;
	int local_stats[length];
	for (size_t i = 0; i < length; i++)
	{
		local_stats[i] = rollStat();
	}
	characterClass chrclass = getClass();
	int lvl = utils_.getRandomUniform(1, 20);
	int hp = rollHp(chrclass, lvl, local_stats[stats::constitution]);

	int numattacks = getNumAttacks(chrclass, lvl);

	character* c = new character(xg::newGuid(),
		statcontainer(local_stats[stats::strength], local_stats[1], local_stats[2], local_stats[3], local_stats[4], local_stats[5]),
		hp, chrclass, lvl,
		equipArmor(chrclass, lvl, local_stats[stats::dexterity]),
		equipShield(chrclass, lvl),
		equipMeleeWeapon(chrclass, lvl),
		equipRangeWeapon(chrclass, lvl),
		numattacks, utils_);

	if (c->getAC() > 30) {

		assert(c->getAC() < 30, "AC TOO high.");
	}

	setSavigThrowProficiencies(c, chrclass);
	if (chrclass == characterClass::Cleric || chrclass == characterClass::Wizard) {
		setSpellComponent(c);
	}
	//prepareSpells(c);
	return c;
}

characterFactory::~characterFactory() {
}

int characterFactory::getNumAttacks(characterClass chrclass, int lvl) {
	switch (chrclass)
	{
	case Fighter:
		if (lvl < 5) { return 1; }
		if (lvl >= 5 && lvl < 11) { return 2; }
		if (lvl >= 11 && lvl < 20) { return 3; }
		if (lvl == 20) { return 4; }
		break;
	case Rogue:
	case Wizard:
	case Cleric:
		return 1;
		break;
	case characterClassCount:
	default:
		break;
	}
	return 1;
}

void characterFactory::setSpellComponent(character* c) const {
	int spells[10 * 20];
	getSpells(spells[0]);
	int lvl = c->getLevel();
	int startIndex = (lvl - 1) * 10;
	int spellSets[10];
	for (int i = 0; i < 10; i++) {
		spellSets[i] = spells[startIndex + i];
	}
	c->updateSpellComponent(spellSets, &sm_);
}

int characterFactory::getMagicBonus(int level)
{
	int bonus = level / 4.f;
	if (bonus < 1) {
		return 0;
	}
	int r = utils_.getRandomUniform(0, 100); // .getRandomUniform(0, 100);
	if (r > 15) {
		level -= 4;
		bonus = characterFactory::getMagicBonus(level);
	}
	return bonus;
}

void characterFactory::prepareSpells(character* c) {

	characterClass cls = c->getClass();

	if (cls == Cleric) {
		std::string spells[10] = { "", "", "Hold Person", //2
			"Inflict Wounds",
			"Guardian of Faith",
			"Flame Strike",
			"Blade Barrier",
			"Fire Storm",
			"Earthquake",
			"Fire Storm9" };
		/*for each (std::string& spellname in spells)
		{
			if (!spellname.empty()) {
				spell s = sf_.getSpell(spellname.c_str());
				c->addSpell(s);
			}
		}*/
	}

	if (cls == Wizard) {
		std::string spells[11] = {
			"",
			"Magic Missile",
			"Hold Person",
			"Flaming Sphere",
			"Fireball",
			//"Lightning Bolt",
			"Ice Storm",
			"Cone of Cold",
			//"Disintegrate", 
			"Chain Lightning",
			"Finger of Death",
			"Sunburst",
			"Power Word Kill" };
		/*for each (std::string& spellname in spells)
		{
			if (!spellname.empty()) {
				c->addSpell(sf_.getSpell(spellname.c_str()));
			}
		}*/
	}
}

const armor& characterFactory::equipShield(characterClass chrClass, int lvl) {
	int v = -1;
	int magicBonus = getMagicBonus(lvl);
	switch (chrClass)
	{
	case Fighter:
		v = utils_.getRandomUniform(0, 1);
		return (v == 0) ? af_.getShield(magicBonus) : af_.getNoShield();
		break;
	case Rogue:
	case Wizard:
		return af_.getNoShield();
		break;
	case Cleric:
		return af_.getShield(magicBonus);
		break;
	case characterClassCount:
	default:
		assert(false);
		break;
	}
}

const armor& characterFactory::equipArmor(characterClass chrClass, int lvl, int dexterity) {
	int magicBonus = getMagicBonus(lvl);
	int dexmod = statcontainer::getStatModifier(dexterity);
	int v = -1;
	switch (chrClass)
	{
	case Fighter:
		if (dexmod > 2) {
			v = utils_.getRandomUniform(0, 2);
			if (v == 0) {
				return af_.getLightArmor(magicBonus);
			}
			else if (v == 1) {
				return af_.getMediumArmor(magicBonus);
			}
			else {
				return af_.getHeavyArmor(magicBonus);
			}
		}
		else {
			int v = utils_.getRandomUniform(0, 1);
			return (v == 0) ?
				af_.getMediumArmor(magicBonus) :
				af_.getHeavyArmor(magicBonus);
		}

		break;
	case Rogue:
		return af_.getLightArmor(magicBonus);
		break;
	case Wizard:
		return af_.getCloth(magicBonus);
		break;
	case Cleric:
		v = utils_.getRandomUniform(0, 1);
		return (v == 0) ?
			af_.getLightArmor(magicBonus) :
			af_.getMediumArmor(magicBonus);

		break;
	case characterClassCount:
	default:
		assert(false);
		std::cout << "invlaid character class.";
		break;
	}
}

weapon& characterFactory::equipRangeWeapon(characterClass chrClass, int lvl) {
	int magicBonus = getMagicBonus(lvl);
	int r = -1;
	switch (chrClass)
	{
	case Fighter:
		r = utils_.getRandomUniform(0, 1);
		return (r == 0) ?
			wf_.getWeapon(weaponName::Longbow, magicBonus) :
			wf_.getWeapon(weaponName::Crossbow_light, magicBonus);
		break;
	case Rogue:
		return wf_.getWeapon(weaponName::Shortbow, magicBonus);
		break;
	case Wizard:
		return wf_.getWeapon(weaponName::Dagger, magicBonus);
		break;
	case Cleric:
		return wf_.getWeapon(weaponName::Crossbow_light, magicBonus);
		break;
	case characterClassCount:
	default:
		assert(false);
		std::cout << "invlaid character class.";
		break;
	}
}

weapon& characterFactory::equipMeleeWeapon(characterClass chrClass, int lvl) {
	int magicBonus = getMagicBonus(lvl);
	int r = -1;
	switch (chrClass)
	{
	case Fighter:
		return wf_.getWeapon(weaponName::Handaxe, magicBonus);
		break;
	case Rogue:
		return wf_.getWeapon(weaponName::Shortsword, magicBonus);
		break;
	case Wizard:
		return wf_.getWeapon(weaponName::Dagger, magicBonus);
		break;
	case Cleric:
		r = utils_.getRandomUniform(0, 1);
		return (r == 0) ?
			wf_.getWeapon(weaponName::Mace, magicBonus) :
			wf_.getWeapon(weaponName::Warhammer, magicBonus);
		break;
	case characterClassCount:
	default:
		assert(false);
		std::cout << "invlaid character class.";
		break;
	}
}


int characterFactory::rollHp(characterClass chrClass, int lvl, int constitution) {

	int dice = 0;
	switch (chrClass)
	{
	case Fighter:
		dice = 10;
		break;
	case Rogue:
		dice = 8;
		break;
	case Wizard:
		dice = 6;
		break;
	case Cleric:
		dice = 8;
		break;
	case characterClassCount:
	default:
		assert(false);
		std::cout << "invlaid character class.";
		break;
	}
	int hp = dice + statcontainer::getStatModifier(constitution); // level 1 starts out with max hp.
	for (int i = 1; i < lvl; i++) {
		int res = utils_.getRandomUniform(1, dice) + statcontainer::getStatModifier(constitution);
		if (res < 1) { res = 1; }
		//LOG(INFO) << "Hp rolls: "<<res;
		hp += res;
	}

	return hp;

}

void characterFactory::setSavigThrowProficiencies(character* c, characterClass chrclass)
{
	switch (chrclass)
	{
	case Fighter:
		c->setSavingThrowProficiency(0, stats::strength);
		c->setSavingThrowProficiency(1, stats::constitution);
		break;
	case Rogue:
		c->setSavingThrowProficiency(0, stats::dexterity);
		c->setSavingThrowProficiency(1, stats::intelligence);
		break;
	case Wizard:
		c->setSavingThrowProficiency(0, stats::intelligence);
		c->setSavingThrowProficiency(1, stats::wisdom);
		break;
	case Cleric:
		c->setSavingThrowProficiency(0, stats::wisdom);
		c->setSavingThrowProficiency(1, stats::charisma);
		break;
	case characterClassCount:
	default:
		assert(false);
		std::cout << "invlaid character class.";
		break;
	}

}

int characterFactory::rollStat() {
	int stat = 0;
	for (int i = 0; i < 3; i++) {
		stat += utils_.getRandomUniform(1, 6);
	}
	return stat;
}