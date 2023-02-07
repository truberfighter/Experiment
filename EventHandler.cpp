/*
 * EventHandler.cpp
 *
 *  Created on: 19.02.2022
 *      Author: uwe-w
 */

#include "EventHandler.hpp"
#include <iostream>
#include <iomanip>
#include "sdltypes.hpp"
#include "Figure.hpp"
#include "Field.hpp"
#include "City.hpp"
#include "GameMain.hpp"
#include <sstream>
#include <list>
#include "SelectorSurface.hpp"
#define NO_EVENT_HANDLING_FOUND return true;

Game* theGame;

void EventHandler::m_setCurrentDrawing(std::shared_ptr<Drawing> drawing){
	m_currentDrawing = drawing;
}
EventHandler::EventHandler()
:  m_currentDrawing(nullptr)
{}

void GameMain::m_showNationInfo(){
	SDL_Color backgroundColor = infoTextBackgroundColor;
	SDL_Rect rectToFill{0,NATION_INFO_Y, FIGURE_INFO_WIDTH, NATION_INFO_HEIGHT};
	SDL_SetRenderDrawColor(theRenderer,backgroundColor);
	SDL_RenderFillRect(theRenderer, &rectToFill);
	std::stringstream theStringstream;
	Nation& nation = *theGame->m_NationAtCurrentTurn();
	theStringstream<<theGame->m_CurrentYear().m_yearString()<<std::endl;
	theStringstream<<std::setw(5)<<nation.m_Treasury()<<"$"<<"; "<<nation.m_TaxRate()<<"."<<nation.m_LuxuriesRate()<<"."<<nation.m_ScienceRate()<<std::endl;
	std::cout<<theStringstream.str()<<std::endl;
	SDL_Rect r = Miscellaneous::printMultipleLines(theStringstream.str(), 0, NATION_INFO_Y, whiteColor);

}

void GameMain::m_showFigureInfo(){
	SDL_Color& backgroundColor = infoTextBackgroundColor;
	SDL_SetRenderDrawColor(theRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	SDL_Rect rectToFill{0,FIGURE_INFO_Y,FIGURE_INFO_WIDTH, SCREEN_HEIGHT-INFO_TEXT_Y};
	SDL_RenderFillRect(theRenderer, &rectToFill);

	std::stringstream theStringstream;
	std::list<int> f;
	if(!m_whatToMove){
		if(theGame->m_NationAtCurrentTurn()->m_queueSize()==0){
			theStringstream<< "End of turn for "<<theGame->m_NationAtCurrentTurn()->m_Nation()<<std::endl;
		}
	}
	else{ //if(m_whatToMove)
		theStringstream<<m_whatToMove->m_figureOverview()<<std::endl;
		theStringstream<<"Moves: "<<m_whatToMove->m_MovementPoints()<<std::endl;
	}
	SDL_Rect r = Miscellaneous::printMultipleLines(theStringstream.str(), 0, FIGURE_INFO_Y, whiteColor);
	if(theGame->m_ClimateState()!=NO_CLIMATE_ALARM){
		std::stringstream s;
		s<<"Climate alarm:\nStage "<<theGame->m_ClimateState()<<" reached!"<<std::endl;
		Miscellaneous::printMultipleLines(s.str(), r.x,r.y+r.h, Graphics::Civ::alarmColor(theGame->m_ClimateState()), true, blackColor);
	}
	//std::cout<<"stringOfInterest: "<<stringOfInterest<<std::endl;

}

void GameMain::m_setWhatToMove(std::shared_ptr<Figure> whatToMove){
	if(!whatToMove)
		std::cout<<"Nun kann man sich nicht mehr bewegen!"<<std::endl;
	m_whatToMove = whatToMove;
	if(whatToMove){
	DrawingElement* whatToPutOver = whatToMove->m_Image()->m_HowDrawn().front();
	std::shared_ptr<DrawingElement> pointer = whatToPutOver->shared_from_this();
	m_currentDrawing->m_putOver(pointer, SIDETEXT_LAYER);
	}
}


bool GameMain::m_handleEvent(const SDL_Event& event){
	try{
	if(event.type == SDL_KEYDOWN){
		if(!m_handleKeyboardEvent(event)){
			std::cout<<"No EventHandling found!"<<std::endl;
			NO_EVENT_HANDLING_FOUND
		}
		this->m_setWhatToMove(theGame->m_getCurrentFigure());
		std::cout<<"looolll"<<std::endl;
		m_drawMainDrawing();
		std::cout<<"\n currentNatiodn: "<<theGame->m_NationAtCurrentTurn()->m_Nation()<<std::endl;
		for(std::shared_ptr<Nation> nationToPrint: theGame->m_NationsPlaying()){
			if(nationToPrint == theGame->m_NationAtCurrentTurn())
			std::cout<<nationToPrint->m_Nation()<<": "<<nationToPrint->m_activeFiguresSize()<<" active figures; "<<nationToPrint->m_Figures().size()<<" figures in general"<<std::endl;
		}
		SDL_RenderPresent(theRenderer);
	}
	if(event.type==SDL_MOUSEBUTTONDOWN ){
		if(event.button.button == SDL_BUTTON_LEFT){
			if(m_handleLeftClick(event.button)){
				m_setWhatToMove(theGame->m_getCurrentFigure());
				m_drawMainDrawing();
				SDL_RenderPresent(theRenderer);
			}
		}
	}
	return false;
	}
	catch(SDLQuitException& exception){
		throw exception;
	}
}

bool GameMain::m_handleKeyboardEvent(const SDL_Event& event){
	m_setWhatToMove(theGame->m_getCurrentFigure());
	KeyCode keyCode = event.key.keysym.sym;
	std::cout<<"Man hat den KeyCode "<<keyCode<<std::endl;
	Direction directions[13] = {DOWN_LEFT, DOWN, DOWN_RIGHT, LEFT, STANDING_STILL,
			RIGHT, UP_LEFT, UP, UP_RIGHT, UP, LEFT, RIGHT, DOWN};
	KeyCode keyPossibilities[13] = {SDLK_1_DOWN_LEFT, SDLK_2_DOWN, SDLK_3_DOWN_RIGHT,
			SDLK_4_LEFT, SDLK_5_CENTER, SDLK_6_RIGHT,
			SDLK_7_UP_LEFT, SDLK_8_UP, SDLK_9_UP_RIGHT, SDLK_UP, SDLK_LEFT, SDLK_RIGHT, SDLK_DOWN};
	if(m_whatToMove){
		for(int i = 0; i<13; i++){
			if(keyCode == keyPossibilities[i]){
				try{
				if(m_whatToMove->m_tryMoveToField(directions[i])){
					return true;
				}
				else return false;
				}
				catch(Fight& fight){
					return true;
				}
			}
		}
	}
	if(keyCode==SDLK_o){
		return m_menu();
	}
	char orders[]={'s', 'r', ' ', 'h','b','i','m','c'};
	KeyCode keyCharPossibilities[]={SDLK_s, SDLK_r, SDLK_SPACE,SDLK_h, SDLK_b,SDLK_i,SDLK_m,SDLK_c};
	if(m_whatToMove){
	for(int i = 0; i<8; i++){
		if(keyCode == keyCharPossibilities[i]){
			try{
			if(m_whatToMove->m_takeOrder(orders[i])){
				if(keyCode == SDLK_b){
					m_whatToMove->m_WhereItStands().m_CityContained()->m_createCitySurface().m_displaySurface(m_currentRenderer);
				}
				m_whatToMove->m_setInstructionsForDrawingElement();
				m_currentDrawing->m_draw();
			}

			else
				std::cout<<"Move failed for KeyCode for "<<orders[i]<<std::endl;
			return true;
		}
		catch(CityFounded& f){
			return true;
		}
		}
	}
}
	switch(event.key.keysym.sym){
	case SDLK_ENTER_KEY:{
		theGame->m_makeEndOfTurn();
		break;
	}
	case SDLK_w:{
		m_whatToMove->m_wait();
		break;
	}
	case SDLK_TAXRATE_KEY:
	{
		theGame->m_NationAtCurrentTurn()->m_alterTaxRate();
		break;
	}
	case SDLK_LUXURIESRATE_KEY:
	{
		theGame->m_NationAtCurrentTurn()->m_alterLuxuriesRate();
		break;
	}
	default:
		NO_EVENT_HANDLING_FOUND

	}
	return true;
}


int GameMain::m_drawMainDrawing(){
	SDL_SetRenderDrawColor(theRenderer,30,30,30,0);
	SDL_RenderClear(theRenderer);
	int whatToReturn = 0;
	if(m_currentDrawing)
	whatToReturn = m_currentDrawing->m_draw(-m_topLeftCorner.x, -m_topLeftCorner.y);
	m_showFigureInfo();
	m_showNationInfo();
	return whatToReturn;
}

std::ostream& operator<<(std::ostream& os, MenuReturn& Return){
	switch(Return){
	case ALTER_LUXURIES_RATE: os<<"Alter luxuries rate"; break;
	case ALTER_TAX_RATE: os<<"Alter tax rate"; break;
	case SAVE_GAME: os<<"Save game"; break;
	default: os<<"Weird menu return: "<<(int)Return; break;
	}
	return os;
}

bool GameMain::m_menu(){
try{
	MenuReturn returns[] = {ALTER_TAX_RATE,ALTER_LUXURIES_RATE,SAVE_GAME};
	std::vector<std::shared_ptr<SelectionElement>> alternatives;
	for(MenuReturn currentReturn: returns){
		std::stringstream s;
		s<<currentReturn; s.flush();
		std::function<void()> dn = [](){};
		auto t = std::make_shared<SelectionElement>(s.str(),currentReturn,dn);
		alternatives.push_back(t);
	}
	SelectorSurface menu(0,0,alternatives);
	SelectionReturn theReturn = menu.m_fetchSelection();
	switch(theReturn.layer){
	case ALTER_TAX_RATE: return theGame->m_NationAtCurrentTurn()->m_alterTaxRate();
	case ALTER_LUXURIES_RATE: return theGame->m_NationAtCurrentTurn()->m_alterLuxuriesRate();
	case SAVE_GAME: m_offerSavingGame(); return true;
	default: return false;
	}
}
catch(SDLQuitException& sqe){
	throw sqe;
}
catch(QuitSelection& sq){
	if(sq.m_returnSomething==NO_ACTION)
		return false;
	throw sq;
}
}
