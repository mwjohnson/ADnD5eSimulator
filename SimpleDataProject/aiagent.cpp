#include "aiagent.h"
#include "character.h"
#include "Utils.h"
#include <cmath>
#include "damageManager.h"
#include "easylogging++.h"

void combatAgent::act(character* character) {
	me_ = character;
	bool done = false;

	int d = utils::getDistance(me_, me_->getTarget());
	if (!done && me_->hasRangedAmmo() && me_->getWeaponRange() < d) {
		approachHalf();
		rangeAttack();
		done = true;
	}
	
	if (!done) {
		if (std::abs(d) > me_->getMeleeAttackRange()) {
			if (me_->calculateMovement(me_->getMovement() / 2.0f) > std::abs(d)) {
				approachHalf();
				meleeAttack();
				done = true;
			}
			else {
				approach();
				done = true;
			}
		}
		else {
			meleeAttack();
		}
	}
} 

void casterAgent::act(character* character) {
	me_ = character;
	bool done = false;

	int d = utils::getDistance(me_, me_->getTarget());

	if (me_->getHighestAvailable() > 2) {
		if (!done && me_->hasSpells() && (me_->getClass() == Wizard || (me_->getClass() == Cleric && me_->getLevel() >= 3))) {
			castHighestDamage();
			done = true;
		}
	}

	if (!done && me_->hasRangedAmmo() && me_->getWeaponRange() < d) {
		rangeAttack();
		done = true;
	}

	if (!done) {
		if (std::abs(d) > me_->getMeleeAttackRange()) {
			if (me_->calculateMovement(me_->getMovement() / 2.0f) > std::abs(d)) {
				approachHalf();
				meleeAttack();
				done = true;
			}
			else {
				approach();
				done = true;
			}
		}
		else {
			meleeAttack();
		}
	}
}

void casterAgent::castHighestDamage() {
	me_->castHighestDamage();
}

void aiagent::moveUp(int d, int maxMove) {
	int move = (maxMove > std::abs(d)) ? std::abs(d) : maxMove;
#ifdef COMBAT_LOGGING 
	CLOG(INFO, "combat") << "Pre-Move Position: "<<me_->getPosition() << " Move Up: " << move;
	me_->move(move);
	CLOG(INFO, "combat") << "Post-Move Position: " << me_->getPosition();
#else
	me_->move(move);
#endif
}

void aiagent::moveDown(int d, int maxMove) {
	int move = (maxMove > std::abs(d)) ? std::abs(d)*-1 : maxMove*-1;
#ifdef COMBAT_LOGGING 
	CLOG(INFO, "combat") << "Pre-Move Position: " << me_->getPosition() << " Move Down: " << move;
	me_->move(move);
	CLOG(INFO, "combat") << "Post-Move Position: " << me_->getPosition();
#else
	me_->move(move);
#endif
}

void aiagent::approach()
{
#ifdef COMBAT_LOGGING 
	CLOG(INFO, "combat") << "Approach:";
#endif
	int maxMove = me_->getMovement();
	int d = utils::getDistance(me_, me_->getTarget());
	(d > 0) ? moveDown(d, maxMove) : moveUp(d, maxMove);
}

void aiagent::approachHalf()
{
#ifdef COMBAT_LOGGING 
	CLOG(INFO, "combat") << "Approach Half:";
#endif
	int maxMove = me_->getMovement();
	maxMove = static_cast<int>(std::round(maxMove / 2.0));
	int d = utils::getDistance(me_, me_->getTarget());
	(d > 0) ? moveDown(d, maxMove) : moveUp(d, maxMove);
}

void aiagent::retreat()
{
#ifdef COMBAT_LOGGING 
	CLOG(INFO, "combat") << "Retreat:";
#endif
	int maxMove = me_->getMovement();
	int d = utils::getDistance(me_, me_->getTarget());
	(d > 0) ? moveUp(d, maxMove) : moveDown(d, maxMove);
}

void aiagent::retreatHalf()
{
#ifdef COMBAT_LOGGING 
	CLOG(INFO, "combat") << "Retreat Half:";
#endif
	int maxMove = me_->getMovement();
	maxMove = static_cast<int>(std::round(maxMove / 2.0));
	int d = utils::getDistance(me_, me_->getTarget());
	(d > 0) ? moveUp(d, maxMove) : moveDown(d, maxMove);
}

void aiagent::rangeAttack()
{
	int numAttack = me_->getNumAttacks();
	for (int i = 0; i < numAttack; i++) {
#ifdef COMBAT_LOGGING 
		CLOG(INFO, "combat") << "Range Attack:";
#endif
		hitResult r = me_->rangeAttack();
		if (r == hitResult::Hit || r == hitResult::Double) {
			int dmg = (r == hitResult::Double) ? me_->getRangeDamage() * 2 : me_->getRangeDamage();
			dm_.addDamageMessage(me_, me_->getTarget(), dmg);
		}
		if (r == hitResult::Fail)
		{
			dm_.addDamageMessage(me_, me_, me_->getRangeDamage());
		}
	}
}

void aiagent::meleeAttack()
{
	int numAttack = me_->getNumAttacks();
	for (int i = 0; i < numAttack; i++) {
#ifdef COMBAT_LOGGING 
		CLOG(INFO, "combat") << "Melee Attack:";
#endif
		hitResult r = me_->meleeAttack();
		if (r == hitResult::Hit || r == hitResult::Double) {
			int dmg = (r == hitResult::Double) ? me_->getMeleeDamage() * 2 : me_->getMeleeDamage();
			dm_.addDamageMessage(me_, me_->getTarget(), dmg);
		}
		if (r == hitResult::Fail)
		{
			dm_.addDamageMessage(me_, me_, me_->getMeleeDamage());
		}
	}
}