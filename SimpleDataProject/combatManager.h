#pragma once

#include "aiManager.h"
#include "DamageManager.h"
#include "combatResult.h"
class character;

class combatManager
{
public:
	combatManager(aiManager& aim, damageManager& dm, spellManager& sm) : aim_(aim), dm_(dm), sm_(sm) {}

	void initializeCombat(character& c1, character& c2);
	int rollInitiatives(character& c1, character& c2);
	void Combat_v1(character c1, character c2, combatResult& cri, combatResult& crj);

	int getLoserIndexId() const { return loserIndexId_; }
private:
	aiManager& aim_;
	damageManager& dm_;
	spellManager& sm_;
	int loserIndexId_;
};

