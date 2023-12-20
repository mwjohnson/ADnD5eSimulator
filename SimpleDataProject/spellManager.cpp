#include "spellManager.h"
#include "spell.h"
#include "easylogging++.h"
#include "character.h"

void spellManager::addSpellMessage(character* source, character* target, spell& sp)
{
#ifdef COMBAT_LOGGING 
	CLOG(INFO, "combat") << "Spell: " << sp->getName().c_str();
#endif
	spells_.push(spellMessage(source, target, sp));
	//spells_.push_back(spellMessage(source, target, sp));
}

spell& spellManager::getSpell(spellName name) {
	return spellLibrary_.getSpell(name);
}

void spellManager::update() {
	if (!spells_.empty()) {
		spellMessage& s = spells_.front();
		s.getTarget()->applyEffect(s.getSpell());
		spells_.pop();
	}
}

