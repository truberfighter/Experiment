/*
 * GameMain.hpp
 *
 *  Created on: 04.03.2022
 *      Author: uwe-w
 */

#ifndef GAMEMAIN_HPP_
#define GAMEMAIN_HPP_

#include "EventHandler.hpp"
#include <iostream>
#include "sdltypes.hpp"
#include "EventHandler.hpp"
#include <string>
#include "Game.hpp"

class GameMain: public EventHandler{
private:
	void doSomething();
	std::unique_ptr<Game> m_theGame;
	std::vector<std::unique_ptr<Window>> m_theWindows;
	//std::vector<std::unique_ptr<Window>>::iterator m_currentWindow;
	unsigned int m_currentWindowIndex = 0;
public:
	void lol(){this->EventHandler::lol(); std::cout<<"GameMain::lol()"<<std::endl;}
	GameMain();
	void m_initLetterTextures();
	SDL_Renderer* m_currentRenderer;
	~GameMain();
	int operator()();
	void m_initGame();
	void m_createFieldTexture(Landscape ls, std::string filename);
	void m_initFieldTextures();
	void gameMainDebug(std::list<SDL_Event>& eventList);
	std::unique_ptr<Window>& m_currentWindow(){return m_theWindows[m_currentWindowIndex];}
};

extern SDL_Renderer *theRenderer;
extern SDL_Texture *theTexture;
extern SDL_Texture *theTexture;
extern SDL_Event currentEvent;
extern GameMain* theEventHandler;
extern std::shared_ptr<Drawing> someDrawing;


#endif /* GAMEMAIN_HPP_ */
