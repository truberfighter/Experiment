/*
 * EventHandler.cpp
 *
 *  Created on: 19.02.2022
 *      Author: uwe-w
 */

#include "EventHandler.hpp"
#include <iostream>
#include "sdltypes.hpp"
#define NO_EVENT_HANDLING_FOUND return true;

void EventHandler::m_setWhatToMove(MovableThing* whatToMove){
	if(!whatToMove)
		std::cout<<"Nun kann man sich nicht mehr bewegen!"<<std::endl;
	m_whatToMove = whatToMove;
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
	}
	return true;
}

bool EventHandler::m_handleKeyboardEvent(const SDL_Event& event){
	KeyCode keyCode = event.key.keysym.sym;
	std::cout<<"Man hat den KeyCode "<<keyCode<<std::endl;
	Direction directions[13] = {DOWN_LEFT, DOWN, DOWN_RIGHT, LEFT, STANDING_STILL,
			RIGHT, UP_LEFT, UP, UP_RIGHT, UP, LEFT, RIGHT, DOWN};
	KeyCode keyPossibilities[13] = {SDLK_1_DOWN_LEFT, SDLK_2_DOWN, SDLK_3_DOWN_RIGHT,
			SDLK_4_LEFT, SDLK_5_CENTER, SDLK_6_RIGHT,
			SDLK_7_UP_LEFT, SDLK_8_UP, SDLK_9_UP_RIGHT, SDLK_UP, SDLK_LEFT, SDLK_RIGHT, SDLK_DOWN};
	for(int i = 0; i<13; i++){
		if(keyCode == keyPossibilities[i]){
			m_whatToMove->m_setMoveToDirection(directions[i]);
			m_whatToMove->m_move();
			m_currentDrawing->m_draw(true);		}
	}
	switch(event.key.keysym.sym){

	default:
		NO_EVENT_HANDLING_FOUND

	}
	return true;
}

void EventHandler::m_setCurrentDrawing(Drawing* drawing){
	m_currentDrawing = drawing;
}
