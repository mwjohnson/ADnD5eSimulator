#pragma once
#include <queue>
#include "spell.h"
#include "spellFactory.h"

class character;

class spellMessage {
public:
	spellMessage(const character* source, character* target, spell& spell) 
		: source_(source), target_(target), spell_(spell) { }
	character* getTarget() { return target_; }
	spell& getSpell() { return spell_; }
private:
	const character* source_;
	character* target_;
	spell& spell_;
};

class spellManager
{
public:
	spellManager(spellFactory& sf) : spellLibrary_(sf), spells_() { }
	void addSpellMessage(character* source, character* target, spell& sp);
	void update();
	spell& getSpell(spellName name);
	~spellManager() { }
private:
	spellFactory& spellLibrary_;
	std::queue<spellMessage> spells_;
};

