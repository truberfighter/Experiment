/*
 * SelectorSurface.cpp
 *
 *  Created on: 21.06.2022
 *      Author: uwe-w
 */

#include "SelectorSurface.hpp"
#include <algorithm>



SelectorSurface::SelectorSurface(int x, int y, std::vector<std::shared_ptr<SelectionElement>>& whatToAskFor, bool inCenter, SDL_Color standardColor, SDL_Color fontColor, SDL_Color markedColor, std::function<void()> backgroundOrders)
:m_x(x), m_y(y), m_inCenter(inCenter), m_whatToAskFor(whatToAskFor), m_standardColor(standardColor), m_fontColor(fontColor), m_markedColor(markedColor), m_backgroundOrders(backgroundOrders)
{
std::cout<<"SelectorSurface-Konstruktor, m_whatToAskFor-size: "<<m_whatToAskFor.size()<<std::endl;
}

struct TextureWithIndex{
	SDL_Texture* surface;
	int surfaceIndex;
	int w;
	int h;
};

#define DRAW_SELECTION 	std::vector<TextureWithIndex> textSurfaces;\
SDL_SetRenderDrawColor(theRenderer, m_standardColor);\
for(int indexForSurfaceCreation(endIndexes[endIndexesIndex]); indexForSurfaceCreation<NEXT_INDEX(endIndexesIndex,HowMuchToDraw, endIndexes); indexForSurfaceCreation++){\
SelectionElement& currentElement = *m_whatToAskFor.at(indexForSurfaceCreation);\
SDL_Surface* elementSurface = TTF_RenderText_Solid(theFont, currentElement.content.c_str() ,m_fontColor);\
SDL_Texture* elementTexture = SDL_CreateTextureFromSurface(theRenderer, elementSurface);\
textSurfaces.push_back(TextureWithIndex{elementTexture, indexForSurfaceCreation, elementSurface->w, elementSurface->h});\
maxWidth = std::max(maxWidth, elementSurface->w);\
SDL_FreeSurface(elementSurface);\
}\
for(TextureWithIndex& elementTexture: textSurfaces){\
if(elementTexture.surfaceIndex == m_effectiveIndex()){\
	SDL_SetRenderDrawColor(theRenderer, m_markedColor);\
}\
SDL_Rect textRect{currentX,currentY,elementTexture.w,NORMAL_FONT_HEIGHT};\
SDL_RenderFillRect(theRenderer, &textRect);\
SDL_RenderCopy(theRenderer, elementTexture.surface, nullptr, &textRect);SDL_RenderPresent(theRenderer);\
currentY+=textRect.h;\
SDL_DestroyTexture(elementTexture.surface);\
if(elementTexture.surfaceIndex == m_effectiveIndex()){\
	SDL_SetRenderDrawColor(theRenderer, m_standardColor);\
}\
}\
/*if necessary, offer stepping to the next unit (modulo)*/\
if(HowMuchToDraw>elementsPerUnit-1){\
if(currentRelativeIndex == (NEXT_INDEX(endIndexesIndex,HowMuchToDraw - 1,endIndexes)) - endIndexes[endIndexesIndex] + 1){\
	SDL_SetRenderDrawColor(theRenderer, m_markedColor);\
}\
if(currentRelativeIndex==elementsPerUnit){\
	SDL_SetRenderDrawColor(theRenderer, m_markedColor);\
}\
SDL_Surface* moreSurface = TTF_RenderText_Solid(theFont, "MORE", blackColor);\
SDL_Texture* elementTexture = SDL_CreateTextureFromSurface(theRenderer, moreSurface);\
SDL_Rect textRect{currentX,currentY,moreSurface->w,NORMAL_FONT_HEIGHT};\
SDL_RenderFillRect(theRenderer, &textRect);\
SDL_RenderCopy(theRenderer, elementTexture, nullptr, &textRect);\
SDL_DestroyTexture(elementTexture);SDL_FreeSurface(moreSurface);\
}\
if(currentRelativeIndex == (NEXT_INDEX(endIndexesIndex,HowMuchToDraw - 1,endIndexes)) - endIndexes[endIndexesIndex]){\
	SDL_SetRenderDrawColor(theRenderer, m_standardColor);\
}\
SDL_RenderPresent(theRenderer);\
currentX = m_x; currentY = m_y;

#ifdef NEXT_INDEX
#undef NEXT_INDEX
#endif
#define NEXT_INDEX(INDEX, MAXIMUM, CONTAINER) (INDEX < CONTAINER.size()-1 ? \
CONTAINER.at(INDEX + 1) : MAXIMUM)

SelectionReturn SelectorSurface::m_fetchSelection(){
	std::sort(m_whatToAskFor.begin(),m_whatToAskFor.end(), [](std::shared_ptr<SelectionElement>& s1, std::shared_ptr<SelectionElement>& s2)->bool{return s1->layer<=s2->layer;});
	SDL_SetRenderDrawColor(theRenderer, m_standardColor);
	HowMuchToDraw = m_whatToAskFor.size();
	int currentX(m_x), currentY(m_y);
	int lastLayerEndIndex = 0;
	int lastLayer = 0;
	for(int explorationIndex(0); explorationIndex < HowMuchToDraw; explorationIndex++){
		std::shared_ptr<SelectionElement>& currentElement = m_whatToAskFor[explorationIndex];
		if(lastLayer!=currentElement->layer){
			lastLayer = currentElement->layer;
			lastLayerEndIndex = explorationIndex;
			maxHeightNormed = std::max(maxHeightNormed, explorationIndex - endIndexes.back());
		}
		if(explorationIndex - endIndexes.back() >= elementsPerUnit){
			if(lastLayerEndIndex == endIndexes.back()){
				endIndexes.push_back(explorationIndex);
				lastLayerEndIndex = explorationIndex;
			}
			else{
				endIndexes.push_back(lastLayerEndIndex);std::cout<<"pushed: exploratipnIndex = "<<explorationIndex<<", pushed: "<<endIndexes.back()<<std::endl;
			}
			maxHeightNormed = explorationIndex - endIndexes.back();
		}
	}
	for(int ff: endIndexes){
		std::cout<<"endIndex: "<<ff<<std::endl;
	}
	endIndexesIndex = 0;
	DRAW_SELECTION
	//start event loop
	bool quit = false;
	SDL_Event event;
	while(!quit){
		while(SDL_PollEvent(&event)){
			try{
				if(event.type == SDL_QUIT){
					throw SDLQuitException();
				}
				if(m_handleEvent(event)){
					DRAW_SELECTION
					continue;
				}
			}
			catch(QuitSelection& qs){
				if(qs.m_returnSomething == NO_ACTION){
					throw qs;
				}
				else if(qs.m_returnSomething == SELECTION){
					return SelectionReturn{m_whatToAskFor[m_effectiveIndex()]->content,(unsigned int) m_effectiveIndex()};
				}
				else if(qs.m_returnSomething == NEXT_PAGE){
					SDL_RenderClear(theRenderer);
					endIndexesIndex++;
//never set value to the very last endIndex (which is just the size of m_whatToAsk)
					std::cout<<"more has been pressed and it is up to be drawn been drawn: endIndexesIndex = "<<endIndexesIndex<<std::endl;
					endIndexesIndex = modulo(endIndexesIndex, endIndexes.size());
					currentRelativeIndex = 0;
					DRAW_SELECTION
					for(int iii: endIndexes){
						std::cout<<"endIndex "<<iii<<std::endl;
					}
				}
			}
			catch(SDLQuitException& sdlqe){
				throw sdlqe;
			}
		}
	}
}

bool SelectorSurface::m_handleEvent(const SDL_Event& event){
	try{
	switch(event.type){
	case SDL_MOUSEBUTTONDOWN:
	{
		std::cout<<"mousebuttondown"<<std::endl;
		if(event.button.button == SDL_BUTTON_LEFT){
			return m_handleLeftClick(event.button);
		}
		if(event.button.button == SDL_BUTTON_RIGHT){
			m_select(event.button.x, event.button.y);
			m_whatToAskFor[m_effectiveIndex()]->rightClickOrders();
			m_backgroundOrders();
			return true;
		}
		return false;
	}
	case SDL_KEYDOWN:
	{
		std::cout<<"keydown"<<std::endl;
		return m_handleKeyboardEvent(event);
	}
	default:
		return false;
	}
	}
	catch(QuitSelection& qs){
		throw qs;
	}
}

int SelectorSurface::m_effectiveIndex(){
	return endIndexes[endIndexesIndex] + currentRelativeIndex;
}

bool SelectorSurface::m_handleLeftClick(const SDL_MouseButtonEvent& event){
	m_select(event.x, event.y);
	throw m_finalSelection();
}

bool SelectorSurface::m_select(int x, int y){
	int relativeY = y-m_y;
	if(relativeY<0){
		return false;
	}
	int normedY = relativeY/NORMAL_FONT_HEIGHT;
	if(NEXT_INDEX(endIndexesIndex, (int)m_whatToAskFor.size()-1,endIndexes)-(int)endIndexes[endIndexesIndex]<normedY){
		return false;
	}
	currentRelativeIndex = normedY;
	return true;
}


bool SelectorSurface::m_handleKeyboardEvent(const SDL_Event& event){
	if(event.type!=SDL_KEYDOWN){
		return false;
	}
	KeyCode keyCode = event.key.keysym.sym;
	std::cout<<"Man hat den KeyCode "<<keyCode<<", effective index: "<<m_effectiveIndex()<<std::endl;
	switch(keyCode){
	case SDLK_ENTER_KEY:{
		enterkey: if(m_effectiveIndex()<0||(m_effectiveIndex()>HowMuchToDraw + 1 || (m_effectiveIndex()==HowMuchToDraw && HowMuchToDraw<=elementsPerUnit))){
			throw QuitSelection(NO_ACTION);
		}
		throw m_finalSelection();
	}
	case SDLK_SPACE:
	{
		goto enterkey;
	}
	case SDLK_UP:
	{
		m_goUp();
		return true;
	}
	case SDLK_8_UP:
	{
		m_goUp();
		return true;
	}
	case SDLK_DOWN:
	{
		m_goDown();
		return true;
	}
	case SDLK_2_DOWN:
	{
		m_goDown();
		return true;
	}
	case SDLK_ESCAPE:
	{
		throw QuitSelection(NO_ACTION);
	}
	default:
		return false;
	}
}

#define MODULO_REFERENCE (NEXT_INDEX(endIndexesIndex,HowMuchToDraw,endIndexes)\
 + (m_whatToAskFor.size()-2 >= elementsPerUnit ? 1 : 0))

void SelectorSurface::m_goUp(){
	currentRelativeIndex = modulo(currentRelativeIndex - 1, MODULO_REFERENCE - endIndexes[endIndexesIndex]);
	std::cout<<"goUp: currentRelativeIndex = "<<currentRelativeIndex<<std::endl;
}

void SelectorSurface::m_goDown(){
	currentRelativeIndex = modulo(currentRelativeIndex + 1, MODULO_REFERENCE - endIndexes[endIndexesIndex]);
}

#undef MODULO_REFERENCE

QuitSelection SelectorSurface::m_finalSelection(){
	if(m_effectiveIndex()==HowMuchToDraw){
		return QuitSelection(NEXT_PAGE);
	}
	if(m_effectiveIndex() == NEXT_INDEX(endIndexesIndex,HowMuchToDraw + 1,endIndexes)){
	std::cout<<"quitselection"<<m_effectiveIndex()<<std::endl;
		return QuitSelection(NEXT_PAGE);
	}
	else return QuitSelection(SELECTION);
}
