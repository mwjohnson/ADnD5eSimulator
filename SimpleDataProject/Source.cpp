#include <iostream>
#include <ctime>

#include "characterFactory.h"
#include "combatManager.h"
#include "spellManager.h"
#include "weaponFactory.h"
#include "armorFactory.h"
#include "spellFactory.h"
#include "Utils.h"
#include "easylogging++.h"
#include "combatResult.h"
#include "optionparser.h"
#include <fstream>

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;
INITIALIZE_EASYLOGGINGPP


#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#ifdef _DEBUG
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif


struct combatTracker {
public:
	combatTracker() : win_(0), lose_(0), draw_(0) { }
	combatTracker(int w, int lo, int d) : win_(w), lose_(lo), draw_(d), cr_() { }
	void win() { win_++; }
	void lose() { lose_++; }
	void draw() { draw_++; }
	void printJson() { std::cout << "{\"W\": " << win_ << ", \"L\": " << lose_ << ", \"D\": " << draw_ << " }"; }
	std::string asString() const {
		std::ostringstream oss;
		oss << "{\"W\": " << win_ << ", \"L\": " << lose_ << ", \"D\": " << draw_ << " }";
		return oss.str();
	}

	std::string asCombatResultFlatString() const {
		std::ostringstream oss;
		oss << "\"crs\": [";
		std::vector<combatResult>::const_iterator endItr = cr_.end();
		for (std::vector<combatResult>::const_iterator itr = cr_.begin(); itr != cr_.end();++itr){
			oss << itr->asFlatString();
			if (itr != endItr-1) { oss << ", "; }
		}
		oss << "], ";
		oss << "\"W\": " << win_ << ", \"L\": " << lose_ << ", \"D\": " << draw_ << " }";
		return oss.str();
	}

	std::string asSimpleFlatString() const {
		std::ostringstream oss;
		oss << "\"W\": " << win_ << ", \"L\": " << lose_ << ", \"D\": " << draw_ << " }";
		return oss.str();
	}

	int getTotalFights() const { return win_ + lose_ + draw_; }

	std::vector<combatResult> cr_;
	int win_;
	int lose_;
	int draw_;
};


void print(std::map<int, combatTracker>& winTracker_, std::vector<character*>& characters) {
	for (std::map<int, combatTracker>::iterator it = winTracker_.begin(); it != winTracker_.end(); ++it) {
		std::string s2 = "{\"character\":";
		characters.at(it->first)->printJson();
		std::cout << "\n";
		it->second.printJson();
		std::cout << "\n";
	}
}

void printToFile(std::map<int, combatTracker>& winTracker_, std::vector<character*>& characters) {

	ofstream myfile("CombatOutput.data");
	if (myfile.is_open())
	{
		std::map<int, combatTracker>::reverse_iterator endItr = winTracker_.rbegin();
		myfile << "{\"data\":[ ";
		for (std::map<int, combatTracker>::iterator it = winTracker_.begin(); it != winTracker_.end(); ++it) {
			myfile << "{\"character\":";
			myfile << characters.at(it->first)->asString();
			myfile << ", \"record\": ";
			myfile << it->second.asString();
			myfile << "}";
			if (it->first != endItr->first) {
				myfile << ",";
			}
		}
		myfile << "]}";
		myfile.close();
	}
}

void printFlatToFile(std::map<int, combatTracker>& winTracker_, std::vector<character*>& characters, bool printCombatResults) {

	ofstream myfile("CombatOutputFlat.data");
	if (myfile.is_open())
	{
		std::map<int, combatTracker>::reverse_iterator endItr = winTracker_.rbegin();
		myfile << "{\"data\":[ ";
		for (std::map<int, combatTracker>::iterator it = winTracker_.begin(); it != winTracker_.end(); ++it) {
			myfile << characters.at(it->first)->asFlatString();
			myfile << ",";

			if (printCombatResults) {
				myfile << it->second.asCombatResultFlatString();
				if (it->first != endItr->first) {
					myfile << ",";
				}
			}
			else {
				myfile << it->second.asSimpleFlatString();
				if (it->first != endItr->first) {
					myfile << ",";
				}
			}
		}
		myfile << "]}";
		myfile.close();
	}
}

void initializeLoggers() {

	// Load configuration from file
	el::Configurations conf("C:/Users/Matt/Documents/Visual Studio 2015/Projects/SimpleDataProject/SimpleDataProject/LoggingConfig.conf");
	// Reconfigure single logger
	el::Loggers::reconfigureLogger("default", conf);
	// Actually reconfigure all loggers instead
	el::Loggers::reconfigureAllLoggers(conf);
	// Now all the loggers will use configuration from file
	el::Logger* combatLogger = el::Loggers::getLogger("combat");

	el::Configurations combatconf("C:/Users/Matt/Documents/Visual Studio 2015/Projects/SimpleDataProject/SimpleDataProject/combatLog.conf");
	el::Loggers::reconfigureLogger("combat", combatconf);
}


void generateCharacters(std::vector<character*>& characters,
	spellManager& sm,
	int charactercount,
	utils& utils,
	armorFactory& af,
	weaponFactory& wf,
	spellFactory& sf
	) {

	characterFactory cf = characterFactory(af, wf, sf, sm, utils);

	int start_s = clock();
	for (size_t i = 0; i < charactercount; i++)
	{
		characters.push_back(cf.GenerateCharacter());
	}
	int stop_s = clock();
	LOG(INFO) << "Character Count: " << charactercount;
	LOG(INFO) << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000;
}

void recordDraw(std::map<int, combatTracker>& winTracker, int id1, int id2) {
	(winTracker.count(id1)) ? winTracker.at(id1).draw() : winTracker.insert(std::make_pair(id1, combatTracker(0, 0, 1)));
	(winTracker.count(id2)) ? winTracker.at(id2).draw() : winTracker.insert(std::make_pair(id2, combatTracker(0, 0, 1)));
}


void run() {
	initializeLoggers();

	const int charactercount = 501;
	utils* utilsInstance = new utils();

	armorFactory af = armorFactory(*utilsInstance);
	weaponFactory wf = weaponFactory();
	spellFactory sf = spellFactory();

	spellManager sm = spellManager(sf);
	
	std::vector<character*> characters = std::vector<character*>();
	characters.reserve(charactercount);
	
	generateCharacters(characters, sm, charactercount, *utilsInstance, af, wf, sf);
	
	int start_s = clock();
	int fightCounter = 0;
	std::map<int, combatTracker> winTracker = map<int, combatTracker>();

	damageManager dm = damageManager();
	aiManager aim = aiManager(dm);
	combatManager cm = combatManager(aim, dm, sm);


	int levelCount[20] = { 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 };

	for (int i = 0; i < charactercount; i++) {
		levelCount[characters.at(i)->getLevel() - 1]++;
	}

	int CTcount[20] = { 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0 };
	for (int i = 0; i < 20; i++) {
		for (int k = i - 5; k <= i + 5; k++) {
			if (k >= 0 && k < 20) {
				CTcount[i] += levelCount[k];
			}
		}
	}

	//int test[20];
	for (int i = 0; i < charactercount; i++) {
		winTracker.insert(std::make_pair(i, combatTracker(0, 0, 0)));
		winTracker.at(i).cr_.reserve(charactercount - 1);
		int index = characters.at(i)->getLevel() - 1;
		int mz = CTcount[index];
		for (int k = 0; k < mz-1; k++) {
			winTracker.at(i).cr_.emplace_back(combatResult());
			//test[index] = mz;
		}
	}

	


	for (int i = 0; i < charactercount; i++) {
		for (int j = i; j < charactercount; j++) {
			if (i != j && characters.at(i)->getLevel() >= characters.at(j)->getLevel() - 5 && characters.at(i)->getLevel() <= characters.at(j)->getLevel() + 5) {
				
				fightCounter++;
				int icurrentCombat = winTracker.at(i).getTotalFights();
				int jcurrentCombat = winTracker.at(j).getTotalFights();
				winTracker.at(i).cr_.at(icurrentCombat).setOpponentLevel(characters.at(j)->getLevel());
				winTracker.at(i).cr_.at(icurrentCombat).setOpponentClass(characters.at(j)->getClass());
				winTracker.at(j).cr_.at(jcurrentCombat).setOpponentLevel(characters.at(i)->getLevel());
				winTracker.at(j).cr_.at(jcurrentCombat).setOpponentClass(characters.at(i)->getClass());

				cm.Combat_v1(*characters.at(i), *characters.at(j), winTracker.at(i).cr_.at(icurrentCombat), winTracker.at(j).cr_.at(jcurrentCombat));
				int loserid = (cm.getLoserIndexId() == 0) ? i : j;
				int winnerid = (cm.getLoserIndexId() == 0) ? j : i;

				if (loserid != -1) {
					winTracker.at(loserid).lose();
					winTracker.at(winnerid).win();
				}
				else {
					winTracker.at(loserid).draw();
					winTracker.at(winnerid).draw();
				}
			}
		}
	}
	int stop_s = clock();
	LOG(INFO) << "time: " << (stop_s - start_s) / double(CLOCKS_PER_SEC) * 1000;

	printFlatToFile(winTracker, characters, true);

	LOG(INFO) << "Fight Counter: " << fightCounter;
	for (std::vector<character*>::iterator i = characters.begin(); i != characters.end(); ++i) {
		delete *i;
	}
	delete utilsInstance;
}

int main(int argc, char* argv[]) {

	run();
	_CrtDumpMemoryLeaks();
	return 0;
}