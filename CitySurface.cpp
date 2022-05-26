/*
 * CitySurface.cpp
 *
 *  Created on: 24.05.2022
 *      Author: uwe-w
 */

#include "City.hpp"
#include "sstream"

CitySurface::CitySurface(City* city): m_associatedCity(city){}

void CitySurface::m_drawCitizens(SDL_Renderer* renderer, int x, int y){
	SDL_Color backgroundColor (Graphics::Civ::cityBackgroundColor());
	SDL_SetRenderDrawColor(theRenderer, backgroundColor);
	SDL_Rect backgroundRect{x,y,SCREEN_WIDTH - IMPROVEMENTS_OVERVIEW_WIDTH, CITIZENS_OVERVIEW_HEIGHT};
	SDL_RenderFillRect(theRenderer, &backgroundRect);
	int currentX = x + FIRST_CITIZEN_X;
	int yToDraw = y + CITIZENS_OVERVIEW_HEIGHT - CITIZEN_HEIGHT*CITIZEN_SCALEFACTOR;
	bool female = false;
	std::vector<CitizenState> citizenVector = m_associatedCity->m_applyCitizenStateVector();
	CitizenState currentState;
	for(int i(0); i<citizenVector.size(); i++){
		if(i==0 || currentState!=citizenVector[i-1]){
			female = false;
		}
		else{
			female = !female;
		}
		currentState = citizenVector[i];
		std::stringstream filenameStream;
		filenameStream<<"bilder/Citizens/";
		switch(currentState){
		case HAPPY:
		{
			filenameStream<<"HappyFace"<<(female ? "Female" : "Male");
			break;
		}
		case UNHAPPY:{
			filenameStream<<"UnhappyFace"<<(female ? "Female" : "Male");
			break;
		}
		case CONTENT:{
			filenameStream<<"ContentFace"<<(female ? "Female" : "Male");
			break;
		}
		case ENTERTAINER:
		{
			filenameStream<<"Entertainer";
			break;
		}
		case TAX_COLLECTOR:
		{
			filenameStream<<"TaxCollector";
			break;
		}
		case SCIENTIST:
		{
			filenameStream<<"Scientist";
			break;
		}
		}
		filenameStream<<".png";
		filenameStream.flush();
		SDL_Texture* textureToDraw = IMG_LoadTexture(theRenderer, filenameStream.str().c_str());
		SDL_Rect rectToFill{currentX, yToDraw, CITIZEN_SCALEFACTOR*(currentState == ENTERTAINER ? 8 : 7), CITIZEN_SCALEFACTOR*CITIZEN_HEIGHT};
		std::cout<<filenameStream.str()<<SDL_GetError()<<std::endl;
		SDL_RenderCopy(theRenderer, textureToDraw, NULL, &rectToFill);
		currentX+=CITIZEN_MAX_WIDTH;
		SDL_DestroyTexture(textureToDraw);
	}
}

void CitySurface::m_displaySurface(SDL_Renderer* renderer){
	SDL_SetRenderDrawColor(theRenderer, brownColor);
	SDL_RenderClear(renderer);
	m_drawFoodProduction();
	m_drawCitizens(renderer, 0,0);
	SDL_RenderPresent(theRenderer);
}

void CitySurface::m_drawFoodProduction(){
	int checkInt = 0;
	const int foodWidth = RESOURCES_SCALEFACTOR*7;
	int x = 0;
	const int yToStart = CITIZENS_OVERVIEW_HEIGHT + RESOURCES_TEXT_HEIGHT;
	const int distance = std::max(2*RESOURCES_SCALEFACTOR, std::min(foodWidth,(int)PRODUCTION_OVERVIEW_WIDTH));
	for(int i(0); i<m_associatedCity->m_foodCost(); i++){
		if(i>=m_associatedCity->m_foodProduction()){
			std::cout<<"minusfood"<<std::endl;
			checkInt += Graphics::Civ::drawFood(theRenderer, x = i*distance, yToStart, RESOURCES_SCALEFACTOR, true);
		}
		else{
			std::cout<<"plusfood"<<std::endl;
			checkInt +=	Graphics::Civ::drawFood(theRenderer, x = i*distance, yToStart, RESOURCES_SCALEFACTOR, false);
		}
	}
	for(int i(m_associatedCity->m_foodCost()); i<m_associatedCity->m_foodProduction();i++){
		checkInt +=Graphics::Civ::drawFood(theRenderer, x = i*distance + 3*foodWidth/2, yToStart, RESOURCES_SCALEFACTOR, false);
	}
	if(checkInt != 0){
		std::cout<<"SDL Error in food drawing: "<<SDL_GetError()<<std::endl;
	}
}
