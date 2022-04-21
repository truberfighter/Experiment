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
	std::shared_ptr<Figure> m_whatToMove;
	//Direction m_directionFromKeyPressed(unsigned int keysym);
	bool m_handleKeyboardEvent(const SDL_Event& event);
	std::list<Drawing*> m_currentDrawings;//Oder so. Damit "Wait" funktioniert.
	std::shared_ptr<Drawing> m_currentDrawing;
	void m_showFigureInfo();
public:
	virtual void m_initInfoDrawing() = 0;
	virtual void lol(){std::cout<<"virtual EventHandler::lol"<<std::endl;}
	EventHandler();
	bool m_handleEvent(const SDL_Event& event);
	void m_setWhatToMove(std::shared_ptr<Figure> whatToMove);
	bool m_deleteDrawing(Drawing* drawingToDelete);
	bool m_addDrawing(Drawing* drawingToAdd);
	void m_setCurrentDrawing(std::shared_ptr<Drawing> drawing);
	void m_draw();
};

extern Game* theGame;




#endif //EVENTHANDLER_HPP_


