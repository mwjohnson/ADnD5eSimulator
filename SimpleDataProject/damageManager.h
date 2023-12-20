#pragma once
#include "character.h"
#include <queue>

//todo: replace with proto buffer.
class damageMessage {
public:
	damageMessage(const character* source, character* target, int damage) : source_(source), target_(target), damage_(damage) {}
	character* getTarget() { return target_; }
	int getDamage() { return damage_; }
private:
	const character* source_;
	character* target_;
	int damage_;
};

class damageManager
{
public:
	damageManager() : damages_(), lastRoundDamage_(0) {}

	void addDamageMessage(character* source, character* target, int damage);

	bool update(int turn); // We don't permit 30+ rounds of no damage.
	~damageManager() {}

private:
	std::queue<damageMessage> damages_;
	int lastRoundDamage_;
};

