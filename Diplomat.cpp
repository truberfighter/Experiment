/*
 * Diplomat.cpp
 *
 *  Created on: 17.09.2022
 *      Author: uwe-w
 */
#include "Diplomat.hpp"
#include "City.hpp"
#include "Field.hpp"


Diplomat::	Diplomat(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)\
:Figure(whereToStart,nationality,home,isVeteran){\
	m_resetMovementPoints();\
	if(!m_initImage()) std::cout<<"Fatal error: MovableThing not created"<<std::endl;\
}\
Diplomat::~Diplomat(){}
CREATE_IMAGE(Diplomat)
DEFAULT_MOVING_POINTS(Diplomat,2)
SHIELD_COST(Diplomat,60)
VISIBILITY(Diplomat,1)
FigureType Diplomat::m_FigureType(){return DIPLOMAT;}
STRENGTH_OF Diplomat WHEN_DEFENDING(0)
STRENGTH_OF Diplomat WHEN_ATTACKING(12)
THE_END_OF_TURN_FOR Diplomat GOES_UNEVENTFUL
bool Diplomat::m_takeOrder(char order){
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
		case 'h': return m_homeCity();
		default: return false;
	}
}

std::string Diplomat::m_orderOverview(){return "H: HomeCity, S: Sentry, F: Fortify";}

