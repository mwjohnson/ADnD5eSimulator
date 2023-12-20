#include "armor.h"
#include <iostream>
#include <sstream>

void armor::print() const {
	std::cout << armorNames[static_cast<int>(name_)] << " ac: " << ac_ << "+" << magicBonus_ << "\n";
}

std::string armor::printjson() const {
	std::ostringstream oss;
	oss << armorNames[static_cast<int>(name_)] << " " + std::to_string(ac_)<<"+"+ std::to_string(magicBonus_);
	return oss.str();
}