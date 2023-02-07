/*
 * Caravan.cpp
 *
 *  Created on: 07.01.2023
 *      Author: uwe-w
 */

#include "Caravan.hpp"
#include "City.hpp"
#include "Field.hpp"
#include "SelectorSurface.hpp"
#include <algorithm>

Caravan::Caravan(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)\
:Figure(whereToStart,nationality,home,isVeteran){\
	m_resetMovementPoints();\
	if(!m_initImage()) std::cout<<"Fatal error: MovableThing not created"<<std::endl;\
}\
Caravan::~Caravan(){}
CREATE_IMAGE(Caravan)
DEFAULT_MOVING_POINTS(Caravan,1)
SHIELD_COST(Caravan,60)
VISIBILITY(Caravan,1)
FigureType Caravan::m_FigureType(){return CARAVAN;}
STRENGTH_OF Caravan WHEN_DEFENDING(0)
STRENGTH_OF Caravan WHEN_ATTACKING(1)
THE_END_OF_TURN_FOR Caravan GOES_UNEVENTFUL
bool Caravan::m_takeOrder(char order){
	this;
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
std::string Caravan::m_orderOverview(){return "H: HomeCity, S: Sentry, F: Fortify";}

bool Caravan::m_doCaravanThings(Field& fieldWhereToGo){
	try{
	if(m_home==nullptr)
		return true;
	if(!fieldWhereToGo.m_CityContained()){
		return true;
	}
	if(m_enterCity(*fieldWhereToGo.m_CityContained())){
		throw CaravanDead();
	}
	return true;
	}
	catch(SDLQuitException& sqe){
		throw sqe;
	}
}

int TradeRoute::m_calculateInstantRevenue(){
	double tmp = m_city1->m_WhereItStands()->m_distanceTo(m_city2) + 10;
	tmp *=(double) (m_city1->m_tradeProduction()+m_city2->m_tradeProduction());
	if(m_city1->m_OwningNation()->m_hasExplored(TECHNOLOGY_RAILROAD))
		tmp/=(3/2);
	if(m_city1->m_OwningNation()->m_hasExplored(TECHNOLOGY_FLIGHT))
		tmp/=3/2;
	if(m_city1->m_WhereItStands()->m_ContinentID()==m_city2->m_WhereItStands()->m_ContinentID()){
		tmp/=2;
	}
	if(m_city1->m_OwningNation()==m_city2->m_OwningNation())
		tmp/=2;
	return (int) tmp/24;
}

bool Caravan::m_enterCity(City& city){
	try{
	if(city.m_OwningNation()->m_Nation()==m_Nationality()){
		if(city.m_WhereItStands()->m_distanceTo(m_home)<10){
			return false;
		}
		if(Miscellaneous::yesOrNo(FIGURE_INFO_WIDTH, SCREEN_HEIGHT/3, "Keep moving", "Establish trade route")){
			return false;
		}
	}
	std::shared_ptr<TradeRoute> tradeRoute = std::make_shared<TradeRoute>(city.shared_from_this(),m_home);
	int revenue = tradeRoute->m_calculateInstantRevenue();
	std::stringstream s;
	s<<m_Nationality()<<" caravan from "<<m_home->m_Name()<<"\narrives in "<<city.m_Name()<<"!\nYou receive "<<revenue<<"$.";s.flush();
	Miscellaneous::displayText(s.str(), FIGURE_INFO_WIDTH, SCREEN_HEIGHT/3, Graphics::Civ::darkgreyColor(), true, Graphics::Civ::resourcesWhiteColor());
	if(m_home->m_TradeRoutes().empty()||std::find_if(m_home->m_TradeRoutes().begin(),m_home->m_TradeRoutes().end(),[&city](std::shared_ptr<TradeRoute>& route){
	return route->m_city1==city.shared_from_this()||route->m_city2==city.shared_from_this();})==city.m_TradeRoutes().end())
		m_home->m_TradeRoutes().push_back(tradeRoute);
	return true;
	}
	catch(SDLQuitException& sqe){
		throw sqe;
	}
}

int TradeRoute::m_calculateProduction(){
	double tmp = (double) (m_city1->m_tradeProduction()+m_city2->m_tradeProduction() + 4);
	tmp/=8;
	if(m_city1->m_OwningNation()->m_Nation()==m_city2->m_OwningNation()->m_Nation())
		tmp/=2;
	return (int) tmp;
}
