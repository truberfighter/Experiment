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

class GameMain: public EventHandler{
private:
	std::vector<std::unique_ptr<Window>> m_theWindows;
	//std::vector<std::unique_ptr<Window>>::iterator m_currentWindow;
	unsigned int m_currentWindowIndex = 0;
public:
	GameMain();
	~GameMain();
	void m_initGame();
};

extern SDL_Renderer *theRenderer;
extern SDL_Texture *theTexture;
extern SDL_Texture *theTexture;
extern SDL_Event currentEvent;
extern GameMain* theEventHandler;
extern std::shared_ptr<Drawing> someDrawing;



#endif /* GAMEMAIN_HPP_ */
