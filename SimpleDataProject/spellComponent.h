#pragma once
#include <vector>
#include <algorithm>
#include "spell.h"
#include "spellManager.h"

class spellComponent
{
public:
	spellComponent() : sm_(nullptr) { }
	spellComponent(spellManager* sm) : sm_(sm) { }
	spellComponent(int* ss, spellManager* sm);
	~spellComponent() { }
	void init(int* ss, spellManager* sm);
	void castSpell(character* me, spellName name, character* target);
	//void addSpell(spell sp);
	//bool spellAvailable(const char* name) const;
	int getHighestAvailable() const;
	//spellComponent(const spellComponent& rhs);
	//spellComponent& spellComponent::operator=(const spellComponent& rhs);
private:
//	std::vector<spell> spells_; //Remove this....
	int spellsAvailable[10];
	int spellsUsed[10];
	spellManager* sm_;
};