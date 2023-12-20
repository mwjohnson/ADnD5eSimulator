#pragma once
#include "statcontainer.h"
#include "Utils.h"
#include <string>

class effect;


class spell
{
public:
	spell(spellName name, effect* eff, stats saveStat, int lvl, int rng, bool hst);
	spell(const spell& sp);
	//spell(spell&& sp) noexcept;
	~spell();

	int getLevel() const { return level_; }
	spellName getName() const { return name_; }
	effect* getEffect();

	bool hasSavingThrow() const { return hasSavingThrow_; }
	stats getSavingStat() const { return savingStat_; }
	int getRange() const { return range_; }

private:
	spellName name_;
	stats savingStat_;
	effect* effect_;
	int level_;
	int range_;
	bool hasSavingThrow_;
};

