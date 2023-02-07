/*
 * AppInitter.cpp
 *
 *  Created on: 03.02.2023
 *      Author: uwe-w
 */
#include "AppInitter.hpp"
#include <map>

AppInitter::AppInitter()
{

}

void AppInitter::m_init(){
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0 ){
	  std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
	  return;
	}
	int imgflags = IMG_INIT_PNG;
	IMG_Init(imgflags);
	TTF_Init();
	theFont = TTF_OpenFont("Fonts/FT88-Regular.ttf", 22);
	citySizeFont = TTF_OpenFont("Fonts/FT88-Regular.ttf",35);
	theWindow = new Window ("A Civilization clone by truberfighter", SCREEN_WIDTH, SCREEN_HEIGHT);
	theRenderer = theWindow->m_Renderer();
}

StartingMode AppInitter::m_requestStartingMode(){
	std::vector<std::shared_ptr<SelectionElement>> options;
	std::function<void()> dn = [](){};
	std::shared_ptr<SelectionElement> load = std::make_shared<SelectionElement>("Load a saved game",1,dn);
	std::shared_ptr<SelectionElement> newGame = std::make_shared<SelectionElement>("Start a new game",0,dn);
	options.push_back(newGame);
	if(m_isAnythingLoadable())
		options.push_back(load);
	SelectorSurface startSelection(0,0,options);
	for(auto& a: options){
		std::cout<<a->content<<std::endl;
	}
	std::map<int,StartingMode> modes = {{0, START_A_NEW_GAME}, {1, LOAD_GAME}};
	int INDEX = startSelection.m_fetchSelection().index;
	return modes[INDEX];
}

bool AppInitter::m_isAnythingLoadable(){
	for(int slotIndex(0);slotIndex<POSSIBLE_SAVE_FILES;slotIndex++){
		std::stringstream stream;
		stream<<"data\\gameData"<<slotIndex<<".txt"; stream.flush();
		std::fstream test(stream.str());
		if(static_cast<bool>(test)){
			return true;
		}
	}
	return false;
}
