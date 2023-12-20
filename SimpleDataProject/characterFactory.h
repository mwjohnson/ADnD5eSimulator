#pragma once

#include "character.h"
class armorFactory;
class weaponFactory;
class spellFactory;
/**
 @class	characterFactory

 @brief	A character factory.

 @author	Matt
 @date	3/6/2018
 */

class characterFactory
{
public:
	characterFactory(armorFactory& af, weaponFactory& wf, spellFactory& sf, spellManager& sm, utils& util) : af_(af), wf_(wf), sf_(sf), sm_(sm), utils_(util) { }
	character* GenerateCharacter();
	int getMagicBonus(int level);
	~characterFactory();

private:
	armorFactory& af_;
	weaponFactory& wf_;
	spellFactory& sf_;
	spellManager& sm_;
	utils& utils_;

	characterClass getClass();
	int rollStat();
	int rollHp(characterClass chrClass, int lvl, int constitution);
	void setSavigThrowProficiencies(character* c, characterClass chrclass);
	void setSpellComponent(character* c) const;
	void prepareSpells(character* c);
	int getNumAttacks(characterClass chrclass, int lvl);

	const armor& equipArmor(characterClass chrClass, int lvl, int dexterity);
	const armor& equipShield(characterClass chrClass, int lvl);
	weapon& equipMeleeWeapon(characterClass chrClass, int lvl);
	weapon& equipRangeWeapon(characterClass chrClass, int lvl);
};