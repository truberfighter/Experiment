/*
 * Game.cpp
 *
 *  Created on: 27.03.2022
 *      Author: uwe-w
 */

#include "Game.hpp"
#include "FieldContainer.hpp"
#include "Settlers.hpp"
#include "Trireme.hpp"
#include <stdlib.h>
#include <random>
#include <iomanip>
#include <functional>
#include "City.hpp"

Nationality Graphics::Civ::currentNationality = ROMAN;

std::shared_ptr<Figure> Game::m_getCurrentFigure(Nation* nation){
	std::cout<<"nation = "<<(nation ? nation : m_NationAtCurrentTurn().get())<<", settlersCount = "<<settlersCount<<std::endl;
	auto whatToReturn = nation ? nation->m_getCurrentFigure()
		: m_NationAtCurrentTurn()->m_getCurrentFigure();
	return whatToReturn;
}

Game::Game(std::vector<Nationality>& nationsToPlay){
	bool firstNation = true;
	for(std::vector<std::shared_ptr<Field>>& currentMeridian: *theContainer->m_getFieldsOfTheWorld()){
		for(std::shared_ptr<Field> currentField: currentMeridian){
			currentField->m_initNationFogInfo(nationsToPlay);
		}
	}
	for(Nationality currentNationality: nationsToPlay){
		Graphics::Civ::currentNationality = currentNationality;
		std::shared_ptr<Nation> nationPointer =  std::make_shared<Nation>(currentNationality, "", PLAYER_PRINCE, firstNation);
		int randomIndex = m_getRandomNumberBetween(0, Science::possibleStartingTechs().size()-1);
		nationPointer->m_setWhatToExplore (Science::possibleStartingTechs()[randomIndex]);
		nationPointer->m_maybeFinishExploration();
		firstNation = false;
		m_nationsPlaying.push_back(nationPointer);
		Nation& nation = *nationPointer;
		Coordinate fieldCoordinate = Nation::getStandardCoordinateForNation(currentNationality);
		std::vector<Meridian>& fieldsOfTheWorld = *theContainer->m_getFieldsOfTheWorld();
		std::shared_ptr<Field> fieldPointer = fieldsOfTheWorld[fieldCoordinate.x][fieldCoordinate.y];
		for(int i(0); i<9; i++){
			std::shared_ptr<Settlers> theSettlersPointer =
			std::make_shared<Settlers>(fieldPointer, nationPointer);
			nation.m_addFigure(theSettlersPointer);
			fieldPointer->m_takeFigure(theSettlersPointer);
			std::shared_ptr<Trireme> theTriremePointer =
			std::make_shared<Trireme>(fieldPointer->m_getNeighbouringField(LEFT), nationPointer);
			nation.m_addFigure(theTriremePointer);
			fieldPointer->m_getNeighbouringField(LEFT)->m_takeFigure(theTriremePointer);
			std::stringstream stream; stream<<"City of "<<nation.m_Nation();stream.flush();
			std::cout<<"stream: "<<stream.str();
		}
		nationPointer->m_setMakingActive(false);
	}
	Graphics::Civ::currentNationality = nationsToPlay[0];
}

Game::Year::Year(unsigned int yearNumberRaw):m_yearNumberRaw(yearNumberRaw)
{

}

//New turn
void Game::m_startNewTurn(){
	int temp = m_nationAtCurrentTurnIndex;
	try{
	for(std::shared_ptr<Figure> currentFigureToCheck: m_NationAtCurrentTurn()->m_Figures()){
		if(currentFigureToCheck->m_FigureState()==MOVING){
			std::cout<<"turnendstooearly by figurestate, nation at fault: "<<currentFigureToCheck->m_Nationality()<<std::endl;
			throw(TurnEndsTooEarly());
		}
	}
	if(m_NationAtCurrentTurn()->m_activeFiguresSize()!=0){
		std::cout<<"turnendstooearly by activefigures"<<std::endl;
		throw(TurnEndsTooEarly());
	}
	(++m_nationAtCurrentTurnIndex)%= (m_nationsPlaying.size());
	m_NationAtCurrentTurn()->m_startNewTurn();
	if(m_nationAtCurrentTurnIndex==0){
		std::cout<<"wieder bei null im Nationenindex angekommen"<<std::endl;
	m_currentYear = Year(m_currentYear.m_yearNumberRaw+1);
	}
	std::cout<<"New turn started for "<<m_NationAtCurrentTurn()->m_Nation()<<"!"<<std::endl;
	if(m_NationAtCurrentTurn()->m_activeFiguresSize()<=0){
		std::cout<<"No figure is active!"<<std::endl;
		m_startNewTurn();
	}
	}
	catch(TurnEndsTooEarly& theException){
		std::cout<<theException.what()<<std::endl;
		m_nationAtCurrentTurnIndex = temp;
		return;
	}
}

std::shared_ptr<Nation> Game::m_NationAtCurrentTurn(){
	return m_nationsPlaying[m_nationAtCurrentTurnIndex];
}

void Game::m_makeEndOfTurn(){
	m_startNewTurn();
}

Nationality Game::m_calculateWinnerInFight(std::shared_ptr<Figure> attacker, std::shared_ptr<Figure> defender){
	try{
	float attackingStrength = attacker->m_attackingStrength()*std::min((float)1, (float) attacker->m_MovementPoints().m_movementPoints/ONE_MOVEMENT_POINT);
	float defensiveStrength = defender->m_defensiveStrength();
	defensiveStrength *= defender->m_WhereItStands().m_defenseBonus();
	if(attacker->m_IsVeteran()){
		attackingStrength *= 1.5;
	}
	if(defender->m_IsVeteran()){
		defensiveStrength*=1.5;
	}
	if(defender->m_FigureState()==FORTIFIED || defender->m_FigureState() == COMPLETELY_FORTIFIED){
		defensiveStrength*=1.5;
	}
	attackingStrength *= 2^8;
	defensiveStrength *= 2^8; //für mehr Präzision, indem Überträge/Rundiúngen entschärft werden
	int randomNumber = m_getRandomNumberBetween(1, ((int) attackingStrength) + ((int) defensiveStrength));

	return randomNumber <= attackingStrength ? attacker->m_Nationality() : defender->m_Nationality();
	}
	catch(RandomFail& randomFail){
	return defender->m_Nationality();
	}
}

int Game::m_getRandomNumberBetween(int lowerBound, int upperBound){
	if(lowerBound > upperBound){
		throw(RandomFail(lowerBound, upperBound));
	}
	 std::uniform_int_distribution<int> distribution(lowerBound, upperBound);
	 int randomNumber = distribution(m_theRandomDevice);
	 return randomNumber;
}

std::string Game::Year::m_yearString(){
	std::stringstream s;
	if(m_yearNumberRaw == 200){
		s<<"1 AD";
		goto Return;
	}
	if(m_yearNumberRaw < 200){
		s<<std::setw(4)<<4000-20*m_yearNumberRaw<<" BC";
		goto Return;
	}
	if(m_yearNumberRaw<=250){
		s<<std::setw(4)<<-4000+20*m_yearNumberRaw<<" AD";
		goto Return;
	}
	if(m_yearNumberRaw<=300){
		s<<std::setw(4)<<1000+(m_yearNumberRaw-250)*10<<" AD";
		goto Return;
	}
	if(m_yearNumberRaw<=350){
		s<<1500+(m_yearNumberRaw-300)*5<<" AD";
		goto Return;
	}
	if(m_yearNumberRaw<=400){
		s<<1750+(m_yearNumberRaw-350)*2<<" AD";
		goto Return;
	}
	s<<1850+(m_yearNumberRaw-400)<<" AD";
	Return: s.flush();
	return s.str();
}
