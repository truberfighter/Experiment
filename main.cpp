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

#define MAIN_LOOP_BEGIN  bool quit = false; while(!quit){ while(SDL_PollEvent(&currentEvent)!=0){if(currentEvent.type == SDL_QUIT) quit = true;
#define MAIN_LOOP_END }}
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
	initFieldContainer();
	vector<Meridian>& meridianContainer = *theContainer->m_getFieldsOfTheWorld();
	Settlers s;
	Meridian& meridian = meridianContainer[0];
	if(meridian[0]->m_Mining(s))
		cout<<"MiningSuccess<<"<<endl;
	else
		cout<<"MiningFail"<<endl;

	   //MovableThing* mmm = nullptr;

	   //mmm->m_add(nullptr);

//The window we'll be rendering to
 SDL_Window* window = NULL;
 //The surface contained by the window
 SDL_Surface* screenSurface = NULL;
 SDL_Surface* screenSurface2 = NULL;
 //Initialize SDL
 if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
  std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
 }else{
	 cout<<"hä?"<<endl;
/*	 if( TTF_Init() == -1 )
	                 {
	                     printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
	                 }*/
	 int imgflags = IMG_INIT_PNG;
	   	cout<<"kevin19"<<endl;
	   IMG_Init(imgflags);
   Window theWindow("SDL Tutorial lol", SCREEN_WIDTH, SCREEN_HEIGHT);

  //Create window
  SDL_Texture *noOneLikesU;
  theRenderer = theWindow.m_Renderer();
  FieldContainer& fc = *theContainer;
  Meridian& m = (*(fc.m_getFieldsOfTheWorld()))[0];cout<<"main 63"<<endl;
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
   theMovableThing2 = make_shared<MovableThing>(theRenderer, 90, 90, "bilder/2022-02-12.png", 0, 0, true);;
   if(theTexture == NULL) cout<<"kevinkevinkevin"<<endl;
   for(int i(0); i<10; i++){
   SDL_Rect *theRect = new SDL_Rect;
   cout<<"i="<<i<<endl;
   theRect->x = 64*i;
   theRect->y = 50;
   theRect->h = 225;
   theRect->w = 64;
   noOneLikesU = theTexture;
   //SDL_RenderClear(theRenderer);
   SDL_RenderCopy(theRenderer, theTexture, NULL, theRect);
   SDL_RenderPresent(theRenderer);
   //SDL_UpdateWindowSurface( window ); //Update the surface
   SDL_Delay(300);
  }}
  SDL_Rect someColoredRect;
  someColoredRect.x = 34;
  someColoredRect.y = 20;
  someColoredRect.h = 125;
  someColoredRect.w = 84;
  SDL_SetRenderDrawColor(theRenderer, 240, 240, 240, 255);
  SDL_RenderFillRect(theRenderer, &someColoredRect);
  SDL_RenderPresent(theRenderer);
  //SDL_Delay(2000);
  //SDL_DestroyTexture(noOneLikesU);
  //SDL_RenderPresent(theRenderer);
  std::shared_ptr<Texture> texture =std::make_shared <Texture>(theTexture,someColoredRect.w, someColoredRect.h);
  MovableThing* theMovableThing = new MovableThing(theRenderer, texture, 200, 350, true);
  cout<<"theMovableThing erfolgreich initialisiert"<<endl;
  //MovableDrawingElement mel(theRenderer, theMovableThing);
  if(!someDrawing) cout<<"lol"<<endl;
  someDrawing->m_add(theMovableThing);cout<<"line 91"<<endl;
  //SDL_Delay(3000);
    //theMovableThing->m_drawRight();
    SDL_Delay(400);
    MovableThing* theMovableThing2 = new MovableThing(theRenderer, 90, 90, "bilder/2022-02-12.png", 280, 220, true);;
    //auto a = new MovableDrawingElement(theRenderer, theMovableThing2);
    cout<<"theMovableThing2 erfolgreich initialisiert"<<endl;
    someDrawing->m_add(theMovableThing2);
    SDL_RenderPresent(theRenderer);
    SDL_Delay(STANDARD_DRAWING_TIME);
    theEventHandler->m_setWhatToMove(theMovableThing);
    std::cout<<"g"<<std::endl;
    theEventHandler->m_setCurrentDrawing(someDrawing);
    MAIN_LOOP_BEGIN
  /*if(currentEvent.type == SDL_KEYDOWN){
	  cout<<"SDL_KEYDOWN, nämlich: "<<currentEvent.key.keysym.sym<<endl;
  		switch(currentEvent.key.keysym.sym){
  			case SDLK_9_UP_RIGHT:{
  				theMovableThing->m_setMoveToDirection(UP_RIGHT);
  				theMovableThing->m_move();
  				constexpr bool JUST_DELETE_ALL = true;
  				someDrawing.m_draw(JUST_DELETE_ALL);

  				break;
  			}
  		}
  	}*/
	if(theEventHandler->m_handleEvent(currentEvent)){
		//TTF_Font* theFont = TTF_OpenFont("Fonts/javatext.ttf", 24);
		//SDL_Color theColor {233, 46, 56};
	//	screenSurface = TTF_RenderText_Blended(theFont, "Komischer Text in komischer Schrift",  theColor);
	}
  MAIN_LOOP_END
  theMovableThing2->m_setMoveToDirection(UP_RIGHT);
  theMovableThing2->m_move();
  someDrawing->m_draw(0);
    				//SDL_Delay(1000);
  //Texture* texture =new Texture(theTexture,someColoredRect.w, someColoredRect.h);
  //MovableThing* theMovableThing = new MovableThing(theRenderer, texture, 200, 350, true);
  SDL_Delay(3000);
  //theMovableThing->m_drawRight();

 SDL_DestroyWindow( window ); //Destroy window
 //TTF_Quit();
 IMG_Quit();
 SDL_Quit();  //Quit SDL subsystems
 return 0;
}




