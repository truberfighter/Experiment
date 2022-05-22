/*
 * EventHandler.cpp
 *
 *  Created on: 19.02.2022
 *      Author: uwe-w
 */

#include "EventHandler.hpp"
#include <iostream>
#include "sdltypes.hpp"
#include "Figure.hpp"
#include <sstream>
#include <list>
#define NO_EVENT_HANDLING_FOUND return true;

Game* theGame;

void EventHandler::m_showFigureInfo(){
	SDL_Color& backgroundColor = infoTextBackgroundColor;
	SDL_SetRenderDrawColor(theRenderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
	SDL_Rect rectToFill{0,FIGURE_INFO_Y,FIGURE_INFO_WIDTH, SCREEN_HEIGHT-INFO_TEXT_Y};
	//std::cout<<"yCoordinate: rectToFill.y = " <<rectToFill.y<<", y = "<<y<<std::endl;
	SDL_RenderFillRect(theRenderer, &rectToFill);

	std::stringstream theStringstream;
	std::string stringOfInterest = "";
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
		stringOfInterest = theStringstream.str();
		//std::cout<<theStringstream.str()<<SDL_GetError()<<std::endl;
	std::string temporaryString = "";
	int length = stringOfInterest.length();
	int whereToPlaceYCoordinate = FIGURE_INFO_Y;
	for(int i(0); i<length; i++){
		if(stringOfInterest[i] != '\n'){
			temporaryString += stringOfInterest[i];
			continue;
		}
		else{
			SDL_Surface* surface = TTF_RenderText_Blended(theFont, temporaryString.c_str(), whiteColor);
				SDL_Texture* texture = SDL_CreateTextureFromSurface(theRenderer, surface);
				SDL_Rect theRect{0, whereToPlaceYCoordinate, surface->w, surface->h};
				SDL_RenderCopy(theRenderer, texture, NULL, &theRect);
			//	std::cout<<temporaryString<<std::endl;
				whereToPlaceYCoordinate+=surface->h;
				temporaryString = "";
				SDL_DestroyTexture(texture);
				SDL_FreeSurface(surface);
		}
	}
	std::cout<<"lol"<<stringOfInterest.length()<<std::endl;
	//std::cout<<"stringOfInterest: "<<stringOfInterest<<std::endl;

}

void EventHandler::m_setWhatToMove(std::shared_ptr<Figure> whatToMove){
	if(!whatToMove)
		std::cout<<"Nun kann man sich nicht mehr bewegen!"<<std::endl;
	m_whatToMove = whatToMove;
	if(whatToMove){
	DrawingElement* whatToPutOver = whatToMove->m_Image()->m_HowDrawn().front();
	std::shared_ptr<DrawingElement> pointer = whatToPutOver->shared_from_this();
	m_currentDrawing->m_putOver(pointer, SIDETEXT_LAYER);
	}
}

EventHandler::EventHandler()
: m_whatToMove(nullptr), m_currentDrawing(nullptr)
{}

bool EventHandler::m_handleEvent(const SDL_Event& event){
	if(event.type == SDL_KEYDOWN){
		if(!m_handleKeyboardEvent(event)){
			std::cout<<"No EventHandling found!"<<std::endl;
			NO_EVENT_HANDLING_FOUND
		}
		m_setWhatToMove(theGame->m_getCurrentFigure());
		m_drawMainDrawing();
		std::cout<<"\n currentNatiodn: "<<theGame->m_NationAtCurrentTurn()->m_Nation()<<std::endl;
		for(std::shared_ptr<Nation> nationToPrint: theGame->m_NationsPlaying()){
			std::cout<<nationToPrint->m_Nation()<<": "<<nationToPrint->m_activeFiguresSize()<<" active figures; "<<nationToPrint->m_Figures().size()<<" figures in general"<<std::endl;
		}
		std::cout<<"Z.51"<<std::endl;
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

bool EventHandler::m_handleKeyboardEvent(const SDL_Event& event){
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

	char orders[]={'s', 'r', ' ', 'h','b'};
	KeyCode keyCharPossibilities[]={SDLK_s, SDLK_r, SDLK_SPACE,SDLK_h, SDLK_b};
	if(m_whatToMove){
	for(int i = 0; i<5; i++){
		if(keyCode == keyCharPossibilities[i]){
			try{
			if(m_whatToMove->m_takeOrder(orders[i])){
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
	default:
		NO_EVENT_HANDLING_FOUND

	}
	return true;
}

void EventHandler::m_setCurrentDrawing(std::shared_ptr<Drawing> drawing){
	m_currentDrawing = drawing;
}

int EventHandler::m_drawMainDrawing(){
	SDL_SetRenderDrawColor(theRenderer,30,30,30,0);
	SDL_RenderClear(theRenderer);
	int whatToReturn = 0;
	if(m_currentDrawing)
	whatToReturn = m_currentDrawing->m_draw(-m_topLeftCorner.x, -m_topLeftCorner.y);
	std::cout<<"nor show figureinfo"<<std::endl;
	m_showFigureInfo();
	return whatToReturn;
}
