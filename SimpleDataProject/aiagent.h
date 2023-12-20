#pragma once

class character;
class damageManager;

class aiagent
{
public:
	aiagent(damageManager& dm) : me_(nullptr), dm_(dm) {}
	

	virtual void act(character* character) {}

	void approach();
	void approachHalf();
	void retreat();
	void retreatHalf();

	void rangeAttack();
	void meleeAttack();
	
	~aiagent() { }
protected:

	void moveUp(int d, int maxMove);
	void moveDown(int d, int maxMove);

	character* me_;
	damageManager& dm_;
};

class combatAgent : public aiagent
{
public:
	combatAgent(damageManager& dm) : aiagent(dm) {}
	void act(character* character) override;
};

class casterAgent : public aiagent
{
public:
	casterAgent(damageManager& dm) : aiagent(dm) {}
	void act(character* character) override;
	void castHighestDamage();
};
