#include "combatManager.h"
#include "character.h"
#include "easylogging++.h"
#include "combatResult.h"

void combatManager::initializeCombat(character& c1, character& c2)
{
	c1.setPosition(620);
	c2.setPosition(520);
	c1.setTarget(&c2);
	c2.setTarget(&c1);
}

int combatManager::rollInitiatives(character& c1, character& c2) {
	int cid = (c1.rollInitiative() > c2.rollInitiative()) ? 0 : 1;
	if (cid == 0) {
		if (c1.getClass() == Rogue) {
			c1.setSneakAttack(true);
		}
	}
	else {
		if (c2.getClass() == Rogue) {
			c2.setSneakAttack(true);
		}
	}
	return cid;
}

void combatManager::Combat_v1(character c1, character c2, combatResult& cri, combatResult& crj)
{
	bool finished = false;
	int loserid = -1;
	character* ca[2] = { &c1, &c2 };

	initializeCombat(*ca[0], *ca[1]);
	
	int indexFlipper = rollInitiatives(c1,c2);
	int cid = indexFlipper;
	int turn = 0;
	while (!finished) {

		//Someone is dead, winner.
		if (ca[cid]->isDead()) {
			finished = true;
			loserIndexId_ = cid;
		}

		// Someone escaped, Draw.
		if (utils::getDistance(ca[0], ca[1]) > 1000) {
			finished = true;
		}

		if (!finished) {
			aim_.update(ca[cid]);
			sm_.update();
			ca[cid]->getTarget()->processEffects(dm_);
			if (dm_.update(turn)) {
				finished = true;
			}
		}
#ifdef COMBAT_LOGGING 
		CLOG(INFO, "combat") << "End of Turn: " << turn;
#endif
		turn++;
		indexFlipper++;
		cid = indexFlipper % 2;
	}
	cri.setTurnCount(turn);
	cri.setMyHitTracker(ca[0]->getHitTracker());
	cri.setOpponentHitTracker(ca[1]->getHitTracker());
	cri.setWon(cid == 0);
	cri.setTurnCount(turn);

	crj.setTurnCount(turn);
	crj.setMyHitTracker(ca[1]->getHitTracker());
	crj.setOpponentHitTracker(ca[0]->getHitTracker());
	crj.setWon(cid == 1);
	crj.setTurnCount(turn);
}