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
SDL_Color whiteColor{250, 250, 250};

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

int drawSquareLines(SDL_Renderer* renderer, int x, int y, SDL_Color color){
	if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)!=0) std::cout<<"SDL_Error in DrawSquareLines: \n"<<SDL_GetError()<<std::endl;
	return SDL_RenderDrawLine(renderer, x,y, x+STANDARD_FIELD_SIZE,y) +
	SDL_RenderDrawLine(renderer, x,y+STANDARD_FIELD_SIZE, x+STANDARD_FIELD_SIZE,y+STANDARD_FIELD_SIZE) +
	SDL_RenderDrawLine(renderer, x,y,x,y+STANDARD_FIELD_SIZE) +
	SDL_RenderDrawLine(renderer, x+STANDARD_FIELD_SIZE, y,x+STANDARD_FIELD_SIZE,y+STANDARD_FIELD_SIZE);
}
