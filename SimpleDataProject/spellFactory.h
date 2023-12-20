#pragma once
#include <vector>
#include <map>
#include <string>
#include "spell.h"
class spellFactory
{
public:
	spellFactory() : spellLibrary_(), map_() { populate(); }
	~spellFactory();
	spell& getSpell(spellName name);
private:
	void populate();
	std::vector<spell> spellLibrary_;
	std::map<spellName, int> map_;
};

