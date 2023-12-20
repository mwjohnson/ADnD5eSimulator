#include "spellComponent.h"
#include "easylogging++.h"

spellComponent::spellComponent(int* ss, spellManager* sm) : sm_(sm)
{
	for (int i = 0; i < 10; i++) {
		spellsAvailable[i] = ss[i];
	}
	spellsUsed[10] = { 0 };
}
void spellComponent::init(int* ss, spellManager* sm) {
	for (int i = 0; i < 10; i++) {
		spellsAvailable[i] = ss[i];
	}
	spellsUsed[10] = { 0 };
	sm_ = sm;
}

int spellComponent::getHighestAvailable() const {
	int highest = -1;
#ifdef COMBAT_LOGGING 
	std::string sps = "";
#endif
	for (int i = 0; i < 10; i++) {
		int AvailMinusUsed = (spellsAvailable[i] - spellsUsed[i]);
#ifdef COMBAT_LOGGING 
		sps += std::to_string(AvailMinusUsed) + " ";
#endif
		if (0 < AvailMinusUsed) {
			highest = i;
		}
	}
#ifdef COMBAT_LOGGING 
	CLOG(INFO, "combat") << "Spell Level AVailable: " << sps.c_str();
#endif
	return highest;
}

void spellComponent::castSpell(character* me, spellName name, character* target) {
	spell& sp = sm_->getSpell(name);
	spellsUsed[sp.getLevel()] += 1;
	sm_->addSpellMessage(me, target, sp);
}