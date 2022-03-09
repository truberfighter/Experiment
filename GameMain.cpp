/*
 * GameMain.cpp
 *
 *  Created on: 04.03.2022
 *      Author: uwe-w
 */

#include "EventHandler.hpp"
#include <iostream>
#include "sdltypes.hpp"
#include "GameMain.hpp"

using namespace std;


SDL_Renderer *theRenderer;
SDL_Texture *theTexture;
SDL_Texture *theTexture2;
SDL_Event currentEvent;
GameMain* theEventHandler = new GameMain;
shared_ptr<Drawing> someDrawing;

GameMain::GameMain(): EventHandler()
{}

void GameMain::m_initGame(){
	Window theWindow("SDL Tutorial lol", SCREEN_WIDTH, SCREEN_HEIGHT);

	  //Create window
	  SDL_Texture *noOneLikesU;
	  theRenderer = theWindow.m_Renderer();
	  //TTF_Init();
	  cout<<"guenther"<<endl;

	  someDrawing = /*make_shared<Drawing>(theWindow.m_Renderer());*/ theWindow.m_CurrentDrawing();
	  cout<<"Kevin"<<endl;
	  //if(theRenderer==NULL)cout<<"Guenther"<<endl;
	  /*if( window == NULL ){
	   std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
	  }else{*/
	   /*screenSurface = SDL_GetWindowSurface( window ); //Get window surface
	   SDL_FillRect( screenSurface, NULL
	   , SDL_MapRGB( screenSurface->format, 0x00, 0xFF, 0xFF ) );
	   //Fill the surface with color
	   SDL_UpdateWindowSurface( window ); //Update the surface*/
	   theTexture = IMG_LoadTexture(theRenderer, "bilder/2022-01-03.png");
	   shared_ptr<MovableThing> theMovableThing2;
}
