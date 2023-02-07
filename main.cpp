/*
 * main.cpp
 *
 *  Created on: 09.02.2022
 *      Author: uwe-w
 */
#include <SDL2\SDL.h>
#include "sdltypes.hpp"
#include "movableThing.hpp"
#undef main
#include <iostream>
#include "Technologytypes.hpp"
#include <SDL2\SDL_image.h>
#include "drawing.hpp"
#include "EventHandler.hpp"
#include "Window.hpp"
#include "Field.hpp"
#include "GameMain.hpp"
#include "SelectorSurface.hpp"
#include "FieldContainer.hpp"
#include "GameLoader.hpp"
#include "AppInitter.hpp"
#include <functional>


//Screen dimension constants
//const int SCREEN_WIDTH = 640,SCREEN_HEIGHT= 480;
//SDL_Renderer *theRenderer;
//SDL_Texture *theTexture;
//SDL_Texture *theTexture;
using namespace std;
//SDL_Event currentEvent;
//EventHandler* theEventHandler = new EventHandler;
//shared_ptr<Drawing> someDrawing;
SDL_Event currentEvent;

int main(int argc, char* args[] ){
	AppInitter initter;
	initter.m_init();
	switch(initter.m_requestStartingMode()){
	case LOAD_GAME:{ //load
		//GameLoader theGameLoader;
		break;
	}
	case START_A_NEW_GAME: //new
	{
		GameMain gameMain;
		gameMain();
	}
	}
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
	return 500;
}




