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
protected:
	bool m_handleLeftClick(const SDL_MouseButtonEvent& currentEvent);
	int m_drawMainDrawing();
	Coordinate m_topLeftCorner = Coordinate(0,0);
	std::shared_ptr<Figure> m_whatToMove;
private:
	void doSomething();
	std::unique_ptr<Game> m_theGame;
	std::vector<std::unique_ptr<Window>> m_theWindows;
	//std::vector<std::unique_ptr<Window>>::iterator m_currentWindow;
	unsigned int m_currentWindowIndex = 0;
	std::shared_ptr<Drawing> m_currentFigureInfo;
	BlinkingState m_currentBlinkingState = VISIBLE();
	BlinkingTime m_blinkingIntervalTime = STANDARD_BLINKING_INTERVAL_TIME;
public:
	void _setWhatToMove(std::shared_ptr<Figure> whatToMove);
	bool m_handleEvent(const SDL_Event& event) override;
	bool m_handleKeyboardEvent(const SDL_Event& event) override;
	void m_showFigureInfo();
	void m_initInfoDrawing();
	void lol(){this->EventHandler::lol(); std::cout<<"GameMain::lol()"<<std::endl;}
	GameMain();
	void m_initLetterTextures();
	SDL_Renderer* m_currentRenderer;
	virtual ~GameMain();
	int operator()();
	void m_initGame();
	void m_showFigureInformation(std::shared_ptr<Figure> whatFigure);
	void m_createFieldTexture(Landscape ls, std::string filename);
	void m_initFieldTextures();
	void gameMainDebug(std::list<SDL_Event>& eventList);
	std::unique_ptr<Window>& m_currentWindow(){return m_theWindows[m_currentWindowIndex];}
	void m_makeBlinkingStep();
	void m_setWhatToMove(std::shared_ptr<Figure> whatToMove);
	bool m_deleteDrawing(Drawing* drawingToDelete);
	bool m_addDrawing(Drawing* drawingToAdd);
	void m_draw();
};

extern SDL_Texture *theTexture;
extern SDL_Event currentEvent;
extern GameMain* theEventHandler;
extern std::shared_ptr<Drawing> someDrawing;


#endif /* GAMEMAIN_HPP_ */
