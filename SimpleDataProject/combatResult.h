#pragma once

#include "character.h"

class combatResult
{
public:
	combatResult() : opponentClass_(), opponentLevel_(0), turnCount_(0), won_() {}
	~combatResult() {}
	void setTurnCount(int count) { turnCount_ = count; }
	void setMyHitTracker(hitTracker& ht) { myHitTracker_ = ht; }
	void setOpponentHitTracker(hitTracker& ht) { opponentHitTracker_ = ht; }
	void setOpponentLevel(int lvl) { opponentLevel_ = lvl; }
	void setOpponentClass(characterClass chrclass) { opponentClass_ = chrclass; }
	void setWon(bool val) { won_ = val; }

	bool isEmpty() { return opponentLevel_ == 0 && won_ == 0 && turnCount_ == 0 && opponentClass_ == 0; }
	

	std::string asFlatString() const {
		std::ostringstream oss;
		myHitTracker_.printjson();
		opponentHitTracker_.printjson();
		oss << "{ \"ocls\": " << opponentClass_ << ", \"olvl\": " << opponentLevel_ << ", \"tn\": " << turnCount_ << ", \"w\": " << won_ << " }";
		return oss.str();
	}

private:
	hitTracker myHitTracker_;
	hitTracker opponentHitTracker_;
	characterClass opponentClass_;
	int opponentLevel_;
	int turnCount_;
	bool won_;
};

