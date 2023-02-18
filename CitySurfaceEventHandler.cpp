/*
 * CitySurfaceEventHandler.cpp
 *
 *  Created on: 17.07.2022
 *      Author: uwe-w
 */

#include "Field.hpp"
#include "Drawing.hpp"
#include "City.hpp"
#include "CitySurface.hpp"
#include <sstream>
#include "SelectorSurface.hpp"
class IrrelevantClick{};

bool CitySurface::m_handleEvent(const SDL_Event& event){
	try{
		if(event.type == SDL_KEYDOWN){
	std::cout<<"Man hat den SurfaceKeyCode "<<event.key.keysym.sym<<std::endl;
		if(m_handleKeyboardEvent(event)){
			return true;
		}
		else return false;
	}
	if(event.type == SDL_MOUSEBUTTONDOWN){
		if(event.button.button == SDL_BUTTON_LEFT){
			if (m_handleLeftClick(event.button)==true){
				std::cout<<"surface drawn"<<std::endl;
				return true;
			}
		}
	}
	return false;
	}
	catch(QuitSurface& qs){
	throw qs;
	}
	catch(SDLQuitException& sdlqe){
		throw sdlqe;
	}
}

bool CitySurface::m_handleKeyboardEvent(const SDL_Event& event){
	KeyCode subsurfaceKeycode[] = {SDLK_i,SDLK_h, SDLK_m, SDLK_v};
	SubSurfaceState subSurfaceState[] = {SUBSURFACE_INFO, SUBSURFACE_HAPPY, SUBSURFACE_MAP, SUBSURFACE_VIEW};
	for(int i(0); i<4; i++){
		if(event.key.keysym.sym == subsurfaceKeycode[i]){
			m_subsurface->m_state = subSurfaceState[i];
			m_subsurface->m_draw();
			return true;
		}
	}
	if(event.key.keysym.sym == SDLK_c){
		return m_changeWhatIsBuilt();
	}
	if(event.key.keysym.sym == SDLK_b){
		return m_buy();
	}
	std::cout<<"false keyboardhandling "<<std::endl;
	throw QuitSurface();
}

bool CitySurface::m_handleLeftClick(const SDL_MouseButtonEvent& event){
	std::cout<<"CitySurface::m_handleLeftClick(), x = "<<event.x<<", y = "<<event.y<<std::endl;
	try
	{
	if (m_subsurface->m_handleMouseClick(event.x, event.y)){
		return true;
	}
	if(buyButton.m_isClicked(event.x, event.y)){
		return m_buy();
	}
	else{
		int xToStart = PRODUCTION_OVERVIEW_WIDTH;
		int yToStart = CITIZENS_OVERVIEW_HEIGHT;
		if(event.x <= xToStart + 5* STANDARD_FIELD_SIZE && std::abs(5*STANDARD_FIELD_SIZE/2 + yToStart - event.y) <= 5*STANDARD_FIELD_SIZE/2
		&& event.x > xToStart){
			std::vector<Coordinate> coordinateVector = Field::coordinatesAroundCity();
			Coordinate differenceCoordinate((event.x - xToStart -1)/STANDARD_FIELD_SIZE - 2,(event.y - yToStart - 1)/STANDARD_FIELD_SIZE -2) ;
			std::cout<<"differenceCoordinate: "<<differenceCoordinate<<std::endl;
			if(isInVector<Coordinate>(coordinateVector, differenceCoordinate, [](const Coordinate& x, const Coordinate& y){return x.x==y.x&&x.y==y.y;})){
				std::cout<<"ruft gleich m_placeCitizen auf"<<std::endl;
				Field* fieldClickedOn = m_associatedCity->m_whereItStands->m_getNeighbouringField(differenceCoordinate);
				std::cout<<"*fieldClickedOn: "<<*fieldClickedOn<<std::endl;
				return m_associatedCity->m_placeCitizen(fieldClickedOn);
			}
		}
		if(event.y <= CITIZENS_OVERVIEW_HEIGHT && event.x <= CITIZENS_OVERVIEW_WIDTH){
			return m_handleCitizenClick(event);
		}
	}
	if(changeButton.m_isClicked(event.x, event.y)){
		return m_changeWhatIsBuilt();
	}
	for(int buttonIndex(0); buttonIndex<m_sellingElements.size();buttonIndex++){
		int effectiveIndex = buttonIndex+m_indexForImprovementOverview;
		std::shared_ptr<ButtonElement>& sellingButton = m_sellingElements[buttonIndex];
		if(sellingButton->m_isClicked(event.x, event.y)){
			//check if it is the "More"-Button
			if(sellingButton==m_sellingElements.back()&&m_associatedCity->m_improvements.size()>=IMPROVEMENT_OVERVIEW_HEIGHT_NORMED){
				m_indexForImprovementOverview = std::min(m_indexForImprovementOverview+IMPROVEMENT_OVERVIEW_HEIGHT_NORMED-1,std::max(0,(int)m_associatedCity->m_improvements.size()-1));
				std::cout<<"Z.98 in city surface"<<std::endl;
				m_createSellingButtonElements();std::cout<<"Z.99citysurface"<<std::endl;
				return true;
			}
			else{
				return m_handleSellingButtonClick(effectiveIndex);
			}
		}
	}

	std::cout<<"quitSufaece"<<std::endl;
	throw(QuitSurface());
	}
	catch (QuitSurface& theException){
		throw theException;
	}
	catch(IrrelevantClick& icl){
		return false;
	}
}


bool Subsurface::m_handleMouseClick(int x, int y){
	if(y<SCREEN_HEIGHT-SUBSURFACE_HEIGHT || x<FOOD_STORAGE_WIDTH || x >= SUBSURFACE_WIDTH + FOOD_STORAGE_WIDTH){
		return false;
	}
	if(y<=SUBSURFACE_BUTTON_HEIGHT + SCREEN_HEIGHT - SUBSURFACE_HEIGHT){
		switch((x-FOOD_STORAGE_WIDTH)/(SUBSURFACE_WIDTH/4)){
		case 0: m_state = SUBSURFACE_INFO; break;
		case 1: m_state = SUBSURFACE_HAPPY; break;
		case 2: m_state = SUBSURFACE_MAP; break;
		case 3: m_state = SUBSURFACE_VIEW; break;
		default: m_state = SUBSURFACE_INFO; break;
		}
		return true;
	}
	switch(m_state){
	case SUBSURFACE_VIEW:
	{
		m_state = SUBSURFACE_INFO;
		return true;
	}
	case SUBSURFACE_MAP:
	{
		throw IrrelevantClick();
	}
	case SUBSURFACE_HAPPY: {
		throw IrrelevantClick();
	}
	case SUBSURFACE_INFO:{
		const int figuresPerRow = SUBSURFACE_WIDTH/STANDARD_FIELD_SIZE;
		int indexX = (x-FOOD_STORAGE_WIDTH)/STANDARD_FIELD_SIZE;
		int indexY = (y-SCREEN_HEIGHT + SUBSURFACE_HEIGHT - 1 - SUBSURFACE_BUTTON_HEIGHT)/STANDARD_FIELD_SIZE;
		int index = figuresPerRow*indexY+indexX;
		int i(0);
		if(index < 0){
			std::cout<<"weird index calculation, result: "<<index<<std::endl;
			throw QuitSurface();
		}
		for(const std::shared_ptr<Figure>& currentFigure: m_surface->m_associatedCity->m_WhereItStands()->m_FiguresOnField()){
			if(index!=i++){
				continue;
			}
			switch(currentFigure->m_FigureState()){
			case SENTRIED:{
				currentFigure->m_startMove(true);
				return true;
			}
			case FORTIFIED: {
				currentFigure->m_setFigureState(DONE_WITH_TURN);
				return true;
			}
			case SENTRYING: {
				currentFigure->m_setFigureState(DONE_WITH_TURN);
				return true;
			}
			case COMPLETELY_FORTIFIED:{
				currentFigure->m_startMove(true);
				return true;
			}
			default:{
				throw IrrelevantClick();
			}
			}
		}
		break;
	}
	default:
	{
		std::cout<<"I do not know what that subsurfacestate is!"<<std::endl;
		return false;
	}
}
	throw IrrelevantClick();

}

bool CitySurface::m_handleCitizenClick(const SDL_MouseButtonEvent& event){
#ifdef CHANGETO
#undef CHANGETO
#endif
#define CHANGETO(x,y) case x: {(m_associatedCity->m_citizens)[quotientX]->m_state = y; return true;}
	int xToStart = FIRST_CITIZEN_X;
	int quotientX = (event.x - FIRST_CITIZEN_X)/(CITIZEN_MAX_WIDTH*CITIZEN_SCALEFACTOR);
	if(quotientX < 0){
		return false;
	}
	if(quotientX >= m_associatedCity->m_size()){
		return false;
	}
	switch(m_associatedCity->m_citizens[quotientX]->m_state){
	CHANGETO(ENTERTAINER, TAX_COLLECTOR)
	CHANGETO(TAX_COLLECTOR, SCIENTIST)
	CHANGETO(SCIENTIST, ENTERTAINER)
	default:{
		return false;
	}
	}
}

bool CitySurface::m_handleSellingButtonClick(int effectiveIndex){
ImprovementType imptype = m_associatedCity->m_improvements[effectiveIndex].m_what;
	if(!City::isWonderType(imptype)){
		std::stringstream notSellingSuggestor;
		notSellingSuggestor<<"Do not sell your "<<imptype;
		notSellingSuggestor.flush();
		std::stringstream sellingSuggestor;
		sellingSuggestor<<"Sell your "<<imptype<<" for "<<City::shieldsNeeded(imptype)<<"$";
		sellingSuggestor.flush();
		std::function<void()> dn = [](){};
		std::shared_ptr<SelectionElement> se1 = std::make_shared<SelectionElement>(notSellingSuggestor.str(),0,dn),
		se2 = std::make_shared<SelectionElement>(sellingSuggestor.str(),1,dn);
		std::vector<std::shared_ptr<SelectionElement>> theVector{se1,se2};
		SelectorSurface surface(0,0,theVector);
		SelectionReturn result = surface.m_fetchSelection();
		if(result.index == 1){
			m_associatedCity->m_sell(effectiveIndex);
			m_createSellingButtonElements();
		}
	}
	return true;
}

bool CitySurface::m_buy(){
	ImprovementType imptype = m_associatedCity->m_whatIsBuilt;
	int price = m_associatedCity->m_buyingPrice(imptype);
	std::stringstream infoStream;
	infoStream<<"Cost to complete"<<std::endl<<imptype<<": "<<price<<"$,\n"<<"Treasury: "<<m_associatedCity->m_owningNation->m_Treasury()<<std::endl;
	SDL_Rect rect;
	rect= Miscellaneous::printMultipleLines(infoStream.str(), BUY_INFO_X, BUY_INFO_Y, whiteColor, true, Graphics::Civ::shieldGreyColor());
	SDL_RenderPresent(theRenderer);
	if(m_associatedCity->m_owningNation->m_Treasury()>=price){
		std::string yes = "Yes", no = "No";
		std::function<void()> dn = [](){};
		std::vector<std::shared_ptr<SelectionElement>> selectionElements = {std::make_shared<SelectionElement>(yes,0,dn), std::make_shared<SelectionElement>(no, 1, dn)};
		SelectorSurface ssurface(BUY_INFO_X,BUY_INFO_Y+rect.h,selectionElements);
		if(ssurface.m_fetchSelection().index==0){
			m_associatedCity->m_buy(price);
		}
		return true;
	}
	return false;
}
