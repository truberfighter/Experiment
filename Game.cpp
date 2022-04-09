/*
 * Game.cpp
 *
 *  Created on: 27.03.2022
 *      Author: uwe-w
 */

#include "Game.hpp"
#include "FieldContainer.hpp"
#include "Settlers.hpp"

std::shared_ptr<Figure> Game::m_getCurrentFigure(Nation* nation){
	std::cout<<"nation = "<<(nation ? nation : m_NationAtCurrentTurn().get())<<", settlersCount = "<<settlersCount<<std::endl;
	std::cout<<"nation->m_getCurrentFigure() = "<<(nation ? nation : m_NationAtCurrentTurn().get())->m_getCurrentFigure()<<std::endl;
	auto whatToReturn = nation ? nation->m_getCurrentFigure()
		: m_NationAtCurrentTurn()->m_getCurrentFigure();
	std::cout<<"whatToReturn initialized!"<<std::endl;
	return whatToReturn;
}

Game::Game(std::vector<Nationality>& nationsToPlay){
	for(Nationality currentNationality: nationsToPlay){
		std::shared_ptr<Nation> nationPointer =  std::make_shared<Nation>(currentNationality);
		m_nationsPlaying.push_back(nationPointer);
		Nation& nation = *nationPointer;
		Coordinate fieldCoordinate = Nation::getStandardCoordinateForNation(currentNationality);
		std::vector<Meridian>& fieldsOfTheWorld = *theContainer->m_getFieldsOfTheWorld();
		std::shared_ptr<Field> fieldPointer = fieldsOfTheWorld[fieldCoordinate.x][fieldCoordinate.y];
		std::shared_ptr<Settlers> theSettlersPointer = std::make_shared<Settlers>(
				fieldPointer, nationPointer);
		nation.m_addFigure(theSettlersPointer);
	}
}

Game::Year::Year(unsigned int yearNumberRaw):m_yearNumberRaw(yearNumberRaw)
{

}

//New turn
void Game::m_startNewTurn(){
	int temp = m_nationAtCurrentTurnIndex;;
	try{std::cout<<"Nationcount: "<<m_nationsPlaying.size()<<std::endl;
	(++m_nationAtCurrentTurnIndex)%= (m_nationsPlaying.size());
	m_NationAtCurrentTurn()->m_startNewTurn();
	if(m_nationAtCurrentTurnIndex==0){
		std::cout<<"wieder bei null im Nationenindex angekommen"<<std::endl;
	m_currentYear = Year(m_currentYear.m_yearNumberRaw);
	}
	std::cout<<"New turn started!"<<std::endl;
	if(m_NationAtCurrentTurn()->m_activeFiguresSize()<=0){
		std::cout<<"No figure is active!"<<std::endl;
		m_startNewTurn();
	}
	}
	catch(TurnEndsTooEarly& theException){
		std::cout<<theException.what()<<std::endl;
		m_nationAtCurrentTurnIndex = temp;
	}
}

std::shared_ptr<Nation> Game::m_NationAtCurrentTurn(){
	return m_nationsPlaying[m_nationAtCurrentTurnIndex];
}

void Game::m_makeEndOfTurn(){
	m_startNewTurn();
}
