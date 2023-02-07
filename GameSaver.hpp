/*
 * GameSaver.hpp
 *
 *  Created on: 14.01.2023
 *      Author: uwe-w
 */

#ifndef GAMESAVER_HPP_
#define GAMESAVER_HPP_
#include "GameMain.hpp"
#include <string>
#include <fstream>
#include <map>
class json; class Citizen;class GameMain;

struct FieldJson{
	int x,y;
	int landscape;
	bool hasSpecialResource;
	bool hasShield;
	bool hasFortress;
	int roadStatus;
	bool isMined;
	bool isPolluted;
	bool isIrrigated;
	int continentID;
	int layer;
	int turnsUntilSwamped;
	std::vector<std::map<std::string, int>> nationKnowsData;
	//does not work. irrigation
	std::vector<int> figureIDs;
};

struct NationJson{
	int government;
	int nation;
	std::string leaderName;
	std::vector<int> figureIDs;
	std::vector<int> otherFigureIDs;
	short unsigned int taxRate;
	short unsigned int luxuriesRate;
	int treasury;
	std::vector<int> technologies;
	int whatToExplore;
	int explorationProgress;
	int difficulty;
	std::vector<int> embassies;
};

struct CityJson{
	int size;
	int globalIndex;
	std::vector<int> improvements;
	std::vector<int> figureIDs;
	std::string name;
	int x;
	int y;
	int shields;
	int food;
	std::vector<int> tradeRoutePartners;
	//there should be something about visibility
};

struct CitizenJson{
	int x;
	int y;
	int homeIndex;
	int state;
};

struct GameJson{
	unsigned int yearNumber;
	int randomSeed;
	unsigned int wonderDataSize;
	int nationAtCurrentTurnIndex;
	int climateState;
	int pollutionCount;
};

struct GameMainJson{
	int topLeftX;
	int topLeftY;
	int whatToMove;
	int blinkingIntervalTime;
};

struct FileFail{std::string whatHasFailed;};
class GameSaver {
private:
	std::string m_slotname;
	std::string m_filename;
	std::fstream& m_stream;
	GameMain& m_gameMain;
public:
	GameSaver(std::string slotname, std::string filename, std::fstream& stream, GameMain& gameMain):m_slotname(slotname),m_filename(filename),m_stream(stream),m_gameMain(gameMain){}
	virtual ~GameSaver();
	void m_saveGame(GameMain& gameMain);
	void m_printGlobalVariables();
	void m_printFields();
	void m_printAField(Field& field, std::ostream& os);
	void m_printNations();
	void m_printANation(Nation& nation, std::ostream& os);
	void m_printFigures();
	void m_printACity(City& city, std::ostream& os);
	void m_printCities();
	void m_printACitizen(Citizen& citizen, std::ostream& os);
	void m_printGame();
	void m_printGameMain();
};



void from_json(NationJson& nj, json& j);

#endif /* GAMESAVER_HPP_ */

