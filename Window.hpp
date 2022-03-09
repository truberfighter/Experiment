/*
 * Window.hpp
 *
 *  Created on: 15.02.2022
 *      Author: uwe-w
 */

#ifndef WINDOW_HPP_
#define WINDOW_HPP_

#include <SDL2\SDL.h>
#include "sdltypes.hpp"
#include "movableThing.hpp"
#include <iostream>
#include <SDL2\SDL_image.h>
#include "drawing.hpp"
#include "EventHandler.hpp"
#include <vector>
#include <string>
#include <string>
#include <stdexcept>

class WindowInitFail: public std::exception{
	 std::string what(){return "Some window could not be created.";}
};

class SetCurrentDrawingFail: public std::exception{
public:
	int m_index;
	SetCurrentDrawingFail(int index):std::exception(),m_index(index){}
	std::string what(){return "Bad index for new current drawing of window";}
};

class Window{
private:
	std::string m_name;
	SDL_Renderer* m_renderer;
	std::shared_ptr<Drawing> m_currentDrawing;
	SDL_Window* m_window;
	//werd ich gucken müssen, ob list oder vector
	//Idee: mehrere Drawings pro Window, dass man switchen kann.
	std::vector<std::shared_ptr<Drawing>> m_mainDrawings;
	unsigned int m_width;
	unsigned int m_height;
public:
	Window(std::string name, unsigned int width, unsigned int height);
	std::vector<std::shared_ptr<Drawing>> m_MainDrawings();
	unsigned int m_Height();
	unsigned int m_Width();
	SDL_Renderer* m_Renderer();
	bool m_Init();
	std::string m_Name();
	std::shared_ptr<Drawing> m_CurrentDrawing();
	void m_setCurrentDrawing(int drawingNr);
	int m_createNewDrawing();
	void m_InitWindowSurface();
};

#endif /* WINDOW_HPP_ */
