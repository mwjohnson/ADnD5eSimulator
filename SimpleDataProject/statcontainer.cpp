#include "statcontainer.h"

statcontainer::statcontainer(int str, int con, int dex, int intl, int wis, int chr)
{
	stats_[strength] = str;
	stats_[constitution] = con;
	stats_[dexterity] = dex;
	stats_[intelligence] = intl;
	stats_[wisdom] = wis;
	stats_[charisma] = chr;
}
statcontainer::~statcontainer()
{
}
