#include "effect.h"
#include "damageManager.h"

void directDamage::apply(character& src, character& target, damageManager& dm, bool saved, utils& utils)
{
	int dmg = 0;
	for (int i = 0; i < damageDiceCount_; i++) {
		dmg += utils.getRandomUniform(1, damageDiceType_);
	}
	dmg += damageBonus_;
	dmg = (saved) ? dmg / 2.0 : dmg;
	dm.addDamageMessage(&src, &target, dmg);
}

void powerWordKill::apply(character& src, character& target, damageManager& dm, bool saved, utils& utils)
{
	if (target.getCurrentHitpoints() < 100) {
		dm.addDamageMessage(&src, &target, 100);
	}
}