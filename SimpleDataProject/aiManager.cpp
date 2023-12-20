#include "aiManager.h"
#include "character.h"

void aiManager::update(character* character)
{
	characterClass cc = character->getClass();
	if (cc == Cleric || cc == Wizard) {
		caster_.act(character);
	}

	if (cc == Fighter || cc == Rogue) {
		combater_.act(character);
	}

}