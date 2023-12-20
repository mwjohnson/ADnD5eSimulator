#pragma once
#include "character.h"
#include "Utils.h"

class damageManager;

class effect
{
public:
	effect() {}
	~effect() {}
	//virtual void apply(character& src, character& target) {}
	virtual void apply(character& src, character& target, damageManager& dm, bool saved, utils& utils) = 0;// {}
	virtual effect* clone() const = 0;

};

class heal : public effect {
public:
	heal(int hdc, int hdt, int bonus) : healDiceCount_(hdc), healDiceType_(hdt), healBonus_(bonus) { }
	void apply(character& src, character& target, damageManager& dm, bool saved, utils& utils) {}
	virtual heal* clone() const
	{
		return new heal(*this);
	}
private:
	int healDiceCount_;
	int healDiceType_;
	int healBonus_;
};
class directDamage : public effect {
public:
	directDamage(int ddc, int ddt, int bonus) : damageDiceCount_(ddc), damageDiceType_(ddt), damageBonus_(bonus) { }
	void apply(character& src, character& target, damageManager& dm, bool saved, utils& utils);
	virtual directDamage* clone() const
	{
		return new directDamage(*this);
	}
	directDamage(const directDamage& dd) {
		damageDiceCount_ = dd.damageDiceCount_;
		damageDiceType_ = dd.damageDiceType_;
		damageBonus_ = dd.damageBonus_;
	}
private:
	int damageDiceCount_;
	int damageDiceType_;
	int damageBonus_;
};

class doubleDirectDamage : public effect {
public:
	doubleDirectDamage(int ddc0, int ddt0, int ddc1, int ddt1) : damageDiceCount0_(ddc0), damageDiceType0_(ddt0)
		, damageDiceCount1_(ddc1), damageDiceType1_(ddt1) { }
	void apply(character& src, character& target, damageManager& dm, bool saved, utils& utils) {}
	virtual doubleDirectDamage* clone() const
	{
		return new doubleDirectDamage(*this);
	}
	doubleDirectDamage(const doubleDirectDamage& e) {
		damageDiceCount0_ = e.damageDiceCount0_;
		damageDiceType0_ = e.damageDiceType0_;
		damageDiceCount1_ = e.damageDiceCount1_;
		damageDiceType1_ = e.damageDiceType1_;
	}
private:
	int damageDiceCount0_;
	int damageDiceType0_;
	int damageDiceCount1_;
	int damageDiceType1_;
};

class paralyze : public effect {
public:
	paralyze(int duration) : duration_(duration) { }
	void apply(character& src, character& target, damageManager& dm, bool saved, utils& utils) {}
	virtual paralyze* clone() const
	{
		return new paralyze(*this);
	}
	paralyze(const paralyze& e) {
		duration_ = e.duration_;
	}
private:
		int duration_;
};

class powerWordKill : public effect {
public:
	powerWordKill() {}
	void apply(character& src, character& target, damageManager& dm, bool saved, utils& utils);
	virtual powerWordKill* clone() const
	{
		return new powerWordKill(*this);
	}
	powerWordKill(const powerWordKill& pwk) {

	}
};