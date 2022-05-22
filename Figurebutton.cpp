/*
 * Figurebutton.cpp
 *
 *  Created on: 21.05.2022
 *      Author: uwe-w
 */

#include "Figurebutton.hpp"
#include "Figure.hpp"
#include "City.hpp"
#include "Game.hpp"
#include "GameMain.hpp"
#include "math.h"
Figurebutton::Figurebutton(std::list<std::shared_ptr<Figure>> figureList):m_figureList(figureList)
{

}

std::shared_ptr<Figure> Figurebutton::operator()(){
	std::shared_ptr<Figure> nulll;
	nulll = nullptr;
	SDL_Color backgroundColor = infoTextBackgroundColor;
	SDL_SetRenderDrawColor(theRenderer, backgroundColor);
	int size = std::min((int)m_figureList.size(), SCREEN_HEIGHT/FIGURE_BUTTON_HEIGHT);
	int yToDraw = SCREEN_HEIGHT /2 - FIGURE_BUTTON_HEIGHT /2;
	yToDraw -= FIGURE_BUTTON_HEIGHT*size;
	yToDraw = std::max(yToDraw,0);
	struct Subbutton{
		std::shared_ptr<Figure> figure;
		int yToDraw;
	};
	std::vector<Subbutton> subbuttons;
	int nr(0);
	SDL_Rect currentRectToDraw{FIGURE_INFO_WIDTH,yToDraw,FIGURE_BUTTON_WIDTH,FIGURE_BUTTON_HEIGHT};
	SDL_Event e;
	nr = 0;
	bool quit = false;
	for(std::shared_ptr<Figure> currentFigure: m_figureList){
		subbuttons.push_back(Subbutton{currentFigure, nr*FIGURE_BUTTON_HEIGHT + yToDraw});
		nr++;
		SDL_RenderFillRect(theRenderer, &currentRectToDraw);
		currentFigure->m_drawFigureSomewhere(FIGURE_INFO_WIDTH, currentRectToDraw.y);
		currentRectToDraw.y += FIGURE_BUTTON_HEIGHT;
	}
	nr = 0;
	SDL_RenderPresent(theRenderer);while(!quit){
		while(SDL_PollEvent(&e)){
			if(e.type==SDL_QUIT){
				throw(SDLQuitException());
			}
			if(e.type == SDL_MOUSEBUTTONDOWN){
				if(e.button.button==SDL_BUTTON_LEFT){
					std::cout<<"figurebutton phase, mouseclick: x = "<<e.button.x<<", y = "<<e.button.y<<std::endl;
					if(size == 1){
						if(!mouseClickedOnSubbutton(subbuttons.front().figure)){
							return nulll;
						}
							quit = true;
					}
					if(e.button.x >= FIGURE_INFO_WIDTH && e.button.x <= FIGURE_INFO_WIDTH + FIGURE_BUTTON_WIDTH){
						if(e.button.y<yToDraw || e.button.y >= yToDraw + size*FIGURE_BUTTON_HEIGHT){
							quit = true;
						}
						else{
							if(mouseClickedOnSubbutton(subbuttons[(e.button.y - yToDraw)/FIGURE_BUTTON_HEIGHT].figure)){
								nr = (e.button.y - yToDraw)/FIGURE_BUTTON_HEIGHT;
							}
						}
					}
					else{
						quit = true;
					}
					std::cout<<"for to entry"<<std::endl;
					for(Subbutton& currentSubbutton: subbuttons){
						currentSubbutton.figure->m_drawFigureSomewhere(currentRectToDraw.x,currentSubbutton.yToDraw);
					}
					SDL_RenderPresent(theRenderer);
					std::cout<<"nr = "<<nr<<", e.button.y - yToDraw = "<<e.button.y - yToDraw <<std::endl;
				}
			}
		}
	}
	std::cout<<"nr = "<<nr<<", size = "<<size<<std::endl;
	for(std::shared_ptr<Figure> comparedFigure: theGame->m_NationAtCurrentTurn()->m_activeFigures){
		std::cout<<"Z.83"<<std::endl;
		if(subbuttons[nr].figure.get()==comparedFigure.get()){
std::cout<<"Z.85"<<std::endl;
			return comparedFigure;
		}
	}
	std::cout<<"nullptr-return"<<std::endl;
	return nulll;
}

bool Figurebutton::mouseClickedOnSubbutton(std::shared_ptr<Figure> figureSelected){
	std::cout<<"figureSelected: "<<figureSelected->m_figureOverview()<<", figureState: "<<figureSelected->m_figureState<<std::endl;
	switch(figureSelected->m_FigureState()){
	case MOVING:
		return true;
	case SENTRYING:{
		figureSelected->m_figureState = DONE_WITH_TURN;
		return false;
	}
	case SENTRIED:
	{
		figureSelected->m_startMove(true);
		figureSelected->m_Nation()->m_addToQueue(figureSelected);
		return true;
	}
	case DONE_WITH_TURN:
		return false;
	case FORTIFYING:{
		figureSelected->m_figureState = DONE_WITH_TURN;
		return false;
	}
	case FORTIFIED:{
		figureSelected->m_figureState = DONE_WITH_TURN;
		return false;
	}
	case COMPLETELY_FORTIFIED:{
		figureSelected->m_figureState = SENTRIED;
		figureSelected->m_startMove(true);
		figureSelected->m_Nation()->m_addToQueue(figureSelected);
		return true;
	}
	case SETTLERS_AT_WORK:{
		(reinterpret_cast<Settlers*>(figureSelected.get()))->m_currentWork = NONE;
		figureSelected->m_figureState = DONE_WITH_TURN;
		return false;
	}
	case PILLAGE_IN_PROGRESS:
	{
		return false;
	}
	default:
		return false;
	}
}
