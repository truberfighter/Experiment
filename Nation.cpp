/*
 * Nation.cpp
 *
 *  Created on: 19.03.2022
 *      Author: uwe-w
 */

#include "Nation.hpp"
#include "sdltypes.hpp"
#include <string>
#include "FieldContainer.hpp"
#include "City.hpp"

using namespace std;

Nation::Nation(Nationality nationality, std::string leaderName, bool directlyMakingFiguresActive): m_nation(nationality), m_directlyMakingFiguresActive(directlyMakingFiguresActive), m_leaderName(leaderName), enable_shared_from_this()
{
	cout<<"Nation-Konstruktor mit Nationalität "<< nationality<<", this = "<<this<<endl;
}

bool Nation::m_canBuildRailroad(){
	return true;
}

bool Nation::m_canBuildBridges(){
	return true;
}

const std::string& Nation::m_LeaderName(){
	if(!m_leaderName.compare(""))
		return m_leaderName;
	switch(m_nation){
	case ROMAN:
		return "Iulius Caesar.png";
	case MONGOL:
		return "Dschinghis Khan.png";
	case GERMAN:
		return "Otto von Bismarck.png";
	case CHINESE:
		return "Xi Jinping.png";
	default:
		return "ANONYMOUS.png";
	}
}

void Nation::m_makeFigureWait(){
	std::shared_ptr<Figure> whatToWait = m_getCurrentFigure();
	if(!whatToWait){
		return;
	}
	if(!m_removeFromQueue(whatToWait)){
		throw("Waiting fail");
	}
	m_addToQueue(whatToWait);
}

bool Nation::m_removeFromQueue(std::shared_ptr<Figure> figureToRemove){
	short int sizeToCompare = m_activeFigures.size();
	m_activeFigures.remove(figureToRemove);
	if(m_activeFigures.size()==sizeToCompare){
		return false;
	}
	return true;
}

Coordinate Nation::getStandardCoordinateForNation(Nationality n){
	switch(n){
	case GERMAN:
		return Coordinate(30,30);
	case ROMAN:
		return Coordinate(15,15);
	default:{
		return Coordinate((int) n , (int) n);
		cout<<"Make better standardcoordinates"<<std::endl;
	}
}
}

std::shared_ptr<Figure> Nation::m_getCurrentFigure(){
	//auto& f = m_activeFigures;
	//cout<<"m_getCurrentFigure(), &m_activeFigures = " <<&m_activeFigures<<endl;

	//cout<< m_activeFigures.front()<<std::endl;
	if(m_activeFigures.empty()){
		cout<<"empty list"<<std::endl;
		return nullptr;
	}
	return m_activeFigures.front();
}

void Nation::m_addFigure(std::shared_ptr<Figure> newFigure){
	if(!newFigure){
		std::string myString("m_addFigure");
		myString+=(int)newFigure.get();
		myString+=").png";
		throw(NullPointerException("Nation",myString));

	}
	m_figures.push_back(newFigure);
	if(m_directlyMakingFiguresActive){
		if(!m_addToQueue(newFigure))
			cout<<"m_addToQueueFail"<<m_activeFigures.size()<<endl;	}
}

bool Nation::m_addToQueue(std::shared_ptr<Figure> newFigure){
	if(!newFigure){
		std::string myString("m_addToQueue");
		myString+=(int)newFigure.get();
		myString+=").png";
		throw(NullPointerException("Nation",myString));
	}
	m_activeFigures.push_back(newFigure);
	cout<<"size: "<<m_activeFigures.size();
	unsigned int previousSize = m_activeFigures.size();
	m_activeFigures.unique();
	cout<<", "<<m_activeFigures.size()<<endl;
	newFigure->m_setFigureState(MOVING);
	return previousSize == m_activeFigures.size();
}

void Nation::m_startNewTurn(){
	try{
		for(std::shared_ptr<Figure>& currentFigureToCheck : m_figures){
			if(currentFigureToCheck->m_FigureState()==MOVING){
				throw(TurnEndsTooEarly());
			}
		}
	m_activeFigures = std::list<std::shared_ptr<Figure>>();
	cout<<"Nation::m_startNewTurn(), m_figures.size() = "<<m_figures.size()<<", m_activeFiguresSize = "<<m_activeFigures.size()<<std::endl;
	for(std::shared_ptr<Figure>& currentFigure : m_figures){
		if(!currentFigure->m_startMove()){
			cout<<"Not what I wanted!"<<std::endl;
			throw(MoveStartFail("not yet defined"));
		}
		if(currentFigure->m_FigureState()==MOVING){


		cout<<"m_startNewTurn"<<endl<<"m_activeFiguresSize: "<<m_activeFigures.size();
			if(m_addToQueue(currentFigure))
			cout<< m_activeFigures.front()<<std::endl;
//			cout<<", m_activeFigures.size() = "<<m_activeFigures.size()<<", &m_activeFigures = "<<&m_activeFigures<<", nation = "<<this<<endl;
		}
	}
	}
	catch(TurnEndsTooEarly& theException){
		throw(theException);
	}
}

bool Nation::m_destroyFigure(std::shared_ptr<Figure> figureToRemove){
	std::cout<<"m_destroyFigure: figure.this = "<<figureToRemove<<std::endl;
	figureToRemove->m_WhereItStands().m_releaseFigure(figureToRemove);
	if(figureToRemove->m_Home()){
		if(!figureToRemove->m_Home()->m_releaseFigure(figureToRemove))
			throw 1111;
	}
	int previousListSize = m_figures.size();
	m_activeFigures.remove(figureToRemove);
	for(int i(0); i<previousListSize; i++){
		if(m_figures[i].get()==figureToRemove.get()){
			for(int j=i; j<previousListSize - 1; j++){
				m_figures[j] = m_figures[j+1];
			}
			break;
		}
	}
	m_figures.pop_back();
	std::cout<<"previousListSize (destroyFigure): "<<previousListSize<<", listSize: "<<m_figures.size()<<std::endl;
	return true;
}

std::string Nation::m_colorString(){
	switch(m_nation){
	case ROMAN:
		return "RomanRussian.png";
	case RUSSIAN:
		return "RomanRussian.png";
	case BABYLONIAN:
		return "BabylonianZulu.png";
	case ZULU:
		return "BabylonianZulu.png";
	case EGYPTIAN:
		return "AztecianEgyptian.png";
	case AZTEC:
		return "AztecEgyptian.png";
	case MONGOL:
		return "IndianMongol.png";
	case INDIAN:
		return "IndianMongol.png";
	case AMERICAN:
		return "AmericanChinese.png";
	case CHINESE:
		return "AmericanChinese.png";
	case ENGLISH:
		return "EnglishGreekpng";
	case GREEK:
		return "EnglishGreek.png";
	case FRENCH:
		return "FrenchGerman.png";
	case GERMAN:
		return "FrenchGerman.png";
	}
	return "Some mistake!";
}

SDL_Color Nation::standardNationColor(Nationality nationalityToDraw){
	switch(nationalityToDraw){
	case ROMAN:
		return SDL_Color{170,170,170};
	case RUSSIAN:
		return SDL_Color{220,220,220};
	case BABYLONIAN:
		return SDL_Color{0,255,0,0};
	case ZULU:
		return SDL_Color{0,255,0,0};
	case EGYPTIAN:
		return SDL_Color{120,255,30,0};
	case AZTEC:
		return SDL_Color{120,255,30,0};
	case MONGOL:
		return SDL_Color{50,40,50,0};
	case INDIAN:
		return SDL_Color{50,40,50,0};
	case AMERICAN:
		return SDL_Color{0,30,255,0};
	case CHINESE:
		return SDL_Color{0,30,255,0};
	case ENGLISH:
		return SDL_Color{180,20,80,0};
	case GREEK:
		return SDL_Color{180,20,80,0};
	case FRENCH:
		return SDL_Color{0,0,255,0};
	case GERMAN:
		return SDL_Color{0,0,255,0};
	case BARBARIAN:
		return SDL_Color{255,0,0,0};
	}
	//This color should NEVER appear
	return SDL_Color{100,0,100,0};
}
