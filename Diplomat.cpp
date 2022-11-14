/*
 * Diplomat.cpp
 *
 *  Created on: 17.09.2022
 *      Author: uwe-w
 */
#include "Diplomat.hpp"
#include "City.hpp"
#include "Field.hpp"
#include "SelectorSurface.hpp"

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
STRENGTH_OF Diplomat WHEN_ATTACKING(0)
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

bool Diplomat::m_doDiplomatThings(Field& whereToGo){
try{
	if(whereToGo.m_CityContained()&&whereToGo.m_CityContained()->m_OwningNation()->m_Nation()!=m_Nationality()){
		return m_enterCity(whereToGo.m_CityContained());
	}
	if(whereToGo.m_FiguresOnField().size()==1 && whereToGo.m_FiguresOnField().front()->m_Nationality()!=m_Nationality()){
		return m_approachEnemy(whereToGo.m_FiguresOnField().front());
	}
	return true;
}
catch(DiplomatDead& dd){
	throw dd;
}
}

bool Diplomat::m_approachEnemy(std::shared_ptr<Figure> target){
	if(target->m_FigureCategory()==SEA){
		return false;
	}
	int price = target->m_desertationCost();
	std::shared_ptr<Nation> enemyNation = target->m_Nation();
	std::stringstream infoStream;
	infoStream<<target->m_Nationality()<<" "<<target->m_FigureType()<<" will"<<std::endl
<<"desert for "<<price<<"$,\n"<<"Treasury: "<<m_nationality->m_Treasury()<<std::endl;
	SDL_Rect rect;
	rect= Miscellaneous::printMultipleLines(infoStream, DIPLOMAT_DESERT_X, DIPLOMAT_DESERT_Y, whiteColor, true, Graphics::Civ::shieldGreyColor());
	SDL_RenderPresent(theRenderer);
	if(m_nationality->m_Treasury()>=price){
		std::string yes = "Pay", no = "Forget it";
		std::function<void()> dn = [](){};
		std::vector<std::shared_ptr<SelectionElement>> selectionElements = {std::make_shared<SelectionElement>(no,0,dn), std::make_shared<SelectionElement>(yes, 1, dn)};
		SelectorSurface ssurface(DIPLOMAT_DESERT_X,DIPLOMAT_DESERT_Y+rect.h,selectionElements);
		if(ssurface.m_fetchSelection().index==1){
			// calculate new home city
			int minimumDistance = target->m_WhereItStands().m_distanceTo(target->m_Home());
			std::shared_ptr<City> newHome;
			for(std::shared_ptr<City> comparedCity: theGame->m_CitiesAlive()){
				if(target->m_WhereItStands().m_distanceTo(comparedCity)< minimumDistance){
					minimumDistance = target->m_WhereItStands().m_distanceTo(comparedCity);
					if(comparedCity->m_OwningNation()->m_Nation()==m_Nationality()){
						newHome = comparedCity;
					}
					else{
						newHome = nullptr;

					}
						std::cout<<"Change temporary home to "<<newHome<<std::endl;
				}
			}
			target->m_changeNationTo(m_nationality, newHome);
			m_nationality->m_receiveMoney(-price);
		}
		return true;
	}
	else
		return false;
}

bool Diplomat::m_enterCity(std::shared_ptr<City> city){
	return false;
}

