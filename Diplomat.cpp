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
SHIELD_COST(Diplomat,30)
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
		case ' ': {
			m_finishMove();
			return true;
		}
		case 'h': return m_homeCity();
		default: return false;
	}
}

std::string Diplomat::m_orderOverview(){return "H: HomeCity, S: Sentry";}

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
catch(QuitSelection& qs){
	if(qs.m_returnSomething==NO_ACTION)
		return false;
	throw qs;
}
catch(SDLQuitException& sqe){
	throw sqe;
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
	rect= Miscellaneous::printMultipleLines(infoStream.str(), DIPLOMAT_DESERT_X, DIPLOMAT_DESERT_Y, whiteColor, true, Graphics::Civ::shieldGreyColor());
	SDL_RenderPresent(theRenderer);
	if(m_nationality->m_Treasury()>=price){
		std::string yes = "Pay", no = "Forget it";
		std::function<void()> dn = [](){};
		std::vector<std::shared_ptr<SelectionElement>> selectionElements = {std::make_shared<SelectionElement>(no,0,dn), std::make_shared<SelectionElement>(yes, 1, dn)};
		SelectorSurface ssurface(DIPLOMAT_DESERT_X,DIPLOMAT_DESERT_Y+rect.h,selectionElements);
		if(ssurface.m_fetchSelection().index==0){
			return false;
		}
		// calculate new home city
		double minimumDistance = target->m_WhereItStands().m_distanceTo(target->m_Home());
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
		return true;
	}
	else
		return false;
}

bool Diplomat::m_enterCity(std::shared_ptr<City> city){
	try{
	std::vector<DiplomatChoice> whatToChooseFrom{ESTABLISH_EMBASSY,INVESTIGATE_CITY,INDUSTRIAL_SABOTAGE,
	STEAL_TECHNOLOGY,MEET_WITH_KING};
	if(!city->m_contains(PALACE) || true){//modified for testing
		whatToChooseFrom.push_back(INCITE_REVOLT);
	}
	std::function <void()> f= [](){};
	std::vector<std::shared_ptr<SelectionElement>> selectionElements;
	for(DiplomatChoice dc: whatToChooseFrom){
		std::stringstream s;
		s<<dc;s.flush();
		auto se = std::make_shared<SelectionElement>(s.str(),dc,f);
		selectionElements.push_back(se);
	}
	SelectorSurface selector(FIGURE_INFO_WIDTH,SCREEN_HEIGHT/3,selectionElements);
	SelectionReturn result = selector.m_fetchSelection();
	std::cout<<"DiplomatChoice: "<<whatToChooseFrom[result.index]<<std::endl;
	switch(whatToChooseFrom[result.index]){
	case INCITE_REVOLT:
	{
		if (!city->m_offerRevolt(*this))
			return false;
		break;
	}
	case INDUSTRIAL_SABOTAGE:
	{ //the most epensive non-wonder, non-palace building will be demolished. Otherwise, the production will be cleared.
		int minCost = 0;
		short int minIndex = -1;
		short int index = 0;
		for(const CityImprovement& currentImp: city->m_Improvements()){
			if(!City::isBuildingType(currentImp.m_what) || currentImp.m_what==PALACE){
				continue;
			}
			minCost = std::max(City::shieldsNeeded(currentImp.m_what),minCost);
			if(City::shieldsNeeded(currentImp.m_what)==minCost)
				minIndex = index;
			index++;
		}
		if(minCost!=0){
			city->m_sell(minIndex);
			city->m_OwningNation()->m_receiveMoney(-City::shieldsNeeded(city->m_Improvements()[minIndex].m_what));
			break;
		}
		else{
			city->m_clearProduction();
		}
		break;
	}
	case INVESTIGATE_CITY:
	{
		CitySurface theSurface = city->m_createCitySurface();
		//theSurface.m_createSellingButtonElements();
		theSurface.m_drawSurface(theRenderer);
		SDL_RenderPresent(theRenderer);
		Miscellaneous::holdImage();
		break;
	}
	case STEAL_TECHNOLOGY:
	{
		std::vector<Technology> availableTechnologies;
		for(const Technology currentTech: city->m_OwningNation()->m_ExploredTechnologies()){
			if(m_nationality->m_hasExplored(currentTech)){
				availableTechnologies.push_back(currentTech);
			}
		}
		if(availableTechnologies.size()==0){
			std::string s("No technologies found");
			Miscellaneous::displayText(s, FIGURE_INFO_WIDTH, 0, Graphics::Civ::darkgreyColor(),true,Graphics::Civ::resourcesWhiteColor());
			return false;
		}
		else{
			std::string s("Technologies found: ");
			SDL_Rect theRect = Miscellaneous::printMultipleLines(s, 0, 0., whiteColor, true, Graphics::Civ::irrigationBlueColor());
			std::vector<std::shared_ptr<SelectionElement>> selectionBase;
			for(Technology techToAskFor: availableTechnologies){
				std::cout<<"Technology available: "<<techToAskFor<<std::endl;
				std::function<void()> rightClick = [techToAskFor](){technologyRightClick(techToAskFor);};
				std::stringstream stream2;
				stream2<<techToAskFor;stream2.flush();
				std::shared_ptr<SelectionElement> element = std::make_shared<SelectionElement>(stream2.str(),(int)techToAskFor,rightClick);
				selectionBase.push_back(element);
			}
			std::cout<<"selectionbasesize: "<<selectionBase.size()<<std::endl;
			SelectorSurface surface(theRect.x,theRect.h+theRect.y, selectionBase, false, Graphics::Civ::shieldGreyColor(), blackColor, Graphics::Civ::brightCityBackgroundColor());
			SelectionReturn result = surface.m_fetchSelection();
			m_nationality->m_addTechnology(availableTechnologies[result.index]);
		}
		break;
	}
	default: std::cout<<"wtf, what just happened in Diplomat::enterCity?"<<std::endl;return false;
	}
	}
	catch(QuitSelection& qs){
		if(qs.m_returnSomething==NO_ACTION){
			return false;
		}
		throw qs;
	}
	catch(SDLQuitException& sqe){
		throw sqe;
	}
	throw DiplomatDead();
}

