#pragma once
#include "statcontainer.h"
#include "Utils.h"
#include "aiagent.h"
#include "armor.h"
#include "weapon.h"
#include "SpellComponent.h"

#include <iostream>
#include <crossguid/guid.hpp>
#include <vector>
#include "spell.h"

class damageManager;

enum hitResult {
	Fail,
	Miss,
	Hit,
	Double,
	hitResultCount
};

class hitTracker {
public:
	hitTracker() : fail_(0), miss_(0), hit_(0), double_(0) {}
	void track(hitResult r) {
		switch (r)
		{
		case Fail:
			fail_++;
			break;
		case Miss:
			miss_++;
			break;
		case Hit:
			hit_++;
			break;
		case Double:
			double_++;
			break;
		case hitResultCount:
		default:
			break;
		}
	}
	void print() {
		std::cout << "Fail: " << std::to_string(fail_) << " Miss: " << std::to_string(miss_) << " Hit: " << std::to_string(hit_) << " Double: " << std::to_string(double_) << "\n";
	}
	std::string printjson() const {
		std::ostringstream oss;
		oss << "\"Fail\": " + std::to_string(fail_) << ", \"Miss\": " + std::to_string(miss_) << ", \"Hit\": " + std::to_string(hit_) << ", \"Double\": " + std::to_string(double_);
		return oss.str();
	}

	int fail_;
	int miss_;
	int hit_;
	int double_;
};

class character
{
public:
	
	character(xg::Guid, statcontainer stats, int hp, characterClass chrClass, int lvl,
		const armor& main, const armor& shield, weapon& melee, weapon& ranged, int numattack, utils& util);

	bool isDead() const { return currentHitpoints_ <= 0; }
	bool isHeavilyDamaged() const { return currentHitpoints_ < maxHitpoints_*0.2f; }
	void receiveDamage(int damage);// { currentHitpoints_ -= damage; }
	
	int calculateMovement(int distance) const;

	void applyEffect(spell& s) { effects_.push_back(s); }
	void processEffects(damageManager& dm);

	void move(int distance);
	hitResult rangeAttack();
	hitResult meleeAttack();
	int rollInitiative() { return utils_.getRandomUniform(1, 20) + stats_.getModifier(stats::dexterity); }
	
	int getMeleeAttackRange() const { return 5; }
	int getRangeAttackRange() const { return ranged_.getNormalRange(); }
	int getMeleeDamage();
	int getRangeDamage();
	character* getTarget() { return target_; }
	int getAC() const;
	int getPosition() const { return position_; }
	int getMovement() const { return movement_; }
	bool hasRangedAmmo() const { return rangedAmmo_ > numAttacks_; }
	int getWeaponRange() const { return ranged_.getNormalRange(); }
	int getLevel() const { return level_; }
	characterClass getClass() const { return class_; }
	int getNumAttacks() const { return numAttacks_; }
	int getCurrentHitpoints() const { return currentHitpoints_; }

	int getHighestAvailable() const { return spellComponent_.getHighestAvailable(); }

	void setPosition(int pos) { position_ = pos; }
	void setTarget(character* target) { target_ = target; }

	int getClassStatModifier() const;
	int getSpellSaveDC() const;
	int getSpellAttackModifer() const;
	int sneakAttackBonusDamage();
	void setSneakAttack(bool) { sneakAttack_ = true; }
	void setSavingThrowProficiency(int index, stats stat) { savingThrowProficiencies_[index] = stat; }
	int getSavingThrowProficiency(stats stat);
//	void setSpellComponent(spellComponent* sc) { spellComponent_ = *sc; }

	void updateSpellComponent(int* ss, spellManager* sm);

	//void addSpell(spell s) { spellComponent_.addSpell(s); }

	bool hasSpells() const;
	void castHighestDamage();
	hitResult baseAttack(bool isRanged);
	int getDamage(int ddc, int ddt, int bonus);
	~character() { }
	void print();
	void printJson() const;
	std::string asString() const;
	std::string asFlatString() const;

	hitTracker getHitTracker() const { return hitTracker_; }
	
private:
	hitResult rollToHit(const weapon& weapon);
	int calculateRangedAmmo(characterClass chrclass) const;
	xg::Guid id_;
	statcontainer stats_;

	const armor& main_;
	const armor& shield_;
	const weapon& melee_;
	const weapon& ranged_;

	spellName wizspells_[10];
	spellName clespells_[10];
	
	spellName to_cast_;
	std::vector<spell> effects_;

	hitTracker hitTracker_;
	stats savingThrowProficiencies_[2];
	utils& utils_;
	character* target_;
	spellComponent spellComponent_;
	characterClass class_;

	int maxHitpoints_;
	int currentHitpoints_;
	int level_;
	int movement_;
	int position_;
	int rangedAmmo_;
	int meleeAttackRange_;
	int numAttacks_;

	bool sneakAttack_;
	bool isTemplate_;
};

