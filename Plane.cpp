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
Plane::	Plane(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)
:Figure(whereToStart,nationality,home,isVeteran)
{

}
Plane::~Plane(){}
void Plane::m_finishMove(){
	m_figureState = DONE_WITH_TURN;
	m_nationality->m_removeFromQueue(shared_from_this());
	if(m_whereItStands->m_CityContained()!=nullptr || m_whereItStands->m_getCargoCapability(*this)>0){
		m_turnsFlying = 0;
		return;
	}
	if(++m_turnsFlying>=m_maximumTurnsFlying()){
		m_nationality->m_destroyFigure(shared_from_this());
		std::stringstream s;
		s<<"Plane of "<<m_FigureType()<<" has crashed!"<<std::endl;
		Miscellaneous::printMultipleLines(s, 400, 400, whiteColor, true, Graphics::redColor());
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


#endif /* PLANE_CPP_ */
