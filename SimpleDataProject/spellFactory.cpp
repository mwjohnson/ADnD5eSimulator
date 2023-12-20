#include "spellFactory.h"
#include "effect.h"

void spellFactory::populate() {

	// Wizard:
	spellLibrary_.push_back(spell(spellName::Fire_Bolt, new directDamage(1, 10, 0), stats::statsCount, 0, 120, false)); //- No saving throw! Requires hit.
	spellLibrary_.push_back(spell(spellName::Magic_Missile, new directDamage(3,4,1), stats::statsCount, 1, 120, false)); //- No saving throw!
	spellLibrary_.push_back(spell(spellName::Hold_Person, new paralyze(10), stats::wisdom,2,60,true)); // 2 Paralyze on failed.
	spellLibrary_.emplace_back(spell(spellName::Flaming_Sphere, new directDamage(2,6,0),stats::dexterity,2,60, true)); // 2
	spellLibrary_.emplace_back(spell(spellName::Fireball, new directDamage(8,6,0),stats::dexterity,3,150, true)); // 3
	spellLibrary_.emplace_back(spell(spellName::Lightning_Bolt, new directDamage(8,6,0),stats::dexterity,3,100, true)); // 3
	spellLibrary_.emplace_back(spell(spellName::Ice_Storm, new doubleDirectDamage(2,8,4,6),stats::dexterity,4,300, true)); // 4 2d8 + 4d6 (becomes 6d6+4)
	spellLibrary_.emplace_back(spell(spellName::Cone_of_Cold, new directDamage(8,8,0),stats::constitution,5,60, true)); // 5
	spellLibrary_.emplace_back(spell(spellName::Disintegrate, new directDamage(10,6,40),stats::dexterity,6,60, true)); // 6 - save for 0.
	spellLibrary_.emplace_back(spell(spellName::Chain_Lightning, new directDamage(10,8,0),stats::dexterity,6,150, true)); // 6
	spellLibrary_.emplace_back(spell(spellName::Finger_of_Death, new directDamage(7,8,30),stats::constitution,7,60, true)); // 7
	spellLibrary_.emplace_back(spell(spellName::Sunburst, new directDamage(12,6,0),stats::constitution,8,150, true)); // 8 blind on failed save.
	spellLibrary_.emplace_back(spell(spellName::Power_Word_Kill, new powerWordKill(),stats::statsCount,9,60, false)); // 9 - No saving throw!

	// Cleric:
	spellLibrary_.emplace_back(spell(spellName::Sacred_Flame, new directDamage(1, 8, 0), stats::dexterity, 0, 60, true));
	spellLibrary_.emplace_back(spell(spellName::Cure_Wounds, new heal(1,8,0),stats::statsCount,1,1,false)); // 1 1d8 + your spellcasting ability modifier. 
	//Hold person.
	spellLibrary_.emplace_back(spell(spellName::Inflict_Wounds, new directDamage(5,10,0),stats::statsCount,3,1,false)); // 3 (Requires Landing a hit)
	spellLibrary_.emplace_back(spell(spellName::Guardian_of_Faith, new directDamage(1,1,19),stats::dexterity,4,30,true)); // 4
	spellLibrary_.emplace_back(spell(spellName::Flame_Strike, new directDamage(8,6,0),stats::dexterity,5,60,true)); // 5
	spellLibrary_.emplace_back(spell(spellName::Blade_Barrier, new directDamage(6,10,0),stats::dexterity,6,90,true)); // 6 
	spellLibrary_.emplace_back(spell(spellName::Fire_Storm, new directDamage(7,10,0),stats::dexterity,7,150,true)); // 7
	spellLibrary_.emplace_back(spell(spellName::Earthquake, new directDamage(1,1,49),stats::dexterity,8,500,true)); // 8
	spellLibrary_.emplace_back(spell(spellName::Mass_Heal, new heal(1,1,699),stats::statsCount,9,60,false)); // 9 (HEAL)
	spellLibrary_.push_back(spell(spellName::Fire_Storm9, new directDamage(7, 10, 0), stats::dexterity, 9, 150, true)); // 7

	for (int i = 0; i < spellLibrary_.size(); i++) {
		map_.insert(std::make_pair(spellLibrary_.at(i).getName(), i));
	}
}
spell& spellFactory::getSpell(spellName name) {
	
		return spellLibrary_.at(map_[name]);
	
}

spellFactory::~spellFactory() {
	map_.clear();
	spellLibrary_.clear();
}