/*
 * ButtonElement.cpp
 *
 *  Created on: 27.06.2022
 *      Author: uwe-w
 */

#include "ButtonElement.hpp"


ButtonElement::	ButtonElement(int row, int column, unsigned int width, unsigned int height, SDL_Color backgroundColor, SDL_Color fontColor, std::string name, SDL_Renderer* renderer)
:m_width(width), m_height(height), m_backgroundColor(backgroundColor), m_fontColor(fontColor), m_name(name), DrawingElement(row, column, renderer)
{
}

ButtonElement::~ButtonElement() {
	std::cout<<"ButtonElement-Destruktor, this = "<<this<<std::endl;
}

bool ButtonElement::m_isClicked(unsigned int x, unsigned int y){
	if(m_row > x ){
		return false;
	}
	if(m_column>y){
		return false;
	}
	if(m_row+m_width<x){
		return false;
	}
	if(m_column+m_height<y){
		return false;
	}
	return true;
}

Drawing_Element ButtonElement::m_DrawingElement(){
	return BUTTON_ELEMENT;
}

bool ButtonElement::m_equals(DrawingElement& comparedDrEl){
	return this==&comparedDrEl;
}

int ButtonElement::m_draw(int rowShift, int columnShift, SDL_Renderer* renderer){
	SDL_Rect rectToFill{(int) rowShift + m_row, (int)columnShift + m_column, (int)m_width, (int)m_height};
	SDL_SetRenderDrawColor(theRenderer, m_backgroundColor);
	SDL_RenderFillRect(theRenderer, &rectToFill);
	SDL_Surface* textSurface = TTF_RenderText_Solid(theFont, m_name.c_str(), m_fontColor);
	SDL_Rect textRect{rectToFill.x + ((int)m_width - textSurface->w)/2, rectToFill.y + ((int)m_height - textSurface->h)/2, textSurface->w, textSurface->h};
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(theRenderer, textSurface);
	SDL_RenderCopy(theRenderer, textTexture, nullptr, &textRect);
	SDL_DestroyTexture(textTexture);
	SDL_FreeSurface(textSurface);
	return 0;
}

bool ButtonElement::m_updatePosition(){
	return true;
}


