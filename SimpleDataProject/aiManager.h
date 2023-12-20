#pragma once
#include "aiagent.h"
class character;

class aiManager
{
public:
	//aiManager() : caster_(nullptr), combater_(nullptr) {}
	aiManager(damageManager& dm) : caster_(dm), combater_(dm) {}
	void update(character* character);
	~aiManager() {}
private:
	casterAgent caster_;
	combatAgent combater_;
};