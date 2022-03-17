/*
 * Figure.cpp
 *
 *  Created on: 16.03.2022
 *      Author: uwe-w
 */

#include "figure.hpp"
#include <memory>
#include "FieldContainer.hpp"

using namespace std;

Figure::Figure(shared_ptr<MovableThing>& image,  Field& whereToStart, std::shared_ptr<Nation> nationality,  std::shared_ptr<City> home, bool isVeteran)
: enable_shared_from_this<Figure>(), m_nationality(nationality), m_whereItStands(whereToStart), m_image(image), m_home(home), m_isVeteran(isVeteran)
{
	m_resetMovementPoints();
}

Figure::~Figure(){}

void Figure::m_resetMovementPoints(){
	m_movementPoints = m_defaultMovementPoints();
}

bool Figure::m_Pillage(){
	if(m_movementPoints!=0){
		m_figureState = PILLAGE_IN_PROGRESS;
		if(m_whereItStands.m_Pillage()){
			m_finishMove();
			return true;
		}
	}
	return false;
}

FigureState Figure::m_FigureState(){
	return m_figureState;
}

bool Figure::m_fortify(){
	if(m_movementPoints!=0 && m_defensiveStrength()!=0){
		m_figureState = FORTIFIED;
		return true;
	}
	else return false;
}

bool Figure::m_sentry(){
	if(m_movementPoints!=0){
		m_figureState = FORTIFIED;
		return true;
	}
	else return false;
}

bool Figure::m_startMove(){
	if(m_figureState == DONE_WITH_TURN){
		m_resetMovementPoints();
		return true;
	}
	return false;
}
