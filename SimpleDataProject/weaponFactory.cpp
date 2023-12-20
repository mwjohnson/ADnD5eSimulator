#include "weaponFactory.h"

weaponFactory::weaponFactory() : weapons_() {
	weapons_.reserve(100);
	populate(); 
	
}

void weaponFactory::populate() {
	
	//Simple Melee
	
	for (int magicBonus = 0; magicBonus < 6; magicBonus++) {

		weapons_.push_back(weapon(weaponName::Mace, 5, 1, 6, 1, simpleMelee, bludgeon, magicBonus)); //Mace 5 gp 1d6 bludgeoning 4 lb.—
		weapons_.push_back(weapon(weaponName::Dagger, 2, 1, 4, 1, simpleMelee, pierce, magicBonus, 20, 60, { weaponProperty::finesse, weaponProperty::wp_light, weaponProperty::thrown })); //Dagger 2 gp 1d4 piercing 1 lb.Finesse, light, thrown(range 20 / 60)
		weapons_.push_back(weapon(weaponName::Quarterstaff, 2, 1, 6, 4, simpleMelee, bludgeon, magicBonus)); //Quarterstaff 2 sp 1d6 bludgeoning 4 lb.Versatile(1d8)
		weapons_.push_back(weapon(weaponName::Handaxe, 5, 1, 6, 1, simpleMelee, slash, magicBonus)); //Handaxe 5 gp 1d6 slashing 2 lb.Light, thrown(range 20 / 60)

		//4
		weapons_.push_back(weapon(weaponName::Shortbow, 25, 1, 6, 2, simpleRange, pierce, magicBonus, 80, 320, { twohanded })); //Shortbow 25 gp 1d6 piercing 2 lb.Ammunition(range 80 / 320), two-handed
		weapons_.push_back(weapon(weaponName::Crossbow_light, 25, 1, 8, 5, simpleRange, pierce, magicBonus, 80, 320, { loading, twohanded })); //Crossbow, light 25 gp 1d8 piercing 5 lb.Ammunition(range 80 / 320), loading, two-handed

		//6
		weapons_.push_back(weapon(weaponName::Battleaxe, 10, 1, 6, 1, martialMelee, slash, magicBonus, { versatile })); //Battleaxe 10 gp 1d8 slashing 4 lb.Versatile(1d10)
		weapons_.push_back(weapon(weaponName::Flail, 10, 1, 6, 1, martialMelee, bludgeon, magicBonus)); //Flail 10 gp 1d8 bludgeoning 2 lb.—
		weapons_.push_back(weapon(weaponName::Glaive, 20, 1, 6, 1, martialMelee, slash, magicBonus, { wp_heavy, reach, twohanded })); //Glaive 20 gp 1d10 slashing 6 lb.Heavy, reach, two - handed
		weapons_.push_back(weapon(weaponName::Greataxe, 30, 1, 6, 1, martialMelee, slash, magicBonus, { wp_heavy, twohanded })); //Greataxe 30 gp 1d12 slashing 7 lb.Heavy, two - handed

		//10

		weapons_.push_back(weapon(weaponName::Greatsword, 50, 1, 6, 1, martialMelee, slash, magicBonus, { wp_heavy, twohanded })); //Greatsword 50 gp 2d6 slashing 6 lb.Heavy, two - handed
		weapons_.push_back(weapon(weaponName::Halberd, 20, 1, 6, 1, martialMelee, slash, magicBonus, { wp_heavy, reach, twohanded })); //Halberd 20 gp 1d10 slashing 6 lb.Heavy, reach, two - handed
		weapons_.push_back(weapon(weaponName::Lance, 10, 1, 6, 1, martialMelee, pierce, magicBonus, { reach, special })); //Lance 10 gp 1d12 piercing 6 lb.Reach, special
		weapons_.push_back(weapon(weaponName::Longsword, 15, 1, 6, 1, martialMelee, slash, magicBonus, { versatile })); //Longsword 15 gp 1d8 slashing 3 lb.Versatile(1d10)
		weapons_.push_back(weapon(weaponName::Maul, 10, 1, 6, 1, martialMelee, bludgeon, magicBonus, { wp_heavy, twohanded })); //Maul 10 gp 2d6 bludgeoning 10 lb.Heavy, two - handed
		weapons_.push_back(weapon(weaponName::Morningstar, 15, 1, 6, 1, martialMelee, pierce, magicBonus)); //Morningstar 15 gp 1d8 piercing 4 lb.—
		weapons_.push_back(weapon(weaponName::Pike, 5, 1, 6, 1, martialMelee, pierce, magicBonus, { wp_heavy, reach, twohanded })); //Pike 5 gp 1d10 piercing 18 lb.Heavy, reach, two - handed
		weapons_.push_back(weapon(weaponName::Rapier, 25, 1, 8, 2, martialMelee, pierce, magicBonus, { finesse })); //Rapier 25 gp 1d8 piercing 2 lb.Finesse
		weapons_.push_back(weapon(weaponName::Scimitar, 25, 1, 6, 1, martialMelee, slash, magicBonus, { finesse, wp_light })); //Scimitar 25 gp 1d6 slashing 3 lb.Finesse, light
		weapons_.push_back(weapon(weaponName::Shortsword, 10, 1, 6, 1, martialMelee, pierce, magicBonus, { finesse })); //Shortsword 10 gp 1d6 piercing 2 lb.Finesse, light
		weapons_.push_back(weapon(weaponName::Trident, 5, 1, 6, 1, martialMelee, pierce, magicBonus, 20, 60, { thrown, versatile })); //Trident 5 gp 1d6 piercing 4 lb.Thrown(range 20 / 60), versatile(1d8)
		weapons_.push_back(weapon(weaponName::War_pick, 5, 1, 6, 1, martialMelee, pierce, magicBonus)); //War pick 5 gp 1d8 piercing 2 lb.—
		weapons_.push_back(weapon(weaponName::Warhammer, 15, 1, 6, 1, martialMelee, bludgeon, magicBonus, { versatile })); //Warhammer 15 gp 1d8 bludgeoning 2 lb.Versatile(1d10)
		weapons_.push_back(weapon(weaponName::Whip, 2, 1, 6, 1, martialMelee, slash, magicBonus, { finesse, reach })); //Whip 2 gp 1d4 slashing 3 lb.Finesse, reach

		weapons_.push_back(weapon(weaponName::Longbow, 50, 1, 6, 1, martialRange, pierce, magicBonus, 150, 600, { wp_heavy, twohanded })); //Longbow 50 gp 1d8 piercing 2 lb.Ammunition(range 150 / 600), heavy, two - handed

	}
	int weaponCount = weapons_.size();
	for (int i = 0; i < weaponCount; i++) {
		map_.insert(std::make_pair(weapons_.at(i).getName(), i));
	}

}

//TODO Make const return value
weapon& weaponFactory::getWeapon(weaponName name, int magicBonus) {
	int index = magicBonus * 25 + map_[name];
	return weapons_.at(index);
}

weapon weaponFactory::getWeapon(int index) {
	return weapons_.at(index);
}

weaponFactory::~weaponFactory() {
	for (std::map<weaponName, int>::iterator itr = map_.begin(); itr != map_.end(); itr++)
	{
		//delete (itr);
	}
	map_.clear();
}
/* Cleric:
(a) a mace or (b) a warhammer (if proficient)
(a) scale mail, (b) leather armor, or (c) chain mail(if proficient)
(a) a light crossbow and 20 bolts or (b) any simple weapon
A shield and a holy symbol
*/

/*Fighter:
(a) chain mail or (b) leather, longbow, and 20 arrows
(a) a martial weapon and a shield or (b) two martial weapons
(a) a light crossbow and 20 bolts or (b) two handaxes
*/

/* Rogue
(a) a rapier or (b) a shortsword
(a) a shortbow and quiver of 20 arrows or (b) a shortsword
Leather armor, two daggers
*/

/* Wizard
(a) a quarterstaff or (b) a dagger
*/