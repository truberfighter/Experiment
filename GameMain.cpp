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
#include "GameSaver.hpp"
#include <fstream>
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


GameMain::GameMain(StartingMode mode): EventHandler(), m_whatToMove(nullptr)
{
	if(mode==START_A_NEW_GAME){
		m_theWindows.push_back(unique_ptr<Window>());
		m_initGame();
	}
}

GameMain::~GameMain(){
	TTF_CloseFont(theFont);
	delete theLetterTextureContainer;
	 if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ){
	  std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
	 }
}

void GameMain::m_initLetterTextures(){
	std::string whatToGenerate = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	theLetterTextureContainer = new LetterTextureContainer(whatToGenerate);
}

void GameMain::m_initGame(){
	m_currentRenderer = theRenderer;
	m_initLetterTextures();
	m_initFieldTextures();
	FieldContainer::initFieldContainer(WORLD_HEIGHT,WORLD_LENGTH);
	Graphics::Civ::miningTexture = std::make_shared<Texture>(IMG_LoadTexture(theRenderer, "bilder/Landscapes/Mining.png"),STANDARD_FIELD_SIZE/2+1,STANDARD_FIELD_SIZE/2+1);
	vector<Nationality> nationsToPlay{ROMAN, MONGOL, ZULU, CHINESE};
	m_theGame = make_unique<Game>();
	theGame = m_theGame.get();
	theContainer->m_initFogInfos(nationsToPlay);
	theGame->m_initDefault(nationsToPlay);
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
	for(Landscape ls = (Landscape)0; ls<=DESERT;ls=(Landscape)(ls+1)){
		std::stringstream stream;
		stream<<ls;stream.flush();
		std::string landscapeString = stream.str();
		int howFar = 0;
		std::transform(landscapeString.begin(), landscapeString.end(), landscapeString.begin(),[&howFar](unsigned char c){return 0 < howFar++ ? tolower(c) : c; });
		stream.clear();
		std::stringstream normalStream;
		normalStream<<"bilder/Landscapes/Default/"<<landscapeString<<".png";
		m_createFieldTexture(ls, normalStream.str().c_str());
		std::cout<<ls<<SDL_GetError()<<std::endl;
		std::stringstream resourceStream;
		resourceStream<<"bilder/Landscapes/Resource/"<<landscapeString<<".png";
		m_createResourceTexture(ls, resourceStream.str().c_str());
		std::cout<<ls<<SDL_GetError()<<std::endl;
		std::stringstream shieldStream;
		shieldStream<<"bilder/Landscapes/Shield/"<<landscapeString<<".png";
		m_createShieldTexture(ls, shieldStream.str().c_str());
		std::cout<<ls<<SDL_GetError()<<std::endl;
	}
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
	doSomething();

 MAIN_LOOP_BEGIN


	if(m_handleEvent(currentEvent)){
true;	}
  MAIN_LOOP_END
 return 0;
}

void GameMain::doSomething(){
	for(auto& it: *theContainer->m_getFieldsOfTheWorld()){
		for(std::shared_ptr<Field> currentField: it){
			for(std::shared_ptr<Figure> currentFigure: currentField->m_FiguresOnField()){
				currentFigure->m_integrateInto(*someDrawing);
			}
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
	 doSomething();
	 for(SDL_Event& event: eventList){
		 m_handleKeyboardEvent(event);
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
	Field* fieldClickedOn = (*theContainer->m_getFieldsOfTheWorld())[meantCoordinate.x/STANDARD_FIELD_SIZE][meantCoordinate.y/STANDARD_FIELD_SIZE].get();
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

void GameMain::m_offerSavingGame(){
	try{
	SelectionReturn theReturn = Miscellaneous::selectSavingSlot();
	int fileIndex = theReturn.index;
	std::stringstream nameStream;
	nameStream<<"data\\gameData"<<fileIndex<<".txt";
	nameStream.flush();
	//clear the selected file
	std::ofstream outputStream1(nameStream.str(),std::ofstream::out|std::ofstream::trunc);
	if(static_cast<bool>(outputStream1)){
	outputStream1.close();
	}
	else{
		std::cerr<<"Problem: file "<<nameStream.str()<<" could not be opened."<<std::endl;
		return;
	}
	SDL_RenderClear(theRenderer);
	SDL_RenderPresent(theRenderer);
	slotNumber = fileIndex;
	bool displayStarted = true;
	std::string slotNameSuggestion = theReturn.content.compare("EMPTY") == 0?
			"Some saving slot":
			theReturn.content;
	SDL_StartTextInput();
	SDL_SetRenderDrawColor(theRenderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a);
	bool ready = false;
	SDL_Surface* mainTextSurface = TTF_RenderText_Shaded(theFont, "Enter slot name:", blackColor, whiteColor);
	SDL_Texture* mainTextTexture = SDL_CreateTextureFromSurface(theRenderer, mainTextSurface);
	SDL_Rect fillingRect{FIGURE_INFO_WIDTH, (SCREEN_HEIGHT -mainTextSurface->h)/2,mainTextSurface->w,mainTextSurface->h, };
	SDL_RenderCopy(theRenderer, mainTextTexture,NULL,&fillingRect);
	SDL_RenderPresent(theRenderer);
	fillingRect.y+=fillingRect.h;
	SDL_Event currentEvent;
	bool clearFirstChar = false;
	while(!ready){
		bool renderText = false;
		while(SDL_PollEvent(&currentEvent)!=0){
			if(currentEvent.type == SDL_QUIT){
				throw(SDLQuitException());
			}
			else if(currentEvent.type == SDL_KEYDOWN){
				if(currentEvent.key.keysym.sym==SDLK_BACKSPACE && slotNameSuggestion.length()>0){
					if(slotNameSuggestion.length()==1){
						clearFirstChar = true;
					}
					else{
					slotNameSuggestion.pop_back();
					renderText = true;
					}				}
				if(currentEvent.key.keysym.sym==SDLK_ENTER_KEY){
					ready = true;
				}
			}
			else if(currentEvent.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL) )
				{
					SDL_SetClipboardText( slotNameSuggestion.c_str() );
				}
				//Handle paste
				else if(currentEvent.key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL) )
				{
				if(clearFirstChar){
					clearFirstChar = false;
					slotNameSuggestion.pop_back();
				}
					slotNameSuggestion = SDL_GetClipboardText();
					renderText = true;
				}
				else if(currentEvent.type == SDL_TEXTINPUT && slotNameSuggestion.length()<MAX_SAVESLOT_LENGTH){
					//Not copy or pasting, taken from lazyfoo
					if(!(( SDL_GetModState() & KMOD_CTRL) && ( currentEvent.text.text[ 0 ] == 'c' || currentEvent.text.text[ 0 ] == 'C' || currentEvent.text.text[ 0 ] == 'v' || currentEvent.text.text[ 0 ] == 'V' ) ) ){
						if(clearFirstChar){
							clearFirstChar = false;
							slotNameSuggestion.pop_back();
						}
						slotNameSuggestion += currentEvent.text.text;
						clearFirstChar = false;
						renderText = true;
					}
				}
			if(renderText || displayStarted){
				SDL_RenderFillRect(theRenderer, &fillingRect);
				if(slotNameSuggestion != ""){
					SDL_Surface* slotNameSuggestionSurface = TTF_RenderText_Solid(theFont, slotNameSuggestion.c_str(), blackColor);
					SDL_Texture* slotNameSuggestionTexture = SDL_CreateTextureFromSurface(theRenderer, slotNameSuggestionSurface);
					fillingRect.w = slotNameSuggestionSurface->w;
					SDL_RenderFillRect(theRenderer, &fillingRect);
					SDL_RenderCopy(theRenderer,slotNameSuggestionTexture,nullptr,&fillingRect );
					SDL_RenderPresent(theRenderer);
					SDL_DestroyTexture(slotNameSuggestionTexture);
					displayStarted = false;
					SDL_FreeSurface(slotNameSuggestionSurface);}
			}
		}
	}
	SDL_DestroyTexture(mainTextTexture);
	SDL_FreeSurface(mainTextSurface);
	SDL_StopTextInput();
	std::fstream outputStream2(nameStream.str());
	GameSaver theGameSaver(slotNameSuggestion,nameStream.str(),outputStream2,*this);
	theGameSaver.m_saveGame(*this);
	}
	catch(InvalidSelectionElement& ise){
		std::cerr<<"A file has been corrupted at slot with index (starting at 0): "<<ise.what->layer<<std::endl;
	}
}

void GameMain::m_basicInitGame(){
		m_currentRenderer = theRenderer;
		m_initLetterTextures();
		m_initFieldTextures();
		Graphics::Civ::miningTexture = std::make_shared<Texture>(IMG_LoadTexture(theRenderer, "bilder/Landscapes/Mining.png"),STANDARD_FIELD_SIZE/2+1,STANDARD_FIELD_SIZE/2+1);
		//TTF_Init();
}

void GameMain::m_initWindow(){
	theWindow->m_InitWindowSurface();
	someDrawing = theWindow->m_CurrentDrawing();
	m_setCurrentDrawing(someDrawing);
}
