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

int Texture::Width(){return m_width;}
int Texture::Height(){return m_height;}
SDL_Texture* Texture::theTexture(){return m_theTexture;}

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
