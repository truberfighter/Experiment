/*
 * EventHandler.hpp
 *
 *  Created on: 19.02.2022
 *      Author: uwe-w
 */

#ifndef EVENTHANDLER_HPP_
#define EVENTHANDLER_HPP_

#include <SDL2\SDL.h>
#include <iostream>
#include <SDL2\SDL_image.h>
#include "sdltypes.hpp"
#include "movableThing.hpp"
#include "Drawing.hpp"
#include "Window.hpp"
#include <queue>
#include "Game.hpp"

class Figure;

class EventHandler{
protected:
	std::list<Drawing*> m_currentDrawings;//Oder so. Damit "Wait" funktioniert.
	std::shared_ptr<Drawing> m_currentDrawing;
	void m_showFigureInfo();
public:
	virtual ~EventHandler(){}
	virtual bool m_handleKeyboardEvent(const SDL_Event& event) = 0;
	virtual void lol(){std::cout<<"virtual EventHandler::lol"<<std::endl;}
	EventHandler();
	virtual bool m_handleEvent(const SDL_Event& event) = 0;
	bool m_deleteDrawing(Drawing* drawingToDelete);
	bool m_addDrawing(Drawing* drawingToAdd);
	void m_setCurrentDrawing(std::shared_ptr<Drawing> drawing);
};

extern Game* theGame;




#endif //EVENTHANDLER_HPP_


