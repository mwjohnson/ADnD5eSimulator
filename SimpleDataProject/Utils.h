/**
 @file	Utils.h.

 @brief	Declares the utilities class.
 */

#pragma once
#include <random>
#include <cmath>

class character;

enum characterClass
{
	Fighter,
	Rogue,
	Wizard,
	Cleric,
	characterClassCount
};

enum armorType {
	at_none,
	light,
	medium,
	heavy,
	shield,
	armorTypeCount
};

enum armorName {
	Nothing,
	Clothing,
	Padded,
	Leather,
	Studded_Leather,
	Hide,
	Chain_shirt,
	Scale_mail,
	Breastplate,
	Half_Plate,
	Ring_mail,
	Chain_mail,
	Splint,
	Plate,
	Shield,
	armorNameCount
};

const std::string armorNames[] = { "Nothing",
"Clothing",
"Padded",
"Leather",
"Studded_Leather",
"Hide",
"Chain_shirt",
"Scale_mail",
"Breastplate",
"Half_Plate",
"Ring_mail",
"Chain_mail",
"Splint",
"Plate",
"Shield"
};

enum weaponType {
	simpleMelee,
	martialMelee,
	simpleRange,
	martialRange,
	weaponTypeCount
};

enum weaponName {
	None,
	Mace,
	Dagger,
	Quarterstaff,
	Handaxe,
	Shortbow,
	Crossbow_light,
	Battleaxe,
	Flail,
	Glaive,
	Greataxe,
	Greatsword,
	Halberd,
	Lance,
	Longsword,
	Maul,
	Morningstar,
	Pike,
	Rapier,
	Scimitar,
	Shortsword,
	Trident,
	War_pick,
	Warhammer,
	Whip,
	Longbow,
	weaponNameCount
};

const std::string weaponNames[] = { "None",
"Mace",
"Dagger",
"Quarterstaff",
"Handaxe",
"Shortbow",
"Crossbow_light",
"Battleaxe",
"Flail",
"Glaive",
"Greataxe",
"Greatsword",
"Halberd",
"Lance",
"Longsword",
"Maul",
"Morningstar",
"Pike",
"Rapier",
"Scimitar",
"Shortsword",
"Trident",
"War_pick",
"Warhammer",
"Whip",
"Longbow" };

enum weaponDamageType {
	bludgeon,
	pierce,
	slash,
	weaponDamageTypeCount
};

enum weaponProperty {
	wp_light,
	finesse,
	thrown,
	versatile,
	wp_heavy,
	twohanded,
	reach,
	ammunition,
	loading,
	special,
	weaponPropertiesCount
};

const std::string classNames[] = { "Fighter", "Rogue", "Wizard", "Cleric"};

enum spellName {
	NoneSpell,
	Fire_Bolt,
	Magic_Missile,
	Hold_Person,
	Flaming_Sphere,
	Fireball,
	Lightning_Bolt,
	Ice_Storm,
	Cone_of_Cold,
	Disintegrate,
	Chain_Lightning,
	Finger_of_Death,
	Sunburst,
	Power_Word_Kill,
	Sacred_Flame,
	Cure_Wounds,
	Inflict_Wounds,
	Guardian_of_Faith,
	Flame_Strike,
	Blade_Barrier,
	Fire_Storm,
	Earthquake,
	Mass_Heal,
	Fire_Storm9,
	spellNameCout
};

class utils
{
public:
	utils();
	~utils() {} 
	int getRandomUniform(int min, int max);
	//static int getRandomUniformStatic(int min, int max);
	static int getRandomNormalCountdSides(int count, int sides);
	static int getDistance(const character* self, const character* enemy);
	static int getProficiencyBonus(int level);
private:
	std::random_device rd_;     // only used once to initialise (seed) engine
	std::mt19937 rng_;// (rd());    // random-number engine used (Mersenne-Twister in this case)
};
