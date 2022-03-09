/*
 * Window.cpp
 *
 *  Created on: 15.02.2022
 *      Author: uwe-w
 */

#include <SDL2\SDL.h>
#include "sdltypes.hpp"
#include "movableThing.hpp"
#include <iostream>
#include <SDL2\SDL_image.h>
#include "drawing.hpp"
#include "EventHandler.hpp"
#include "Window.hpp"
#include <SDL2\SDL_image.h>
#include<vector>

using namespace std;

Window::Window(std::string name, unsigned int width, unsigned int height)
:m_name(name), m_width(width), m_height(height)
{
	try{
	m_Init();
	}
	catch(WindowInitFail someFail){
		throw(WindowInitFail());
	}
}

bool Window::m_Init()
{
	m_window = SDL_CreateWindow(m_name.c_str(), SDL_WINDOWPOS_UNDEFINED
			  , SDL_WINDOWPOS_UNDEFINED
			  , m_width, m_height, SDL_WINDOW_SHOWN );
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if(m_window == nullptr){
		throw(WindowInitFail());
	}
	m_InitWindowSurface();
	std::cout<<"InitWindowSurface hat geklappt"<<std::endl;
	return true;
}

unsigned int Window::m_Width(){
	return m_width;
}

unsigned int Window::m_Height(){
	return m_height;
}

SDL_Renderer* Window::m_Renderer(){
	return m_renderer;
}

void Window::m_setCurrentDrawing(int drawingNr){
	if((int) m_mainDrawings.size()<=drawingNr || drawingNr<0){
		throw(SetCurrentDrawingFail(drawingNr));
	}
	m_currentDrawing = m_mainDrawings[drawingNr];
}

std::shared_ptr<Drawing> Window::m_CurrentDrawing(){
	return m_currentDrawing;
}

int Window::m_createNewDrawing(){
	m_mainDrawings.push_back(make_shared<Drawing>(m_renderer));
	return m_mainDrawings.size();
}

void Window::m_InitWindowSurface(){

	SDL_Texture* sdltexture[2] = {IMG_LoadTexture( m_renderer, "bilder/Landscapes/Plains.png"), IMG_LoadTexture( m_renderer, "bilder/Landscapes/Grassland.png")};
	std::shared_ptr<Texture> theTextures[2] = {make_shared<Texture>(sdltexture[0], STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE),
			std::make_shared<Texture>(sdltexture[1], STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE)
	};
	 m_createNewDrawing();
	 cout<<"NewDrawingCreated"<<endl;
	 m_setCurrentDrawing( m_mainDrawings.size()-1);
	//std::vector<unique_ptr<ImmovableDrawingElement>> v;
	Drawing& currentDrawing = *( m_currentDrawing);
	cout<<"currentDrawing initialized"<<endl;
	for(int i(-15); i < 16; i++){
			for(int j(-15); j<16; j++){
				currentDrawing.m_add(make_shared<ImmovableDrawingElement>(currentDrawing.m_Renderer(), theTextures[modulo(i+j,2)], xModulo(i * STANDARD_FIELD_SIZE), yModulo(j * STANDARD_FIELD_SIZE)));
				cout<<"m_add aufgerufen für i = "<<i<<", j = "<<j<<endl;
			}
	}
	currentDrawing.m_draw();
}




