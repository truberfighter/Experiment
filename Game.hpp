/*
 * Game.hpp
 *
 *  Created on: 27.03.2022
 *      Author: uwe-w
 */

#ifndef GAME_HPP_
#define GAME_HPP_

#include "sdltypes.hpp"
#include "Figure.hpp"
#include "Nation.hpp"
#include "Player.hpp"
#include <string>
#include <random>
class City;
class NationTurn;
struct GameJson;

struct WonderData{
	ImprovementType what;
	Nationality who;
	bool isDestroyed = false;
	bool isObsolete = false;
	City* city = nullptr;
};

class Game{
public: class Year{
public:
	unsigned int m_yearNumberRaw;
	Year(unsigned int yearNumberRaw);
	std::string m_yearString();
	std::vector<std::shared_ptr<NationTurn>> m_turns;
	int m_randomSeed;
};
private:
	std::vector<WonderData> m_hasWonderBeenBuilt;
	std::vector<std::shared_ptr<City>> m_allCities;
	std::random_device m_theRandomDevice;
	std::vector<std::shared_ptr<City>> m_citiesAlive;
	void m_beginNewYear();
	Year m_currentYear = Year(0);
	int m_nationAtCurrentTurnIndex = 0;
	std::vector<std::shared_ptr<Nation>> m_nationsPlaying;
	void m_startNewTurn();
	ClimateState m_climateState = NO_CLIMATE_ALARM;
	int m_pollutionCount = 0;
public:
	void m_initDefault(std::vector<Nationality>& nationsToPlay);
	ClimateState m_ClimateState(){return m_climateState;}
	bool m_depollute(){if(m_pollutionCount==0) throw false; m_pollutionCount--;return true;}
	void m_pollute(){m_pollutionCount++;}
	std::shared_ptr<Nation> m_getNation(Nationality nationality);
	std::vector<Nationality> m_NationalitiesPlaying();
	const WonderData& m_wonderData(ImprovementType imptype);
	bool m_isObsolete(ImprovementType imptype);
	std::vector<WonderData>& m_HasWonderBeenBuilt(){return m_hasWonderBeenBuilt;}
	Year& m_CurrentYear(){return m_currentYear;}
	std::vector<std::shared_ptr<City>>& m_CitiesAlive(){return m_citiesAlive;}
	std::vector<std::shared_ptr<City>>& m_AllCities(){return m_allCities;}
	std::vector<std::shared_ptr<Nation>>& m_NationsPlaying(){return m_nationsPlaying;}
	Game();
	bool m_addNation(Nationality naewNationality);
	bool m_addNation(std::shared_ptr<Nation> newNation);
	void m_makeEndOfTurn();
	Nationality m_calculateWinnerInFight(std::shared_ptr<Figure> attacker, std::shared_ptr<Figure> defender);
	std::shared_ptr<Nation> m_NationAtCurrentTurn();
	unsigned int m_Year(){return m_currentYear.m_yearNumberRaw;}
	std::shared_ptr<Figure> m_getCurrentFigure(Nation* nation = nullptr);
	void m_handleObsoletion(std::vector<ImprovementType>& imptypes, std::vector<ImprovementType>& wonderTypes);
	void m_acknowledgeExploration(Technology tech, Nationality nationality = NO_NATIONALITY);
	int m_getRandomNumberBetween(int lowerBound, int upperBound);
	bool m_hasActiveWonder(Nationality nationality, ImprovementType imptype);
	bool m_isWonderOnContinent(City& city,ImprovementType imptype);
	void m_handleGlobalWarming();
	GameJson m_createJson();
	friend class Player;
	friend class GameLoader;
	friend class GameFactory;
};

extern bool gameReady;

#endif /* GAME_HPP_ */
