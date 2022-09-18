/*
 * NormalUnit.cpp
 *
 *  Created on: 14.09.2022
 *      Author: uwe-w
 */

#include "NormalUnit.hpp"
#include "Game.hpp"
#include "sdltypes.hpp"
#include "Field.hpp"
#include "EventHandler.hpp"
#include "City.hpp"

FigureCategory NormalUnit::m_FigureCategory(){
	return GROUND;
}

std::string NormalUnit::m_orderOverview(){return "H: HomeCity, S: Sentry, P: Pillage, F: Fortify";}

NormalUnit::NormalUnit(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)
: Figure(whereToStart, nationality, home, isVeteran)
{
}
NormalUnit::~NormalUnit(){
	std::cout<<"normalunitdestruktor"<<this<<std::endl;
}


bool NormalUnit::m_takeOrder(char order){
	switch(order){
	case 's':{
		m_sentry();
		return true;
	}
	case 'f':{
		m_fortify();
		return true;
	}
	case ' ': {
		m_finishMove();
		return true;
	}
	case 'p':{
		return m_Pillage();
	}
	case 'h': return m_homeCity();
	default: return false;
}
}

THE_END_OF_TURN_FOR NormalUnit GOES_UNEVENTFUL



