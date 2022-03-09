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

int Texture::m_Width(){std::cout<<"Width aufgerufen"<<std::endl;return m_width;}
int Texture::m_Height(){ std::cout<<"Height aufgerufen"<<std::endl;return m_height;}
SDL_Texture* Texture::theTexture(){return m_theTexture;}

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
	return modulo(i, WORLD_LENGTH*STANDARD_FIELD_SIZE);
}
int yModulo(const int& i){
	return modulo(i, WORLD_HEIGHT*STANDARD_FIELD_SIZE);
}

Texture::Texture(SDL_Texture* texture, int width, int height)
: m_theTexture(texture), m_width(width), m_height(height)
{

}

Texture::~Texture(){
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
