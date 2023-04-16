/*
 * Plane.cpp
 *
 *  Created on: 18.09.2022
 *      Author: uwe-w
 */

#ifndef PLANE_CPP_
#define PLANE_CPP_
#include "Field.hpp"
#include "City.hpp"
#include "Plane.hpp"

FigureCategory Plane::m_FigureCategory(){return FLIGHT;}
Plane::	Plane(Field* whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)
:Figure(whereToStart,nationality,home,isVeteran)
{

}
Plane::~Plane(){}
void Plane::m_finishMove(){
	m_makeFiguresVisibleAround();
	m_figureState = DONE_WITH_TURN;
	m_nationality->m_removeFromQueue(shared_from_this());
	if(m_whereItStands->m_CityContained()!=nullptr || m_whereItStands->m_getCargoCapability(*this)>0){
		m_turnsFlying = 0;
		return;
	}
	if(++m_turnsFlying>=m_maximumTurnsFlying()){
		std::cout<<"Plane of "<<m_FigureType()<<" has crashed!"<<std::endl;
		m_nationality->m_destroyFigure(shared_from_this());
		std::stringstream s;
		s<<"Plane of "<<m_FigureType()<<" has crashed!"<<std::endl;
		Miscellaneous::displayText(s.str(), 400, 400, whiteColor, true, Graphics::redColor());
		SDL_RenderPresent(theRenderer);
		SDL_Delay(250);
	}
}

std::string Plane::m_orderOverview(){return "H: HomeCity";}

bool Plane::m_takeOrder(char order){
	switch(order){
	case ' ': {
		m_finishMove();
		return true;
	}
	case 'h': return m_homeCity();
	default: return false;
	}
}



DECONSTRUCTOR_PLANE(Bomber)
FIGURE_TYPE(Bomber,BOMBER)
SHIELD_COST(Bomber,120)
CREATE_IMAGE(Bomber)
DEFAULT_MOVING_POINTS(Bomber,8)
STRENGTH_OF Bomber WHEN_ATTACKING(12)
STRENGTH_OF Bomber WHEN_DEFENDING(1)
VISIBILITY(Bomber,2)
MAXIMUM_TURNS_FLYING(Bomber,2)

DECONSTRUCTOR_PLANE(Fighter)
FIGURE_TYPE(Fighter,FIGHTER)
SHIELD_COST(Fighter,60)
CREATE_IMAGE(Fighter)
DEFAULT_MOVING_POINTS(Fighter,10)
STRENGTH_OF Fighter WHEN_ATTACKING(4)
STRENGTH_OF Fighter WHEN_DEFENDING(2)
VISIBILITY(Fighter,2)
MAXIMUM_TURNS_FLYING(Fighter,1)

DECONSTRUCTOR_PLANE(Nuclear)
FIGURE_TYPE(Nuclear,NUCLEAR)
SHIELD_COST(Nuclear,160)
CREATE_IMAGE(Nuclear)
DEFAULT_MOVING_POINTS(Nuclear,16)
STRENGTH_OF Nuclear WHEN_ATTACKING(99)
STRENGTH_OF Nuclear WHEN_DEFENDING(0)
VISIBILITY(Nuclear,1)
MAXIMUM_TURNS_FLYING(Nuclear,2)

bool Nuclear::m_canBeSeenBy(Figure* figureLooking){
	return false;
}

void Plane::m_finishWinningAttack(Field* battlefield){
	Figure::m_finishWinningAttack(battlefield);
}

void Nuclear::m_finishWinningAttack(Field* battlefield){
	std::vector<Field*> fieldsDamaged = battlefield->m_neighbouringFields(1);
	for(Field* currentField: fieldsDamaged){
		currentField->m_receiveNuclearStrike(this);
	}
	Plane::m_finishWinningAttack(battlefield);
}

void Bomber::m_finishWinningAttack(Field* battlefield){
	Plane::m_finishWinningAttack(battlefield);
	m_movementPoints = 0;;
	m_finishMove();
}
#endif /* PLANE_CPP_ */
