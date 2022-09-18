/*
 * sdtypes.cpp
 *
 *  Created on: 14.02.2022
 *      Author: uwe-w
 */
#include "sdltypes.hpp"
#include "SDL2/SDL.h"
#include <iostream>
#include <sstream>
#include <SDL2\SDL_image.h>
#include <algorithm>


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
SDL_Color Graphics::Civ::cityBackgroundColor(){return SDL_Color{50,50,255};}
int CONTENT_BASE(2);

Layer STANDARD_FIELD_MODIFICATOR_LAYER = 0, STANDARD_FIELD_LAYER = -1000, STANDARD_LAYER = 1000, SIDETEXT_LAYER = 2000, CITY_LAYER = 1500;
DrawState Graphics::m_whatsUpDrawingwise = NOT_IN_ANY_DRAWING;
int modulo (int i, int j){
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

int xModulo(int i){
	return modulo(i  + STANDARD_FIELD_SIZE - MODULO_WIDTH_FOR_NEGATIVE, WORLD_LENGTH*STANDARD_FIELD_SIZE) - STANDARD_FIELD_SIZE + MODULO_WIDTH_FOR_NEGATIVE;
}
int yModulo(int i){
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
	if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)!=0)
		std::cout<<"SDL_Error in DrawSquareLines: \n"<<SDL_GetError()<<std::endl;
	int whatToReturn = 0;
	whatToReturn+=Graphics::drawThickerVerticalLine(renderer, x,y);
	whatToReturn+=Graphics::drawThickerVerticalLine(renderer, x+STANDARD_FIELD_SIZE-STANDARD_LINE_THICKNESS,y);
	whatToReturn+=Graphics::drawThickerHorizontalLine(renderer, x,y);
	whatToReturn+=Graphics::drawThickerHorizontalLine(renderer, x,y+STANDARD_FIELD_SIZE-STANDARD_LINE_THICKNESS);
	return whatToReturn;
}

int Graphics::drawSquareStarLines(SDL_Renderer* renderer, int x, int y, SDL_Color color){
	if(SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a)!=0)
		std::cout<<"SDL_Error in DrawSquareLines: \n"<<SDL_GetError()<<std::endl;
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
	case NO_NATIONALITY: PRINT("NO_NATIONALITY")
	default: PRINT("WEIRD NATIONALITY");
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
	default: {
		std::stringstream s;
		s<<(ImprovementType)figureType;
		s.flush();
		std::string String = s.str();
		std::transform(String.begin(), String.end(), String.begin(),[](unsigned char c){ return toupper(c); });
		os<<String;
	}
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

int SDL_SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color){
	return SDL_SetRenderDrawColor(renderer, (unsigned char) color.r,(unsigned char)color.g,(unsigned char)color.b,(unsigned char)color.a);
}

CitizenState operator++(CitizenState& state){
	switch(state){
	case UNHAPPY:
		return state = CONTENT;
	case HAPPY:
		throw InertCitizenState(HAPPY);
	case CONTENT:
		return state = HAPPY;
	case TAX_COLLECTOR:
		throw InertCitizenState(TAX_COLLECTOR);
	case ENTERTAINER:
		throw InertCitizenState(ENTERTAINER);
	case SCIENTIST:
		throw InertCitizenState(SCIENTIST);
	default:
		throw InertCitizenState((CitizenState) 0);
	}
}

CitizenState operator--(CitizenState& state){
	switch(state){
	case UNHAPPY:
		throw InertCitizenState(UNHAPPY);
	case HAPPY:
		return state = CONTENT;
	case CONTENT:
		return state = UNHAPPY;
	case TAX_COLLECTOR:
		throw InertCitizenState(TAX_COLLECTOR);
	case ENTERTAINER:
		throw InertCitizenState(ENTERTAINER);
	case SCIENTIST:
		throw InertCitizenState(SCIENTIST);
	default:
		throw InertCitizenState((CitizenState) 0);
	}
}

int whiteOrYellow(int X, int Y, int relativeX, int relativeY, SDL_Renderer* renderer){
	SDL_Color coloryellow = Graphics::Civ::yellowColor();
	SDL_Color colorwhite = Graphics::Civ::resourcesWhiteColor();
	int WhatToReturn = 0;
	if((X-relativeX +Y -relativeY) % 2 ==0){
		WhatToReturn += SDL_SetRenderDrawColor(renderer, coloryellow);
	}
	else{
		WhatToReturn += SDL_SetRenderDrawColor(renderer, colorwhite);
	}
	SDL_Rect r{X,Y,1,1};
	WhatToReturn +=SDL_RenderFillRect(renderer,&r);
	return WhatToReturn;
}


int Graphics::Civ::drawFood(SDL_Renderer* renderer, int x, int y, int scaleFactor, bool minus){
	SDL_Color colorblack = blackColor;
	int whatToReturn = 0;
	whatToReturn += SDL_SetRenderDrawColor(renderer, blackColor);
	if(minus == false){
	SDL_Rect rectToFill{x,y,2*scaleFactor,scaleFactor};
	whatToReturn += SDL_RenderFillRect(renderer, &rectToFill);
	rectToFill.x +=5*scaleFactor;
	rectToFill.y+=scaleFactor;
	whatToReturn += SDL_RenderFillRect(renderer, &rectToFill);
	rectToFill.w = scaleFactor;
	rectToFill.x -= 3*scaleFactor;
	whatToReturn += SDL_RenderFillRect(renderer, &rectToFill);
	rectToFill.w = 2*scaleFactor;
	rectToFill.x += scaleFactor;
	rectToFill.y+=scaleFactor;
	whatToReturn += SDL_RenderFillRect(renderer, &rectToFill);
	rectToFill.w = scaleFactor;
	rectToFill.x = x;
	rectToFill.y += scaleFactor;
	whatToReturn += SDL_RenderFillRect(renderer, &rectToFill);
	rectToFill.h += 3*scaleFactor;
	rectToFill.x += 3*scaleFactor;
	whatToReturn += SDL_RenderFillRect(renderer, &rectToFill);
	rectToFill.h -= scaleFactor;
	rectToFill.y += scaleFactor;
	rectToFill.x -= 2*scaleFactor;
	whatToReturn += SDL_RenderFillRect(renderer, &rectToFill);
	rectToFill.x += 5*scaleFactor;
	rectToFill.h -= 2*scaleFactor;
	whatToReturn += SDL_RenderFillRect(renderer, &rectToFill);
	rectToFill.x -= scaleFactor;
	rectToFill.h += scaleFactor;
	rectToFill.y+=scaleFactor;
	whatToReturn += SDL_RenderFillRect(renderer, &rectToFill);

	//Now, the white and the yellow.

	int xCoordinate[19]={0,1,0,1,2,5,6,1,2,4,5,6,2,4,5,2,4,2,4};
	int yCoordinate[19]={1,1,2,2,2,2,2,3,3,3,3,3,4,4,4,5,5,6,6};
	for(int i(0); i<19; i++){
		for(int j(0); j < scaleFactor; j++){
			for(int k(0); k < scaleFactor; k++){
				whatToReturn += whiteOrYellow(x+scaleFactor*xCoordinate[i]+j, y+scaleFactor*yCoordinate[i]+k, x, y, renderer);
			}
		}
	}
}
	if(minus){
		SDL_SetRenderDrawColor(theRenderer, blackColor);
		int yCoordinateBlack[] = {0,0,1,1,1,1,2,2,2,2,3,3,3,3,4,4,4,4,4,5,5,5,5,5,6,6,6,6,6};
		int xCoordinateBlack[] = {0,1,0,2,5,6,1,3,4,6,0,2,3,5,6,1,2,3,4,1,2,3,4,5,1,2,3,4,5};
		for(int i(0); i<29; i++){
			SDL_Rect rect{x + scaleFactor*xCoordinateBlack[i], y+scaleFactor*yCoordinateBlack[i], scaleFactor, scaleFactor};
			whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
		}
		int xCoordinateWhiteYellow[8]={1,0,2,1,5,4,6,5};
		int yCoordinateWhiteYellow[8]={1,2,2,3,2,3,3,4};
		for(int i(0); i<8; i++){
			for(int j(0); j < scaleFactor; j++){
				for(int k(0); k < scaleFactor; k++){
					whatToReturn += whiteOrYellow(x+scaleFactor*xCoordinateWhiteYellow[i]+j, y+scaleFactor*yCoordinateWhiteYellow[i]+k, x, y, renderer);
				}
			}
		}
	}


	return whatToReturn;
}

SDL_Color Graphics::Civ::brightCityBackgroundColor(){
	return SDL_Color{125,125,255};
}

SDL_Color Graphics::Civ::darkgreyColor(){return SDL_Color{72,72,72};}

SDL_Color Graphics::Civ::shieldGreyColor(){return SDL_Color{138,138,142};}
SDL_Color Graphics::Civ::resourcesWhiteColor(){return SDL_Color{235,235,235};}
int Graphics::Civ::drawShield(SDL_Renderer* renderer, int x, int y, int scaleFactor, bool minus){
	int whatToReturn = 0;
	SDL_Color colorgrey = Graphics::Civ::shieldGreyColor();
	whatToReturn+=SDL_SetRenderDrawColor(renderer, colorgrey);
	int xCoordinateGrey[]={2,3,4,3,3};
	int yCoordinateGrey[]={1,1,1,3,4};
	for(int i(0); i<5; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateGrey[i], y+scaleFactor*yCoordinateGrey[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	SDL_SetRenderDrawColor(theRenderer, blackColor);
	int xCoordinateBlack[]={1,2,3,4,5,0,6,0,6,0,6,0,6,1,5,2,3,4};
	int yCoordinateBlack[]={0,0,0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,6};
	for(int i(0); i<18; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateBlack[i], y+scaleFactor*yCoordinateBlack[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	if(!minus){
		SDL_Color colorwhite = Graphics::Civ::resourcesWhiteColor();
		whatToReturn+=SDL_SetRenderDrawColor(renderer, colorwhite);
	}
	int xCoordinateWhite[] = {1,5,1,2,3,4,5,1,2,4,5,1,2,4,5,2,3,4};
	int yCoordinateWhite[] = {1,1,2,2,2,2,2,3,3,3,3,4,4,4,4,5,5,5};
	for(int i(0); i<18; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateWhite[i], y+scaleFactor*yCoordinateWhite[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	return whatToReturn;
}

int Graphics::Civ::drawScience(SDL_Renderer* renderer, int x, int y, int scaleFactor){
	int whatToReturn = 0;
	SDL_Color colorblack = blackColor;
	int xCoordinateBlack[] = {2,3,4,1,5,0,6,0,6,0,6,1,5,2,4,2,4};
	int yCoordinateBlack[] = {0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,7,7};
	whatToReturn += SDL_SetRenderDrawColor(theRenderer, colorblack);
	for(int i(0); i<15; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateBlack[i], y+scaleFactor*yCoordinateBlack[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	SDL_Color colordarkgrey = Graphics::Civ::darkgreyColor();
	int xCoordinateDarkGrey = 3;
	int yCoordinateDarkGrey = 6;
	whatToReturn += SDL_SetRenderDrawColor(theRenderer, colordarkgrey);
	SDL_Rect rect{x + scaleFactor*xCoordinateDarkGrey, y+scaleFactor*yCoordinateDarkGrey, scaleFactor, scaleFactor};
	whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	SDL_Color colorgrey = Graphics::Civ::shieldGreyColor();
	whatToReturn += SDL_SetRenderDrawColor(theRenderer, colorgrey);
	int xCoordinateGrey[] = {3,3};
	int yCoordinateGrey[] = {4,5};
	for(int i(0); i<2; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateGrey[i], y+scaleFactor*yCoordinateGrey[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	SDL_Color colorturquoise = Graphics::Civ::turquoiseColor();
	whatToReturn += SDL_SetRenderDrawColor(theRenderer, colorturquoise);
	int xCoordinateTurquoise[] = {2,3,4,1,2,4,5,1,2,4,5,2,4};
	int yCoordinateTurquoise[] = {1,1,1,2,2,2,2,3,3,3,3,4,4};
	for(int i(0); i<13; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateTurquoise[i], y+scaleFactor*yCoordinateTurquoise[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	SDL_Color colorwhite = Graphics::Civ::resourcesWhiteColor();
	whatToReturn += SDL_SetRenderDrawColor(theRenderer, colorwhite);
	int xCoordinateWhite[] = {3,3};
	int yCoordinateWhite[] = {2,3};
	for(int i(0); i<2; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateWhite[i], y+scaleFactor*yCoordinateWhite[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	return whatToReturn;
}

int Graphics::Civ::drawTrade(SDL_Renderer* renderer, int x, int y, int scaleFactor, bool corruption){
	int whatToReturn = 0;
	SDL_Color colorblack = blackColor;
	SDL_Color coloryellow = Graphics::Civ::yellowColor();
	whatToReturn+=SDL_SetRenderDrawColor(renderer, coloryellow);
	int xCoordinateYellow[]={6,5,2,4,2,3,2,3,4};
	int yCoordinateYellow[]={2,3,4,4,5,5,6,6,6};
	for(int i(0); i<9; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateYellow[i], y+scaleFactor*yCoordinateYellow[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	SDL_SetRenderDrawColor(theRenderer, blackColor);
	int xCoordinateBlack[]={5,2,5,1,3,5,0,2,4,6,1,3,5,1,4,1};
	int yCoordinateBlack[]={0,1,1,2,2,2,3,3,3,3,4,4,4,5,5,6};
	for(int i(0); i<16; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateBlack[i], y+scaleFactor*yCoordinateBlack[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	if(!corruption){
		SDL_Color colorwhite = Graphics::Civ::resourcesWhiteColor();
		whatToReturn+=SDL_SetRenderDrawColor(renderer, colorwhite);
	}
	int xCoordinateWhite[] = {2,3,4,3,4,2,4,1,0};
	int yCoordinateWhite[] = {0,0,0,1,1,2,2,3,4};
	for(int i(0); i<9; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateWhite[i], y+scaleFactor*yCoordinateWhite[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	whatToReturn;
}

SDL_Color Graphics::Civ::yellowColor(){
	return SDL_Color{255,255,150};
}

SDL_Color Graphics::Civ::turquoiseColor(){
	return SDL_Color{12,227,235};
}

int Graphics::Civ::drawUnhappyFace(SDL_Renderer* renderer, int x, int y, int scaleFactor){
	int whatToReturn = 0;
	SDL_Color colorblack = blackColor;
	SDL_Color coloryellow = Graphics::Civ::yellowColor();
	whatToReturn+=SDL_SetRenderDrawColor(renderer, coloryellow);
	int xCoordinateYellow[]={2,4};
	int yCoordinateYellow[]={2,2};
	for(int i(0); i<2; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateYellow[i], y+scaleFactor*yCoordinateYellow[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	whatToReturn+=SDL_SetRenderDrawColor(theRenderer, blackColor);
	int xCoordinateBlack[]={1,2,3,4,5,0,1,2,3,4,5,6,0,1,3,5,6,0,1,2,3,4,5,6,0,1,5,6,0,2,3,4,6,1,2,3,4,5};
	int yCoordinateBlack[]={0,0,0,0,0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5,5,6,6,6,6,6};
	for(int i(0); i<38; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateBlack[i], y+scaleFactor*yCoordinateBlack[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	return whatToReturn;
}

int Graphics::Civ::drawLuxury(SDL_Renderer* renderer, int x, int y, int scaleFactor){
	int whatToReturn = 0;
	SDL_Color colorwhite = Graphics::Civ::resourcesWhiteColor();
	whatToReturn+=SDL_SetRenderDrawColor(renderer, colorwhite);
	int xCoordinateWhite[] = {3,2,3,1,2,3,0,1,2,3};
	int yCoordinateWhite[] = {0,1,1,2,2,2,3,3,3,3};
	for(int i(0); i<10; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateWhite[i], y+scaleFactor*yCoordinateWhite[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	SDL_Color colorturquoise = Graphics::Civ::turquoiseColor();
	int xCoordinateTurquoise[] = {4,4,5,4,5,6,1,2,3,4,5,2,3,4,3};
	int yCoordinateTurquoise[] = {1,2,2,3,3,3,4,4,4,4,4,5,5,5,6};
	whatToReturn+=SDL_SetRenderDrawColor(renderer, colorturquoise);
	for(int i(0); i<15; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateTurquoise[i], y+scaleFactor*yCoordinateTurquoise[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	return whatToReturn;
}

int Graphics::Civ::drawGold(SDL_Renderer* renderer, int x, int y, int scaleFactor){
	int whatToReturn = 0;
	SDL_Color colorwhite = Graphics::Civ::resourcesWhiteColor();
	whatToReturn+=SDL_SetRenderDrawColor(renderer, colorwhite);
	int xCoordinateWhite[] = {2,3,4,1,5,1,5,1,5,2,3,4};
	int yCoordinateWhite[] = {1,1,1,2,2,3,3,4,4,5,5,5};
	for(int i(0); i<12; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateWhite[i], y+scaleFactor*yCoordinateWhite[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	SDL_Color coloryellow = Graphics::Civ::yellowColor();
	int xCoordinateYellow[] = {2,3,4,2,3,4,2,3,4};
	int yCoordinateYellow[] = {2,2,2,3,3,3,4,4,4};
	whatToReturn+=SDL_SetRenderDrawColor(renderer, coloryellow);
	for(int i(0); i<9; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateYellow[i], y+scaleFactor*yCoordinateYellow[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	SDL_Color colorblack = blackColor;
	int xCoordinateBlack[] = {2,3,4,1,5,0,6,0,6,0,6,1,5,2,3,4};
	int yCoordinateBlack[] = {0,0,0,1,1,2,2,3,3,4,4,5,5,6,6,6};
	whatToReturn+=SDL_SetRenderDrawColor(renderer, colorblack);
	for(int i(0); i<16; i++){
		SDL_Rect rect{x + scaleFactor*xCoordinateBlack[i], y+scaleFactor*yCoordinateBlack[i], scaleFactor, scaleFactor};
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);
	}
	return whatToReturn;
}

void DoNothing::operator ()(){}
DoNothing::DoNothing(){}

std::vector<Coordinate> coordinatesAroundField(int visibilityRange){
	std::vector<Coordinate> whatToReturn;
	for(int i(-visibilityRange); i<=visibilityRange;i++){
		for(int j(-visibilityRange); j<=visibilityRange;j++){
			whatToReturn.push_back(Coordinate(i,j));
		}
	}
	return whatToReturn;
}

std::ostream& operator<<(std::ostream& os, ImprovementType imptype){
	switch(imptype){
	case IMPROVEMENT_ARTILLERY: os<<"Artillery"; break;
	case IMPROVEMENT_SETTLERS: os<<"Settlers"; break;
	case IMPROVEMENT_TRIREME: os<<"Trireme"; break;
	case IMPROVEMENT_BOMBER: os<<"Bomber"; break;
	case IMPROVEMENT_CAVALRY: os<<"Cavalry"; break;
	case IMPROVEMENT_DIPLOMAT: os<<"Diplomat"; break;
	case IMPROVEMENT_CARRIER: {os<<"Carrier"; break;}
	case IMPROVEMENT_FIGHTER: {os<<"Fighter"; break;}
	case PALACE: os<<"Palace"; break;
	case TEMPLE: os<<"Temple"; break;
	case GRANARY: os<<"Granary"; break;
	case COURTHOUSE: os<<"Courthouse";break;
	case AQUEDUCT: os<<"Aqueduct"; break;
	case BANK: os<<"Bank"; break;
	case LIBRARY: os<<"Library"; break;
	case MFG_PLANT: os<<"Mfg Plant"; break;
	case CATHEDRAL: os<<"Cathedral"; break;
	case BARRACKS: os<<"Barracks"; break;
	case NUCLEAR_PLANT: os<<"Nuclear Plant"; break;
	case COLOSSEUM: os<<"Colosseum"; break;
	case POWER_PLANT: os<<"Power Plant"; break;
	case MARKETPLACE: os<<"Marketplace"; break;
	case MASS_TRANSIT: os<<"Mass Transit"; break;
	case RECYCLING_CENTER: os<<"Recycling Center"; break;
	case SDI_DEFENSE: os<<"SDI Defense"; break;
	case CITY_WALLS: os<<"City Walls"; break;
	case UNIVERSITY: os<<"University"; break;
	case HYDRO_PLANT: os<<"Hydro Plant"; break;
	case APOLLO_PROGRAM: os<<"Apollo Program"; break;
	case GREAT_WALL: os<<"Great Wall"; break;
	case DARWINS_VOYAGE: os<<"Darwin's Voyage"; break;
	case MAGELLANS_EXPEDITION: os<<"Magellan's Expedition"; break;
	case WOMENS_SUFFRAGE: os<<"Women's Suffrage"; break;
	case GREAT_LIBRARY: os<<"Great Library"; break;
	case HOOVER_DAM: os<<"Hoover Dam"; break;
	case HANGING_GARDENS: os<<"Hanging Gardens"; break;
	case ISAAC_NEWTONS_COLLEGE: os<<"Isaac Newton's College"; break;
	case BACH_CATHEDRAL: os<<"Bach's Cathedral"; break;
	case MICHELANGELOS_CHAPEL: os<<"Michelangelo' Chapel"; break;
	case COLOSSUS: os<<"Colossus"; break;
	case COPERNICUS_OBSERVATORY: os<<"Copernicus' Observatory"; break;
	case CURE_FOR_CANCER: os<<"Cure for Cancer"; break;
	case LIGHTHOUSE: os<<"Lighthouse"; break;
	case MANHATTAN_PROJECT: os<<"Manhattan Project"; break;
	case ORACLE: os<<"Oracle"; break;
	case PYRAMIDS: os<<"Pyramids"; break;
	case SETI_PROGRAM: os<<"SETI Program"; break;
	case SHAKESPEARES_THEATRE: os<<"Shakespeare's Theatre"; break;;
	case UNITED_NATIONS: os<<"United Nations"; break;
	case SS_STRUCTURAL: os<<"SS Structural"; break;
	case SS_MODULE: os<<"SS Module"; break;
	case SS_COMPONENT: os<<"SS Component"; break;
	default: os<<(int)imptype; break;
	}
	return os;
}

bool lolllll = false;

int Graphics::Civ::drawIrrigation(SDL_Renderer* renderer, int x, int y){
	int whatToReturn = 0;
	std::vector<int> xCoordinateBlue={3,11,3,4,5,6,7,8,9,10,11,11,12,13,14,11,11,0,1,2,3,4,5,6,7,8,9,10,11,9,14,3,9,14,3,9,14,3,9,14,10,11,12,13,14,3,11};
	int yToStart = 0;

	whatToReturn+=SDL_SetRenderDrawColor(theRenderer, Graphics::Civ::irrigationBlueColor());
	for(int i(0);i<xCoordinateBlue.size();i++){
		if(i>0 && xCoordinateBlue[i-1]>=xCoordinateBlue[i]){
			yToStart++;
		}
		SDL_Rect rect{x + 4*xCoordinateBlue[i], y+4*yToStart,4,4};
		if(!lolllll)
			std::cout<<"drawing x"<<xCoordinateBlue[i]<<", y "<<yToStart<<std::endl;
		whatToReturn += SDL_RenderFillRect(theRenderer, &rect);

	}
	lolllll = true;
	return whatToReturn;
}

SDL_Color Graphics::Civ::irrigationBlueColor(){return SDL_Color{85,89,162,128};}

SDL_Rect Miscellaneous::printMultipleLines(std::stringstream& str, int x, int y, SDL_Color color, bool shaded, SDL_Color backgroundColor){
	int maxWidth = 0,
	height = 0;
	std::string stringOfInterest = "";
	stringOfInterest = str.str();
	std::cout<<str.str()<<SDL_GetError()<<std::endl;
	std::string temporaryString = "";
	int length = stringOfInterest.length();
	int reachedHeight = 0;
	std::vector<SDL_Surface*> textures;
	for(int i(0); i<length; i++){
		if(stringOfInterest[i] != '\n'){
			temporaryString += stringOfInterest[i];
			continue;
		}
		else{
			SDL_Surface* surface = TTF_RenderText_Blended(theFont, temporaryString.c_str(), color);
			//	std::cout<<temporaryString<<std::endl;
			temporaryString = "";
			if(!surface){
				continue;
			}
			maxWidth=std::max(maxWidth,surface->w);
			height+=surface->h;
			textures.push_back(surface);
		}
	}
	SDL_Rect rectToFill{x,y,maxWidth,height};
	if(shaded){
		SDL_SetRenderDrawColor(theRenderer, backgroundColor);
		SDL_RenderFillRect(theRenderer, &rectToFill);
	}
	for(SDL_Surface* currentSurface: textures){
		SDL_Texture* texture = SDL_CreateTextureFromSurface(theRenderer, currentSurface);
		SDL_Rect textRect{x, reachedHeight+y, currentSurface->w, currentSurface->h};
		reachedHeight+=currentSurface->h;
		SDL_RenderCopy(theRenderer, texture, nullptr,&textRect);
		SDL_FreeSurface(currentSurface);
		SDL_DestroyTexture(texture);
	}
			std::cout<<234456<<std::endl;
	return rectToFill;
}

SDL_Color Graphics::redColor(){return SDL_Color{255,0,0};}
