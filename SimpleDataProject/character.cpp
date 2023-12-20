#include "character.h"
#include "easylogging++.h"
#include "damageManager.h"
#include "spell.h"
#include "effect.h"
#include "statcontainer.h"
#include <cassert>
#include <cstring>
#include <math.h>

character::character(xg::Guid guid, statcontainer stats, int hp, characterClass chrClass, int lvl,
	const armor& main, const armor& shield, weapon& melee, weapon& ranged, int numattacks, utils & util)
	: id_(guid), stats_(stats), maxHitpoints_(hp), currentHitpoints_(hp), class_(chrClass), level_(lvl)
	, movement_(30), meleeAttackRange_(5), main_(main), shield_(shield), melee_(melee), ranged_(ranged)
	, to_cast_(spellName::NoneSpell), spellComponent_(nullptr), numAttacks_(numattacks), hitTracker_(), sneakAttack_(false), utils_(util)
	, isTemplate_(true)

{
	rangedAmmo_ = calculateRangedAmmo(chrClass);

	clespells_[0] = spellName::Sacred_Flame; // c (0)
	clespells_[1] = spellName::NoneSpell;
	clespells_[2] = spellName::Hold_Person; // 2
	clespells_[3] =spellName::Inflict_Wounds; // 3
	clespells_[4] =spellName::Guardian_of_Faith; // 4
	clespells_[5] =spellName::Flame_Strike; // 5
	clespells_[6] =spellName::Blade_Barrier; // 6
	clespells_[7] =spellName::Fire_Storm;  // 7
	clespells_[8] =spellName::Earthquake;  // 8
	clespells_[9] =spellName::Fire_Storm9; // 9

	wizspells_[0] = spellName::Fire_Bolt; // c (0)
	wizspells_[1] = spellName::Magic_Missile; // 1
	wizspells_[2] = spellName::Flaming_Sphere; // 2 //"Hold Person",
	wizspells_[3] = spellName::Fireball; // 3 //"Lightning Bolt",
	wizspells_[4] = spellName::Ice_Storm; // 4
	wizspells_[5] = spellName::Cone_of_Cold; // 5
	wizspells_[6] = spellName::Chain_Lightning; // 6
	wizspells_[7] = spellName::Finger_of_Death; // 7
	wizspells_[8] = spellName::Sunburst; // 8
	wizspells_[9] = spellName::Power_Word_Kill; // 9	
}

void character::updateSpellComponent(int* ss, spellManager* sm) {
	spellComponent_.init(ss, sm);
}

bool character::hasSpells() const
{
	if (-1 == spellComponent_.getHighestAvailable()) {
		return false;
	}
	return true;
}

void character::castHighestDamage()
{
	int lvl = spellComponent_.getHighestAvailable();
	if (class_ == Cleric) {
		to_cast_ = clespells_[lvl];
	}

	if (class_ == Wizard) {
		to_cast_ = wizspells_[lvl];
	}
	spellComponent_.castSpell(this, to_cast_, getTarget());
}

void character::receiveDamage(int damage) {
	currentHitpoints_ -= damage;
}

int character::calculateRangedAmmo(characterClass chrclass) const {
	switch (chrclass)
	{
	case Fighter:
	case Rogue:
	case Cleric:
		return 20;
		break;
	case Wizard:
		return 1;
		break;
	case characterClassCount:
	default:
		assert(false);
		break;
	}
	return 0;
}

int character::calculateMovement(int distance) const {
	int maxMove = 30;
	return maxMove;
}

int character::getSavingThrowProficiency(stats stat) {
	if (savingThrowProficiencies_[0] == stat || savingThrowProficiencies_[1] == stat) {
		return utils::getProficiencyBonus(level_);
	}
	return 0;
}

void character::processEffects(damageManager& dm)
{
	for (int i = 0; i < effects_.size(); i++) {
		bool savingthrow = false;
		spell& sp = effects_.at(i);
		if (sp.hasSavingThrow()) {
			stats st = sp.getSavingStat();
			int dc = this->getTarget()->getSpellSaveDC(); // The caster sets the spell-"attack" dc.
			int roll = utils_.getRandomUniform(1, 20) + getSavingThrowProficiency(st);
			savingthrow = roll >= dc;
		}

		effect* e = sp.getEffect();
		e->apply(*(this->target_), *this, dm, savingthrow, utils_);
	}
	effects_.clear();
}

void character::move(int distance)
{
	//int adjustedDistance = calculateMovement(distance);
	position_ += distance;
}

hitResult character::baseAttack(bool isRanged) { // TODO STOP COPYING WEAPON
	hitResult r = (isRanged) ? rollToHit(ranged_) : rollToHit(melee_);
	hitTracker_.track(r);
	return r;
}

hitResult character::rangeAttack()
{
	rangedAmmo_ -= 1;
	if (rangedAmmo_ < 0) { assert(false); }
	return baseAttack(true);
}

hitResult character::meleeAttack() {
	return baseAttack(false);
}
int character::getMeleeDamage() {
	int ddc = melee_.getDamageDiceCount();
	int ddt = melee_.getDamageDiceType();
	int bonus = melee_.getBonus();
	return getDamage(ddc, ddt, bonus);
}

int character::getRangeDamage() {
	int ddc = ranged_.getDamageDiceCount();
	int ddt = ranged_.getDamageDiceType();
	int bonus = ranged_.getBonus();
	return getDamage(ddc, ddt, bonus);
}

int character::getDamage(int ddc, int ddt, int bonus) {
	int dmg = 0;
	for (int i = 0; i < ddc; i++) {
		dmg += utils_.getRandomUniform(1, ddt);
	}
	if (sneakAttack_) {
		bonus += sneakAttackBonusDamage();
	}
	return dmg + bonus;
}

hitResult character::rollToHit(const weapon& weapon) {
	int r = utils_.getRandomUniform(1, 20);
	if (r == 20) { return hitResult::Double; }
	if (r == 1) { return hitResult::Fail; }
	int mod = (class_ == Rogue && weapon.hasFinesse()) ? stats_.getModifier(stats::dexterity) : stats_.getModifier(stats::strength);
#ifdef COMBAT_LOGGING 
	int roll = r + mod + utils::getProficiencyBonus(level_);
	bool hit = roll >= target_->getAC();
	const char* result = (hit) ? ": Hit" : ": Miss";
	CLOG(INFO, "combat") << "|" << id_.str().substr(0, 6) << "| -- To Hit: " << roll << result;
	return (hit) ? hitResult::Hit : hitResult::Miss;
#else
	return (r + mod + utils::getProficiencyBonus(level_) >= target_->getAC()) ? hitResult::Hit : hitResult::Miss;
#endif
}

int character::getAC() const {
	int dexmod = stats_.getModifier(stats::dexterity);
	int mainac = main_.getTotalAC();
	switch (main_.getType())
	{
	case light:
		break;
	case medium:
		dexmod = (dexmod > 2) ? 2 : dexmod;
		break;
	case heavy:
		dexmod = 0;
		break;
	case shield:
	case armorTypeCount:
	default:
		break;
	}
	int dexmodac = dexmod;
	int shieldac = shield_.getTotalAC();

	int ac = mainac + dexmodac + shieldac;
	if (ac > 30) {

		assert(ac < 30, "AC TOO high.");
	}
	return ac;
}

int character::getClassStatModifier() const {
	switch (class_)
	{
	case Fighter:
		return 0;
		break;
	case Rogue:
		return 0;
		break;
	case Cleric:
		return stats_.getModifier(stats::wisdom);
		break;
	case Wizard:
		return stats_.getModifier(stats::intelligence);
		break;
	case characterClassCount:
	default:
		assert(false);
		break;
	}
	return 0;
}

int character::sneakAttackBonusDamage() {

	int ddc = ceil(static_cast<float>(level_) / 2.0f);
	int dmg = 0;
	for (int i = 0; i < ddc; i++) {
		dmg += utils_.getRandomUniform(1, 6);
	}
	sneakAttack_ = false;
	return dmg;
}

int character::getSpellSaveDC() const
{
	return 8 + utils::getProficiencyBonus(level_) + getClassStatModifier();
}

int character::getSpellAttackModifer() const
{
	return utils::getProficiencyBonus(level_) + getClassStatModifier();
}

std::string character::asFlatString() const {
	std::ostringstream oss;
	oss << "{ \"Id\": \"" << id_.str() << "\", " << "\"Class\": \"" << classNames[static_cast<int>(class_)] << "\", ";
	oss << "\"Hp\": " << maxHitpoints_ << ", " << "\"Lvl\": " << level_ << ", ";
	oss << "\"AC\": " << getAC() << ", " << "\"Move\": " << movement_ << ", " << "\"Atk\": " << numAttacks_ << ", ";
	oss << stats_.asString();
	oss << "\"Mle\": \"" << melee_.printjson() << "\", " << "\"Ran\": \"" << ranged_.printjson() << "\", ";
	oss << "\"Ammo\": " << rangedAmmo_ << ", " << "\"Arm\": \"" << main_.printjson() << "\", " << "\"Shld\": \"" << shield_.printjson() << "\", " << hitTracker_.printjson();
	return oss.str();
}


std::string character::asString() const {
	std::ostringstream oss;
	oss << "{ \"Id\": \"" << id_.str() << "\", " << "\"Class\": \"" << classNames[static_cast<int>(class_)] << "\", ";
	oss << "\"Hp\": " << maxHitpoints_ << ", " << "\"Lvl\": " << level_ << ", ";
	oss << "\"AC\": " << getAC() << ", " << "\"Move\": " << movement_ << ", " << "\"Atk\": " << numAttacks_ << ", ";
	oss << stats_.asString();
	oss << "\"Mle\": \"" << melee_.printjson() << "\", " << "\"Ran\": \"" << ranged_.printjson() << "\", ";
	oss << "\"Ammo\": " << rangedAmmo_ << ", " << "\"Arm\": \"" << main_.printjson() << "\", " << "\"Shld\": \"" << shield_.printjson() << "\", " << hitTracker_.printjson() << " }";
	return oss.str();
}

void character::printJson() const {
	std::cout << "{ \"Id\": \"" << id_.str() << "\", " << "\"Class\": \"" << classNames[static_cast<int>(class_)] << "\", ";
	std::cout << "\"Hp\": " << maxHitpoints_ << ", " << "\"Lvl\": " << level_ << ", ";
	std::cout << "\"AC\": " << getAC() << ", " << "\"Move\": " << movement_ << ", " << "\"Atk\": " << numAttacks_ << ", ";
	stats_.printjson();
	std::cout << "\"Mle\": \"" << melee_.printjson() << "\", " << "\"Ran\": \"" << ranged_.printjson() << "\", ";
	std::cout << "\"Ammo\": " << rangedAmmo_ << ", " << "\"Arm\": \"" << main_.printjson() << "\", " << "\"Shld\": \"" << shield_.printjson() << "\", " << hitTracker_.printjson() << " }";
}

void character::print() {
	std::cout << "Id: " << id_.str() << "\n";
	std::cout << "Class: " << classNames[static_cast<int>(class_)] << "\n";
	std::cout << "Hp: " << getCurrentHitpoints() << " / " << maxHitpoints_ << "\n";
	std::cout << "Lvl: " << level_ << "\n";
	std::cout << "AC: " << getAC() << "\n";
	std::cout << "Pos: " << position_ << " Move: " << movement_ << "\n";
	std::cout << "Atk: " << numAttacks_ << "\n";
	stats_.print();
	std::cout << "Weap: \n";
	std::cout << "Mle: ";
	melee_.print();
	std::cout << "Ran: ";
	std::cout << "Ammo: " << rangedAmmo_ << "\n";
	ranged_.print();
	std::cout << "Arm:\n";
	std::cout << "Arm: ";
	main_.print();
	std::cout << "Shld: ";
	shield_.print();
	hitTracker_.print();
}