#include "spell.h"
#include "effect.h"


spell::spell(spellName name, effect* eff, stats saveStat, int lvl, int rng, bool hst) :
	name_(name),
	savingStat_(saveStat),
	effect_(eff),
	level_(lvl),
	range_(rng),
	hasSavingThrow_(hst) 
{
}

effect* spell::getEffect() 
{ return effect_; }

/*spell::spell(spell&& sp) noexcept {
	name_ = sp.name_;
	savingStat_ = sp.savingStat_;
	level_ = sp.level_;
	range_ = sp.range_;
	hasSavingThrow_ = sp.hasSavingThrow_;

	*effect_ = *sp.effect_;
	//Allocate the memory first
	sp.effect_ = nullptr;
}*/


spell::spell(const spell& sp) {

	name_ = sp.name_;
	savingStat_ = sp.savingStat_;
	level_ = sp.level_;
	range_ = sp.range_;
	hasSavingThrow_ = sp.hasSavingThrow_;

	//Allocate the memory first
	//effect_ = new effect();

	//Then copy the value from the passed object
	//effect_ = new effect(*sp.effect_);

	effect_ = sp.effect_->clone();
}

spell::~spell() {
	delete effect_;
}
