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


unsigned int settlersCount = 0;
int Texture::m_Width(){return m_width;}
int Texture::m_Height(){ return m_height;}
SDL_Texture* Texture::theTexture(){return m_theTexture;}
MovementPoints MOVE_PROHIBITED = -2;
MovementPoints FIGHT_IS_COMING = -1;
TTF_Font* theFont = nullptr; TTF_Font* citySizeFont = nullptr;
Figure* figureToDraw = nullptr;
Field* fieldToDraw = nullptr;
SDL_Color whiteColor{250, 250, 250};
SDL_Color blackColor{0,0,0};
SDL_Color brownColor{130,80,0};
SDL_Color infoTextColor{0,0,0};
SDL_Color infoTextBackgroundColor{125,125,140};
SDL_Color Graphics::Civ::cityNameColor(){return SDL_Color{30,200,200};}
SDL_Color Graphics::Civ::cityOccupiedColor(){return blackColor;}


Layer STANDARD_FIELD_MODIFICATOR_LAYER = 0, STANDARD_FIELD_LAYER = -1000, STANDARD_LAYER = 1000, SIDETEXT_LAYER = 2000, CITY_LAYER = 1500;
DrawState Graphics::m_whatsUpDrawingwise = NOT_IN_ANY_DRAWING;
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
//std::cout<<"Texture-Konstruktor, this ="<<this<<", m_height = "<<m_height<<", m_width = "<<m_width<<std::endl;
}

Texture::~Texture(){
	//std::cout<<"Texture-Destruktor, this = "<<this<<std::endl;
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
#define PRINT(p) {os<<p; break;}
std::ostream& operator<<(std::ostream& os, Nationality nationality){
	switch(nationality){
	case GERMAN: PRINT("GERMAN")
	case ROMAN: PRINT("ROMAN")
	case CHINESE: PRINT("CHINESE")
	case MONGOL: PRINT("MONGOL")
	case FRENCH: PRINT("FRENCH")
	case AZTEC: PRINT("AZTEC")
	case EGYPTIAN: PRINT("EGYPTIAN")
	case GREEK: PRINT("GREEK")
	case ENGLISH: PRINT("ENGLISH")
	case INDIAN: PRINT("INDIAN")
	case RUSSIAN: PRINT("RUSSIAN")
	case AMERICAN: PRINT("AMERICAN")
	case ZULU: PRINT("ZULU")
	case BABYLONIAN: PRINT("BABYLONIAN")
	}
	return os;
}
#undef PRINT

char getSettlersOrder(SettlersWork work){
	switch(work){
	case MAKE_MINING: return 'm';
	case IRRIGATE: return 'i';
	case BUILD_ROAD: return 'r';
	case BUILD_RAILROAD: return 'm';
	case CHANGE_TO_FOREST: return 'm';
	case CHANGE_TO_GRASSLAND: return 'i';
	case CHANGE_TO_PLAINS: return 'i';
	case BUILD_FORTRESS: return 'f';
	case BUILD_BRIDGE: return 'r';
	case NONE: return ' ';
	}
	return 'F';
}

std::ostream& operator<<(std::ostream& os, FigureType figureType){
	switch(figureType){
	case SETTLERS: {os<<"SETTLERS"; break;}
	case MILITIA: {os<<"MILITIA"; break;}
	case TRANSPORT: {os<<"TRANSPORT"; break;}
	case SAIL: {os<<"SAIL"; break; }
	case TRIREME: {os<<"TRIREME"; break;}
	default: {os<<"UNKNOWN"; break;}
	}
	return os;
}

std::ostream& operator<<(std::ostream& os, MovementPoints mp){
	int a = ( mp.m_movementPoints)/3;
	switch((mp % 3).m_movementPoints){
	case 0: {os<<a; break;}
	case 1: {os<<a;os<<".3"; break;}
	case 2: {os<<a;os<<".7"; break;}
	}
	if(mp.m_movementPoints<0)os<<"MOVE PROHIBITED!";
	return os;
}
Coordinate Coordinates::leftCornerCoordinate(){return Coordinate(leftCornerX(),leftCornerY());}

std::ostream & operator <<(std::ostream & os, Coordinate & co)
{
	os << "(" << co.x << "," << co.y << ")";
	return os;
}

int SDL_SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color& color){
	return SDL_SetRenderDrawColor(renderer, (unsigned char) color.r,(unsigned char)color.g,(unsigned char)color.b,(unsigned char)color.a);
}
