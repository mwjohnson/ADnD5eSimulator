#include "damageManager.h"
#include "easylogging++.h"

void damageManager::addDamageMessage(character* source, character* target, int damage)
{
#ifdef COMBAT_LOGGING 
	CLOG(INFO, "combat") << "Damage: " << damage;
#endif
	damages_.push(damageMessage(source, target, damage));
}

bool damageManager::update(int turn) {
	if (!damages_.empty()) {
		damageMessage& d = damages_.front();
#ifdef COMBAT_LOGGING 
		CLOG(INFO, "combat") << "Pre Damage Target HP: " << d.getTarget()->getCurrentHitpoints();
#endif
		d.getTarget()->receiveDamage(d.getDamage());
		damages_.pop();
		lastRoundDamage_ = turn;
#ifdef COMBAT_LOGGING 
		CLOG(INFO, "combat") << "Post Damage Target HP: " << d.getTarget()->getCurrentHitpoints();
#endif
	}
	
	return turn - lastRoundDamage_ > 30;
}