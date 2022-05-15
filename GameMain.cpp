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
#include "Field.hpp"
#include "FieldContainer.hpp"
#include "Window.hpp"
#include "movableThing.hpp"
#include "Figure.hpp"
#include <sstream>

#define MAIN_LOOP_BEGIN  bool quit = false; while(!quit){ \
	  Uint32 currentTime = SDL_GetTicks();\
    if (currentTime > millisecsAtLastBlinkingStep + m_blinkingIntervalTime) {\
    millisecsAtLastBlinkingStep = currentTime;\
    if(m_whatToMove){\
			m_whatToMove->m_drawFigure(m_currentBlinkingState);\
			SDL_RenderPresent(m_currentRenderer);\
			m_currentBlinkingState = !m_currentBlinkingState;\
		}\
	}\
while(SDL_PollEvent(&currentEvent)!=0){\
	if(currentEvent.type == SDL_QUIT) quit = true;\

#define MAIN_LOOP_END }}

using namespace std;
SDL_Renderer* theRenderer = nullptr;

SDL_Texture *theTexture;
SDL_Texture *theTexture2;
SDL_Event currentEvent;
//GameMain* theEventHandler = new GameMain;
shared_ptr<Drawing> someDrawing;


GameMain::GameMain(): EventHandler()
{
	m_theWindows.push_back(unique_ptr<Window>());
	m_initGame();
}

void GameMain::m_initLetterTextures(){
	std::string whatToGenerate = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	theLetterTextureContainer = new LetterTextureContainer(whatToGenerate);
}

void GameMain::m_initGame(){
	int imgflags = IMG_INIT_PNG;
	IMG_Init(imgflags);
	TTF_Init();
	theFont = TTF_OpenFont("Fonts/FT88-Regular.ttf", 15);
	citySizeFont = TTF_OpenFont("Fonts/FT88-Regular.ttf",35);
	fieldTextures = new shared_ptr<Texture>[30];
	Window* theWindow = new Window ("Game Main, Window 0", SCREEN_WIDTH, SCREEN_HEIGHT);
	m_currentRenderer = theRenderer = theWindow->m_Renderer();
	m_initLetterTextures();
	m_initFieldTextures();
	initFieldContainer();
	vector<Nationality> nationsToPlay{ROMAN, MONGOL, ZULU, CHINESE};
	cout<<"Gameinitialisierung"<<endl;
	m_theGame = make_unique<Game>(nationsToPlay);
	theGame = m_theGame.get();
	cout<<"Gameinitialisierung abgeschlossen"<<endl;
	m_whatToMove = theGame->m_getCurrentFigure(theGame->m_NationAtCurrentTurn().get());
	FieldContainer::getTheContainer()->initContinentIDs();
	m_theWindows.push_back(unique_ptr<Window>());
	m_theWindows[0].reset(theWindow);
	//TTF_Init();
	cout<<"Kevin"<<endl;
	theWindow->m_InitWindowSurface();
	someDrawing = theWindow->m_CurrentDrawing();
	//someDrawing->m_draw();
    	//SDL_RenderPresent(m_currentRenderer);
	//SDL_Delay(2000);
	cout<<"m_initInfoDrawing"<<std::endl;
	m_initInfoDrawing();
	cout<<"m_initInfoDrawing"<<std::endl;
}

void GameMain::m_createFieldTexture(Landscape ls, string filename){
	theTexture = IMG_LoadTexture(m_currentRenderer, filename.c_str());
	fieldTextures[ls]=make_shared<Texture>(theTexture, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE);
}

void GameMain::m_initFieldTextures(){
	m_createFieldTexture(GRASSLAND, "bilder/Landscapes/Grassland.png");
	cout<<"SDL_Error: "<<SDL_GetError()<<endl;
	m_createFieldTexture(PLAINS, "bilder/Landscapes/Plains.png");
	m_createFieldTexture(OCEAN, "bilder/Landscapes/Ocean.png");
}

GameMain::~GameMain(){
	delete fieldTextures;
	TTF_CloseFont(theFont);
	delete theLetterTextureContainer;
}

void GameMain::m_initInfoDrawing(){
	m_currentFigureInfo = std::make_shared<Drawing>(m_currentRenderer, -FIGURE_INFO_WIDTH, 0, SIDETEXT_LAYER);
	std::shared_ptr<DrawingElement> rectPointer = std::make_shared<LambdaDrawingElement>(m_currentRenderer, [] (int x, int y, SDL_Renderer* renderer)->int {
	SDL_Color& backgroundColor = infoTextBackgroundColor;
		SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		SDL_Rect rectToFill{x,y,FIGURE_INFO_WIDTH, SCREEN_HEIGHT};
		//std::cout<<"yCoordinate: rectToFill.y = " <<rectToFill.y<<", y = "<<y<<std::endl;
		SDL_RenderFillRect(renderer, &rectToFill);
		return 0;
	},
	0,FIGURE_INFO_Y,STANDARD_LAYER + 50);
	m_currentFigureInfo->m_add(rectPointer);
	auto& r = m_currentFigureInfo;
	m_setCurrentDrawing(someDrawing);
	//cout<<m_currentFigureInfo<<std::endl;
	m_currentDrawing->m_add(r);
}

int GameMain::operator()(){
	Uint32 millisecsAtLastBlinkingStep = SDL_GetTicks();
 if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ){
  std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
 }else{
 doSomething();
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
	if(m_handleEvent(currentEvent)){
	    std::cout<<"gewaltig"<<std::endl;
		//TTF_Font* theFont = TTF_OpenFont("Fonts/javatext.ttf", 24);
		//SDL_Color theColor {233, 46, 56};
	//	screenSurface = TTF_RenderText_Blended(theFont, "Komischer Text in komischer Schrift",  theColor);
	}
  MAIN_LOOP_END
  //theMovableThing2->m_setMoveToDirection(UP_RIGHT);
  //theMovableThing2->m_move();
 }			//SDL_Delay(1000);
  //Texture* texture =new Texture(theTexture,someColoredRect.w, someColoredRect.h);
  //MovableThing* theMovableThing = new MovableThing(theRenderer, texture, 200, 350, true);
  //SDL_Delay(3000);
  //theMovableThing->m_drawRight();

 TTF_Quit();
 IMG_Quit();
 SDL_Quit();  //Quit SDL subsystems
 return 0;
}

void GameMain::doSomething(){
	 for(std::shared_ptr<Nation> currentNation: theGame->m_NationsPlaying()){
		for(shared_ptr<Figure> currentFigure : currentNation->m_Figures()){
			currentFigure->m_integrateInto(*someDrawing);
		}
	}
		 Window* theWindow=&(*m_currentWindow());
		   if(theWindow)cout<<"Window existiert"<<endl;

		  //theRenderer = theWindow->m_Renderer();
		  FieldContainer& fc = *theContainer;

		someDrawing->m_draw();
		m_showFigureInfo();
		SDL_RenderPresent(m_currentRenderer);


		  //theMovableThing2->m_setMoveToDirection(UP_RIGHT);
		  //theMovableThing2->m_move();
		 }			//SDL_Delay(1000);
		  //Texture* texture =new Texture(theTexture,someColoredRect.w, someColoredRect.h);
		  //MovableThing* theMovableThing = new MovableThing(theRenderer, texture, 200, 350, true);
		  //SDL_Delay(3000);
		  //theMovableThing->m_drawRight();


void GameMain::gameMainDebug(list<SDL_Event>& eventList){
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ){
	  std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
	 }else{
	/*	 if( TTF_Init() == -1 )
		                 {
		                     printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
		                 }*/

	 doSomething();
	 for(SDL_Event& event: eventList){
		 m_handleKeyboardEvent(event);
		 std::cout<<"GameMain-Loop-Ende"<<std::endl;
	 }
	 std::cout<<"Ende"<<std::endl;

	 }
}

void GameMain::m_makeBlinkingStep(){
	//change blinking state
	m_currentBlinkingState = !m_currentBlinkingState;
	m_whatToMove->m_drawFigure(m_currentBlinkingState);
}
