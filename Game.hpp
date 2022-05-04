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

class Game{
public: class Year{
public:
	unsigned int m_yearNumberRaw;
	Year(unsigned int yearNumberRaw);
	std::string m_yearString();
	std::vector<std::shared_ptr<NationTurn>> m_turns;
};
private:
	std::random_device m_theRandomDevice;
	std::vector<std::shared_ptr<City>> m_cities;
	void m_beginNewYear();
	Year m_currentYear = Year(0);
	int m_nationAtCurrentTurnIndex = 0;
	std::vector<std::shared_ptr<Nation>> m_nationsPlaying;
	void m_startNewTurn();
public:
	std::vector<std::shared_ptr<Nation>>& m_NationsPlaying(){return m_nationsPlaying;}
	Game(std::vector<Nationality>& nationsToPlay);
	Game(std::vector<std::shared_ptr<Nation>>& nationsToPlay);
	bool m_addNation(Nationality naewNationality);
	bool m_addNation(std::shared_ptr<Nation> newNation);
	void m_makeEndOfTurn();
	//if necessary, create a log
	Nationality m_calculateWinnerInFight(std::shared_ptr<Figure> attacker, std::shared_ptr<Figure> defender);
	std::shared_ptr<Nation> m_NationAtCurrentTurn();
	unsigned int m_Year(){return m_currentYear.m_yearNumberRaw;}
	std::shared_ptr<Figure> m_getCurrentFigure(Nation* nation = nullptr);
	friend class Player;
	int m_getRandomNumberBetween(int lowerBound, int upperBound);
};


#endif /* GAME_HPP_ */
