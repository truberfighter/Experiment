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
#include <SDL2\SDL_image.h>
#include "drawing.hpp"
#include "EventHandler.hpp"
#include "Window.hpp"
#include "Field.hpp"
#include "GameMain.hpp"
#include "FieldContainer.hpp"
#include "Settlers.hpp"
#include "Grassland.hpp"


//Screen dimension constants
//const int SCREEN_WIDTH = 640,SCREEN_HEIGHT= 480;
//SDL_Renderer *theRenderer;
//SDL_Texture *theTexture;
//SDL_Texture *theTexture;
using namespace std;
//SDL_Event currentEvent;
//EventHandler* theEventHandler = new EventHandler;
//shared_ptr<Drawing> someDrawing;

int main( int argc, char* args[] ){

	GameMain gameMain;
	gameMain();
}




