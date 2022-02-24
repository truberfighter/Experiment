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

class EventHandler{
protected:
	MovableThing* m_whatToMove;
	//Direction m_directionFromKeyPressed(unsigned int keysym);
	bool m_handleKeyboardEvent(const SDL_Event& event);
	std::list<Drawing*> m_currentDrawings;//Oder so. Damit "Wait" funktioniert.
	std::shared_ptr<Drawing> m_currentDrawing;
public:
	EventHandler();
	bool m_handleEvent(const SDL_Event& event);
	void m_setWhatToMove(MovableThing* whatToMove);
	bool m_deleteDrawing(Drawing* drawingToDelete);
	bool m_addDrawing(Drawing* drawingToAdd);
	void m_setCurrentDrawing(std::shared_ptr<Drawing> drawing);
	void m_draw();
};






#endif //EVENTHANDLER_HPP_


