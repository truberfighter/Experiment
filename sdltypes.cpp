/*
 * sdtypes.cpp
 *
 *  Created on: 14.02.2022
 *      Author: uwe-w
 */
#include "sdltypes.hpp"
#include "SDL2/SDL.h"
#include <iostream>
#include <SDL2\SDL_image.h>

int Texture::m_Width(){return m_width;}
int Texture::m_Height(){ return m_height;}
SDL_Texture* Texture::theTexture(){return m_theTexture;}
MovementPoints MOVE_PROHIBITED = -2;
MovementPoints FIGHT_IS_COMING = -1;
TTF_Font* theFont = nullptr;
Figure* figureToDraw = nullptr;
Field* fieldToDraw = nullptr;
SDL_Color whiteColor{25, 25, 250};
SDL_Color blackColor{0,0,0};
SDL_Color brownColor{130,80,0};

int modulo (const int& i, const int& j){
	if(j <= 0)
		return MODULO_FAIL;
	if(i>=0)
		return i % j;
	if(i<0){
		int inversemodulo = (-i)%j;
		inversemodulo = (j-inversemodulo)%j;
		return inversemodulo;
	}
}

int xModulo(const int& i){
	return modulo(i  + STANDARD_FIELD_SIZE - MODULO_WIDTH_FOR_NEGATIVE, WORLD_LENGTH*STANDARD_FIELD_SIZE) - STANDARD_FIELD_SIZE + MODULO_WIDTH_FOR_NEGATIVE;
}
int yModulo(const int& i){
	return modulo(i + STANDARD_FIELD_SIZE - MODULO_HEIGHT_FOR_NEGATIVE, WORLD_HEIGHT*STANDARD_FIELD_SIZE) - STANDARD_FIELD_SIZE + MODULO_HEIGHT_FOR_NEGATIVE;
}

Texture::Texture(SDL_Texture* texture, int width, int height)
: m_theTexture(texture), m_width(width), m_height(height)
{
std::cout<<"Texture-Konstruktor, this ="<<this<<", m_height = "<<m_height<<", m_width = "<<m_width<<std::endl;
}

Texture::~Texture(){
	std::cout<<"Texture-Destruktor, this = "<<this<<std::endl;
	SDL_DestroyTexture(m_theTexture);
}

Coordinate::Coordinate(int a, int b)
    : x(a), y(b) {}

bool Coordinate::operator==(const Coordinate& other){
    if(other.x != this->x || other.y != this->y)
        return false;
    else
        return true;
}


bool Coordinate::operator!=(const Coordinate& other){
    if(*this == other)
        return false;
    else
       return true;
}

int Graphics::drawSquareLines(SDL_Renderer* renderer, int x, int y, SDL_Color color){
	if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)!=0) std::cout<<"SDL_Error in DrawSquareLines: \n"<<SDL_GetError()<<std::endl;
	int whatToReturn = 0;
	whatToReturn+=Graphics::drawThickerVerticalLine(renderer, x,y);
	whatToReturn+=Graphics::drawThickerVerticalLine(renderer, x+STANDARD_FIELD_SIZE-STANDARD_LINE_THICKNESS,y);
	whatToReturn+=Graphics::drawThickerHorizontalLine(renderer, x,y);
	whatToReturn+=Graphics::drawThickerHorizontalLine(renderer, x,y+STANDARD_FIELD_SIZE-STANDARD_LINE_THICKNESS);
	return whatToReturn;
}

int Graphics::drawSquareStarLines(SDL_Renderer* renderer, int x, int y, SDL_Color color){
	if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)!=0) std::cout<<"SDL_Error in DrawSquareLines: \n"<<SDL_GetError()<<std::endl;
	int whatToReturn = 0;
		whatToReturn+=Graphics::drawThickerVerticalLine(renderer, x+STANDARD_FIELD_SIZE/2-STANDARD_LINE_THICKNESS/2,y);
		whatToReturn+=Graphics::drawThickerDiagonalLineDown(renderer, x,y);
		whatToReturn+=Graphics::drawThickerHorizontalLine(renderer, x,y+STANDARD_FIELD_SIZE/2-STANDARD_LINE_THICKNESS/2);
		whatToReturn+=Graphics::drawThickerDiagonalLineUp(renderer, x,y);
		return whatToReturn;
}

int Graphics:: drawThickerDiagonalLineDown(SDL_Renderer* renderer, int x, int y, int thickness){
	if(thickness <=0) return 0;
	int whatToReturn = 0;
	whatToReturn+=SDL_RenderDrawLine(renderer, x,y, x+STANDARD_FIELD_SIZE,y+STANDARD_FIELD_SIZE);
	int thicknessReached(1);
	for(;thicknessReached < thickness; thicknessReached++){
		switch(thicknessReached%2){
		case 0:{
			whatToReturn+=SDL_RenderDrawLine(renderer, x+thicknessReached/2,y, x+STANDARD_FIELD_SIZE,y+STANDARD_FIELD_SIZE-thicknessReached/2);
			break;
		}
		case 1: {
			thicknessReached++;
			whatToReturn+=SDL_RenderDrawLine(renderer, x,y+thicknessReached/2, x+STANDARD_FIELD_SIZE-thicknessReached/2,y+STANDARD_FIELD_SIZE);
			thicknessReached--;
		}
	}
	}
	if(whatToReturn!=0) std::cout<<"SDL Error: \n"<<SDL_GetError()<<std::endl;
	return whatToReturn;
}

int Graphics:: drawThickerDiagonalLineUp(SDL_Renderer* renderer, int x, int y, int thickness){
	if(thickness <=0) return 0;
	int whatToReturn = 0;
	whatToReturn+=SDL_RenderDrawLine(renderer, x+STANDARD_FIELD_SIZE,y, x,y+STANDARD_FIELD_SIZE);
	int thicknessReached(1);
	for(;thicknessReached < thickness; thicknessReached++){
		switch(thicknessReached%2){
		case 0:{
			whatToReturn+=SDL_RenderDrawLine(renderer, x+STANDARD_FIELD_SIZE-thicknessReached/2,y, x,y+STANDARD_FIELD_SIZE-thicknessReached/2);
			break;
		}
		case 1: {
			thicknessReached++;
			whatToReturn+=SDL_RenderDrawLine(renderer, x+STANDARD_FIELD_SIZE,y+thicknessReached/2, x+thicknessReached/2,y+STANDARD_FIELD_SIZE);
			thicknessReached--;
		}
	}
	}
	if(whatToReturn!=0) std::cout<<"SDL Error: \n"<<SDL_GetError()<<std::endl;
	return whatToReturn;
}

int Graphics:: drawThickerHorizontalLine(SDL_Renderer* renderer, int x, int y, int thickness){
	if(thickness <=0) return 0;
	int whatToReturn = 0;
	int thicknessReached(0);
	for(;thicknessReached < thickness; thicknessReached++){
		whatToReturn+=SDL_RenderDrawLine(renderer, x,y+thicknessReached, x+STANDARD_FIELD_SIZE,y+thicknessReached);
	}
	if(whatToReturn!=0) std::cout<<"SDL Error: \n"<<SDL_GetError()<<std::endl;
	return whatToReturn;
}

int Graphics:: drawThickerVerticalLine(SDL_Renderer* renderer, int x, int y, int thickness){
	if(thickness <=0) return 0;
	int whatToReturn = 0;
	int thicknessReached(0);
	for(;thicknessReached < thickness; thicknessReached++){
		whatToReturn+=SDL_RenderDrawLine(renderer, x+thicknessReached, y, x+thicknessReached, y+STANDARD_FIELD_SIZE);
	}
	if(whatToReturn!=0) std::cout<<"SDL Error: \n"<<SDL_GetError()<<std::endl;
	return whatToReturn;
}
