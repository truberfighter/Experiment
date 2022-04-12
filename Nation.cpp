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

using namespace std;

Nation::Nation(Nationality nationality, std::string leaderName): m_nation(nationality), m_leaderName(leaderName), enable_shared_from_this()
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
		return "Iulius Caesar";
	case MONGOL:
		return "Dschinghis Khan";
	case GERMAN:
		return "Otto von Bismarck";
	case CHINESE:
		return "Xi Jinping";
	default:
		return "ANONYMOUS";
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
		return Coordinate(14,15);
	default:{
		return Coordinate((int) n , (int) n);
		cout<<"Make better standardcoordinates"<<std::endl;
	}
}
}

std::shared_ptr<Figure> Nation::m_getCurrentFigure(){
	//auto& f = m_activeFigures;
	cout<<"m_getCurrentFigure(), &m_activeFigures = " <<&m_activeFigures<<endl;

	cout<< m_activeFigures.front()<<std::endl;
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
		myString+=")";
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
		myString+=")";
		throw(NullPointerException("Nation",myString));
	}
	m_activeFigures.push_back(std::shared_ptr<Figure>(newFigure));
	cout<<"size: "<<m_activeFigures.size();
	unsigned int previousSize = m_activeFigures.size();
	m_activeFigures.unique();
	cout<<", "<<m_activeFigures.size()<<endl;
	return previousSize == m_activeFigures.size();
}

void Nation::m_startNewTurn(){
	try{
	this->m_activeFigures = std::list<std::shared_ptr<Figure>>();
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
			cout<<", m_activeFigures.size() = "<<m_activeFigures.size()<<", &m_activeFigures = "<<&m_activeFigures<<", nation = "<<this<<endl;
		}
	}
	}
	catch(TurnEndsTooEarly& theException){
		throw(theException);
	}
}
