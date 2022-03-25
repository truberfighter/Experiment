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
	TTF_Init();theFont = TTF_OpenFont("Fonts/FT88-Regular.ttf", 15);
	fieldTextures = new shared_ptr<Texture>[30];
	Window* theWindow = new Window ("Game Main, Window 0", SCREEN_WIDTH, SCREEN_HEIGHT);
	m_currentRenderer = theRenderer = theWindow->m_Renderer();
	m_initLetterTextures();
	m_initFieldTextures();
	initFieldContainer();
	FieldContainer::getTheContainer()->initContinentIDs();
	m_theWindows.push_back(unique_ptr<Window>());
	m_theWindows[0].reset(theWindow);
	//TTF_Init();
	cout<<"Kevin"<<endl;
	theWindow->m_InitWindowSurface();
	someDrawing = theWindow->m_CurrentDrawing();
	//someDrawing->m_draw();
    MovableThing* theMovableThing2 = new MovableThing(m_currentRenderer, 90, 90, "bilder/Landscapes/Grassland.png", 280, 220, true);
    someDrawing->m_add(theMovableThing2);
	//SDL_RenderPresent(m_currentRenderer);
	//SDL_Delay(2000);
	cout<<"m_currentRenderer: "<<m_currentRenderer<<endl;

}

void GameMain::m_createFieldTexture(Landscape ls, string filename){
	theTexture = IMG_LoadTexture(m_currentRenderer, filename.c_str());
	fieldTextures[ls]=make_shared<Texture>(theTexture, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE);
}

void GameMain::m_initFieldTextures(){
	m_createFieldTexture(GRASSLAND, "bilder/Landscapes/Grassland.png");
	cout<<"SDL_Error: "<<SDL_GetError()<<endl;
	m_createFieldTexture(PLAINS, "bilder/Landscapes/Plains.png");
}

GameMain::~GameMain(){
	delete fieldTextures;
	TTF_CloseFont(theFont);
	delete theLetterTextureContainer;
}

int GameMain::operator()(){
 if( SDL_Init( SDL_INIT_VIDEO ) < 0 ){
  std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
 }else{
/*	 if( TTF_Init() == -1 )
	                 {
	                     printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
	                 }*/

   Window* theWindow=&(*m_currentWindow());
   if(theWindow)cout<<"Window existiert"<<endl;

  //theRenderer = theWindow->m_Renderer();
  FieldContainer& fc = *theContainer;
  cout<<"guenther"<<endl;

  //someDrawing = /*make_shared<Drawing>(theWindow.m_Renderer());*/ theWindow->m_CurrentDrawing();
  //if(theRenderer==NULL)cout<<"Guenther"<<endl;
  /*if( window == NULL ){
   std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
  }else{*/
   /*screenSurface = SDL_GetWindowSurface( window ); //Get window surface
   SDL_FillRect( screenSurface, NULL
   , SDL_MapRGB( screenSurface->format, 0x00, 0xFF, 0xFF ) );
   //Fill the surface with color
   SDL_UpdateWindowSurface( window ); //Update the surface*/
  cout<<"kevinkevinkevin"<<endl;
  SDL_RenderPresent(m_currentRenderer);
theTexture = IMG_LoadTexture(m_currentRenderer, "bilder/2022-01-03.png");

  SDL_Rect someColoredRect;
  someColoredRect.x = 34;
  someColoredRect.y = 20;
  someColoredRect.h = STANDARD_FIELD_SIZE;
  someColoredRect.w = STANDARD_FIELD_SIZE;
	cout<<"m_currentRenderer: "<<m_currentRenderer<<endl;
  SDL_SetRenderDrawColor(m_currentRenderer, 240, 240, 240, 255);
  SDL_RenderFillRect(m_currentRenderer, &someColoredRect);
  SDL_RenderPresent(m_currentRenderer);
  //SDL_Delay(2000);
  //SDL_DestroyTexture(noOneLikesU);
  //SDL_RenderPresent(theRenderer);
  std::shared_ptr<Texture> texture =std::make_shared <Texture>(theTexture,someColoredRect.w, someColoredRect.h);
  MovableThing* theMovableThing = new MovableThing(m_currentRenderer, texture, 45, 45, true);
  cout<<"theMovableThing erfolgreich initialisiert"<<endl;
  cout<<"m_currentRenderer: "<<m_currentRenderer<<endl;

  //MovableDrawingElement mel(theRenderer, theMovableThing);
  if(!someDrawing) cout<<"lol"<<endl;
  someDrawing->m_add(theMovableThing);cout<<"line 91"<<endl;
  //SDL_Delay(3000);
    //theMovableThing->m_drawRight();
    SDL_Delay(400);
    MovableThing* theMovableThing2 = new MovableThing(m_currentRenderer, 90, 90, "bilder/Landscapes/Grassland.png", 280, 220, true);;
    //auto a = new MovableDrawingElement(theRenderer, theMovableThing2);
    //cout<<"theMovableThing2 erfolgreich initialisiert"<<endl;
    someDrawing->m_add(theMovableThing2);
    theRenderer = m_currentRenderer;
    SDL_RenderPresent(m_currentRenderer);
    SDL_Delay(STANDARD_DRAWING_TIME*3);
    shared_ptr<Nation> theRomans = make_shared<Nation>(ROMAN);
    shared_ptr<Field>& theField = ((*theContainer->m_getFieldsOfTheWorld())[0][3]);
    cout<<theRenderer;
    shared_ptr<Settlers> myFirstSettlers = std::make_shared<Settlers>(theField, theRomans);
    m_setWhatToMove(myFirstSettlers);
myFirstSettlers->m_integrateInto(*someDrawing);
m_setCurrentDrawing(someDrawing);
    cout<<theLetterTextureContainer<<endl;
    shared_ptr<DrawingElement> drElShPo = make_shared<ImmovableDrawingElement>(theRenderer, theLetterTextureContainer->m_getLetterTexture('F'), 5*STANDARD_FIELD_SIZE, 10*STANDARD_FIELD_SIZE,  STANDARD_FIELD_MODIFICATOR_LAYER);
    cout<<drElShPo<<endl;
    someDrawing->m_add(drElShPo);
    someDrawing->m_draw();
    SDL_RenderPresent(m_currentRenderer);

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
  theMovableThing2->m_setMoveToDirection(UP_RIGHT);
  theMovableThing2->m_move();
  shared_ptr<DrawingElement> st =make_shared<LambdaDrawingElement>(theRenderer, [](int a, int b, SDL_Renderer* renderer){int h = SDL_RenderDrawLine(renderer, a, b, a+120, b+120); cout<<"h = "<<h<<endl; return h;},20,20);
  someDrawing->m_add(st);
  someDrawing->m_draw();
  SDL_Delay( 2000);
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
