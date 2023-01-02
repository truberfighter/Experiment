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
#include "Figurebutton.hpp"
#include <sstream>
#include "ctype.h"
#include "City.hpp"
#include "SelectorSurface.hpp"
#include<algorithm>

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
	try{if(currentEvent.type == SDL_QUIT) quit = true;\

#define MAIN_LOOP_END 	}catch(SDLQuitException& exception){quit = true;}}}

using namespace std;
SDL_Renderer* theRenderer = nullptr;

SDL_Texture *theTexture;
SDL_Texture *theTexture2;
//GameMain* theEventHandler = new GameMain;
shared_ptr<Drawing> someDrawing;


GameMain::GameMain(): EventHandler(), m_whatToMove(nullptr)
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
	theFont = TTF_OpenFont("Fonts/FT88-Regular.ttf", 22);
	citySizeFont = TTF_OpenFont("Fonts/FT88-Regular.ttf",35);
	Window* theWindow = new Window ("Game Main, Window 0", SCREEN_WIDTH, SCREEN_HEIGHT);
	m_currentRenderer = theRenderer = theWindow->m_Renderer();
	m_initLetterTextures();
	m_initFieldTextures();
	initFieldContainer();
	vector<Nationality> nationsToPlay{ROMAN, MONGOL, ZULU, CHINESE};
	m_theGame = make_unique<Game>(nationsToPlay);
	theGame = m_theGame.get();
	m_whatToMove = theGame->m_getCurrentFigure(theGame->m_NationAtCurrentTurn().get());
	FieldContainer::getTheContainer()->initContinentIDs();
	m_theWindows.push_back(unique_ptr<Window>());
	m_theWindows[0].reset(theWindow);
	//TTF_Init();
	theWindow->m_InitWindowSurface();
	someDrawing = theWindow->m_CurrentDrawing();
	m_setCurrentDrawing(someDrawing);
//someDrawing->m_draw();
    	//SDL_RenderPresent(m_currentRenderer);

}

void GameMain::m_createFieldTexture(Landscape ls, string filename){
	theTexture = IMG_LoadTexture(m_currentRenderer, filename.c_str());
	fieldTextures.push_back(make_shared<Texture>(theTexture, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE));
}

void GameMain::m_createResourceTexture(Landscape ls, string filename){
	theTexture = IMG_LoadTexture(m_currentRenderer, filename.c_str());
	resourceTextures.push_back(make_shared<Texture>(theTexture, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE));
}

void GameMain::m_createShieldTexture(Landscape ls, string filename){
	theTexture = IMG_LoadTexture(m_currentRenderer, filename.c_str());
	shieldTextures.push_back(make_shared<Texture>(theTexture, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE));
}

void GameMain::m_initFieldTextures(){
	for(Landscape ls = (Landscape)0; ls<=OCEAN;ls=(Landscape)(ls+1)){
		std::stringstream stream;
		stream<<ls;stream.flush();
		std::string landscapeString = stream.str();
		int howFar = 0;
		std::transform(landscapeString.begin(), landscapeString.end(), landscapeString.begin(),[&howFar](unsigned char c){return 0 < howFar++ ? tolower(c) : c; });
		stream.clear();
		std::stringstream normalStream;
		normalStream<<"bilder/Landscapes/Default/"<<landscapeString<<".png";
		m_createFieldTexture(ls, normalStream.str().c_str());
		/*
		std::stringstream resourceStream;
		normalStream<<"bilder/Landscapes/Resource/"<<landscapeString<<".png";
		m_createResourceTexture(ls, resourceStream.str().c_str());
		std::stringstream shieldStream;
		normalStream<<"bilder/Landscapes/Shield/"<<landscapeString<<".png";
		m_createShieldTexture(ls, shieldStream.str().c_str());
		 */
	}
}

GameMain::~GameMain(){
	TTF_CloseFont(theFont);
	delete theLetterTextureContainer;
}

void GameMain::m_initInfoDrawing(){
	m_currentFigureInfo = std::make_shared<Drawing>(m_currentRenderer, -FIGURE_INFO_WIDTH, 0, SIDETEXT_LAYER);
	std::shared_ptr<DrawingElement> rectPointer = std::make_shared<LambdaDrawingElement>(m_currentRenderer, [] (int x, int y, SDL_Renderer* renderer)->int {
	SDL_Color& backgroundColor = infoTextBackgroundColor;
		SDL_SetRenderDrawColor(renderer, backgroundColor.r, backgroundColor.g, backgroundColor.b, backgroundColor.a);
		SDL_Rect rectToFill{x,y,FIGURE_INFO_WIDTH, SCREEN_HEIGHT};
		SDL_RenderFillRect(renderer, &rectToFill);
		return 0;
	},
	0,FIGURE_INFO_Y,STANDARD_LAYER + 50);
	m_currentFigureInfo->m_add(rectPointer);
	auto& r = m_currentFigureInfo;
	m_currentDrawing->m_add(r);
}

int GameMain::operator()(){
	Uint32 millisecsAtLastBlinkingStep = SDL_GetTicks();
 if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ){
  std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
 }else{
 doSomething();

 MAIN_LOOP_BEGIN


	if(m_handleEvent(currentEvent)){
true;	}
  MAIN_LOOP_END
 }

 TTF_Quit();
 IMG_Quit();
 SDL_Quit();
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
		m_showNationInfo();
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

	 doSomething();
	 for(SDL_Event& event: eventList){
		 m_handleKeyboardEvent(event);
	 }
	 }
}

void GameMain::m_makeBlinkingStep(){
	//change blinking state
	m_currentBlinkingState = !m_currentBlinkingState;
	m_whatToMove->m_drawFigure(m_currentBlinkingState);
}

bool GameMain::m_handleLeftClick(const SDL_MouseButtonEvent& currentEvent){
	try{
	std::cout<<"Mouse leftclicked on Board: x = "<<currentEvent.x<<", y = "<<currentEvent.y<<", topLeftCorner: "<<m_topLeftCorner<<std::endl;
	Coordinate meantCoordinate {xModulo(m_topLeftCorner.x + currentEvent.x - Coordinates::leftCornerX()), m_topLeftCorner.y + currentEvent.y - Coordinates::leftCornerY()};
	if(currentEvent.x > Coordinates::leftCornerX() && currentEvent.y > Coordinates::leftCornerY() && currentEvent.x < SCREEN_WIDTH && currentEvent.y < SCREEN_HEIGHT && meantCoordinate.x>=0 && meantCoordinate.y>=0){
	if(meantCoordinate.x < WORLD_LENGTH*STANDARD_FIELD_SIZE && meantCoordinate.y<WORLD_HEIGHT*STANDARD_FIELD_SIZE){
	std::shared_ptr<Field> fieldClickedOn = (*theContainer->m_getFieldsOfTheWorld())[meantCoordinate.x/STANDARD_FIELD_SIZE][meantCoordinate.y/STANDARD_FIELD_SIZE];
	std::cout<<*fieldClickedOn<<std::endl;
	if(fieldClickedOn->m_CityContained() && fieldClickedOn->m_CityContained()->m_OwningNation()==theGame->m_NationAtCurrentTurn()){
		std::cout<<"city hit!"<<std::endl;
		fieldClickedOn->m_CityContained()->m_createCitySurface().m_displaySurface(theRenderer);
		return true;
	}
	if(!fieldClickedOn->m_FiguresOnField().empty() && fieldClickedOn->m_FiguresOnField().front()->m_Nationality() == theGame->m_NationAtCurrentTurn()->m_Nation()){
		Figurebutton theButton(fieldClickedOn->m_FiguresOnField());
		std::shared_ptr<Figure> figureToMove = theButton();
		//figureToMove is not active
		if(figureToMove==nullptr){
			return true;
		}
		std::cout<<"figureToMoveNow: "<<figureToMove->m_figureOverview()<<std::endl;
		while(theGame->m_getCurrentFigure().get()!=figureToMove.get()){
			theGame->m_getCurrentFigure()->m_wait();
		}
		return true;
	}
	}

		//Centralize arou nd the clicked spot
	m_topLeftCorner.x = xModulo(m_topLeftCorner.x + currentEvent.x - (SCREEN_WIDTH + Coordinates::leftCornerX())/2);
	m_topLeftCorner.y = m_topLeftCorner.y + currentEvent.y - (SCREEN_HEIGHT + Coordinates::leftCornerY())/2;
		//Scroll pole hit
	m_topLeftCorner.y = max(0,m_topLeftCorner.y);
	m_topLeftCorner.y = min(m_topLeftCorner.y, WORLD_HEIGHT*STANDARD_FIELD_SIZE - SCREEN_HEIGHT);
	std::cout<<"scrolling done"<<std::endl;
	return true;
	}
	return false;
	}
	catch(SDLQuitException& exception){
		throw exception;
	}
}


