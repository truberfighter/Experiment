/*
 * CitySurface.cpp
 *
 *  Created on: 24.05.2022
 *      Author: uwe-w
 */

#include "Field.hpp"
#include "Drawing.hpp"
#include "City.hpp"
#include "sstream"

CitySurface::CitySurface(City* city): m_associatedCity(city)
{
	m_subsurface = std::make_unique<Subsurface>(this);
}

Subsurface::Subsurface(CitySurface* surface):m_surface(surface)
{
	m_state = SUBSURFACE_HAPPY;
}

void CitySurface::m_drawCitizens(SDL_Renderer* renderer, int x, int y, int backgroundWidth){
	SDL_Color backgroundColor (Graphics::Civ::cityBackgroundColor());
	SDL_SetRenderDrawColor(theRenderer, backgroundColor);
	SDL_Rect backgroundRect{x,y,backgroundWidth, CITIZENS_OVERVIEW_HEIGHT};
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
		SDL_RenderCopy(theRenderer, textureToDraw, NULL, &rectToFill);
		currentX+=CITIZEN_MAX_WIDTH;
		SDL_DestroyTexture(textureToDraw);
	}
}

void CitySurface::m_displaySurface(SDL_Renderer* renderer){
	SDL_SetRenderDrawColor(theRenderer, brownColor);
	SDL_RenderClear(renderer);
	m_drawFoodProduction();
	m_drawShieldProduction();
	m_drawFoodStorage();
	m_drawCitizens(renderer, 0,0);
	m_drawCityFields();
	m_drawFigures();
	m_drawSubsurfaceButtons();
	m_subsurface->m_draw();
	SDL_RenderPresent(theRenderer);
}

void CitySurface::m_drawFoodProduction(){
	int checkInt = 0;
	const int foodWidth = RESOURCES_SCALEFACTOR*7;
	int x = 0;
	const int yToStart = CITIZENS_OVERVIEW_HEIGHT + RESOURCES_TEXT_HEIGHT;
	const int distance = std::max(1*RESOURCES_SCALEFACTOR, std::min(foodWidth,(int)PRODUCTION_OVERVIEW_WIDTH/std::max(m_associatedCity->m_foodCost(), m_associatedCity->m_foodProduction())));
	for(int i(0); i<m_associatedCity->m_foodCost(); i++){
		if(i>=m_associatedCity->m_foodProduction()){
			//std::cout<<"minusfood"<<std::endl;
			checkInt += Graphics::Civ::drawFood(theRenderer, x = i*distance, yToStart, RESOURCES_SCALEFACTOR, true);
		}
		else{
			//std::cout<<"plusfood"<<std::endl;
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

void CitySurface::m_drawFoodStorage(){
	SDL_Rect foodRect{0,FOOD_STORAGE_Y, FOOD_STORAGE_WIDTH, SCREEN_WIDTH - FOOD_STORAGE_Y};
	SDL_Color storageColor = Graphics::Civ::brightCityBackgroundColor(),
	backgroundColor = Graphics::Civ::cityBackgroundColor();
	SDL_SetRenderDrawColor(theRenderer, backgroundColor);
	SDL_RenderFillRect(theRenderer, &foodRect);
	SDL_Surface* textSurface = TTF_RenderText_Solid(theFont, "FOOD STORAGE", whiteColor);
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(theRenderer, textSurface);
	SDL_Rect textRect{0,FOOD_STORAGE_Y, textSurface->w, textSurface->h};
	SDL_RenderCopy(theRenderer, textTexture, nullptr, &textRect);
	SDL_SetRenderDrawColor(theRenderer, storageColor);
	SDL_DestroyTexture(textTexture);
	int foodPerRow = m_associatedCity->m_foodStorageSize()/10;
	const int foodWidth = RESOURCES_SCALEFACTOR*7;
	const int distance = std::max(1*RESOURCES_SCALEFACTOR, std::min(foodWidth,(int)FOOD_STORAGE_WIDTH/foodPerRow));
	const int foodStorageWidth = std::min((int) FOOD_STORAGE_WIDTH, distance*foodPerRow);
	const int verticalDistance = 8*RESOURCES_SCALEFACTOR;
	SDL_Rect brightRect{0,FOOD_STORAGE_Y+textRect.h,foodStorageWidth,10*verticalDistance};
	SDL_SetRenderDrawColor(theRenderer, storageColor);
	SDL_RenderFillRect(theRenderer, &brightRect);
	const int food = m_associatedCity->m_food;
	for(int currentFoodAmount(0); currentFoodAmount < food;currentFoodAmount++){
		Graphics::Civ::drawFood(theRenderer, distance*(currentFoodAmount % foodPerRow), brightRect.y + (currentFoodAmount/foodPerRow)*verticalDistance, RESOURCES_SCALEFACTOR, false);
	}
	SDL_FreeSurface(textSurface);
}

void CitySurface::m_drawShieldProduction(){
	int checkInt = 0;
	bool isThereAnyCost = false;
	const int shieldWidth = RESOURCES_SCALEFACTOR*7;
	int x = 0;
	const int yToStart = CITIZENS_OVERVIEW_HEIGHT + RESOURCES_TEXT_HEIGHT + RESOURCE_TEXT_SHIELD_DIFFERENCE;
	const int distance = std::max(1*RESOURCES_SCALEFACTOR, std::min(shieldWidth,(int)PRODUCTION_OVERVIEW_WIDTH/std::max(m_associatedCity->m_shieldCost(), m_associatedCity->m_shieldProduction())));
	for(int i(0); i<m_associatedCity->m_shieldCost(); i++){
		isThereAnyCost = true;
		if(i>=m_associatedCity->m_shieldProduction()){
			//std::cout<<"minusshield"<<std::endl;
			checkInt += Graphics::Civ::drawShield(theRenderer, x = i*distance, yToStart, RESOURCES_SCALEFACTOR, true);
		}
		else{
			//std::cout<<"plusshield"<<std::endl;
			checkInt +=	Graphics::Civ::drawShield(theRenderer, x = i*distance, yToStart, RESOURCES_SCALEFACTOR, false);
		}
	}
	for(int i(m_associatedCity->m_shieldCost()); i<m_associatedCity->m_shieldProduction();i++){
		checkInt +=Graphics::Civ::drawShield(theRenderer, x = i*distance + (isThereAnyCost ? 3*shieldWidth/2 : 0), yToStart, RESOURCES_SCALEFACTOR, false);
	}
	if(checkInt != 0){
		std::cout<<"SDL Error in food drawing: "<<SDL_GetError()<<std::endl;
	}
}

void CitySurface::m_drawCityFields(){
	int xToStart = PRODUCTION_OVERVIEW_WIDTH;
	int yToStart = CITIZENS_OVERVIEW_HEIGHT;
	Nation& nation = *m_associatedCity->m_owningNation;
	std::vector<Coordinate> coordinateVector = Field::coordinatesAroundCity();
	std::vector<std::shared_ptr<Field>> fieldVector = m_associatedCity->m_WhereItStands()->m_cityFieldsAround();
	for (int i(0); i<21; i++){
		bool drawProduction = false;
		for(Citizen& currentCitizen: m_associatedCity->m_citizens){
			if(currentCitizen.m_whereItWorks == fieldVector[i]){
				drawProduction = true;
			}
		}
		fieldVector[i]->m_DrawingElement()->m_drawAt(xToStart + (2+coordinateVector[i].x)*STANDARD_FIELD_SIZE, yToStart+(2+coordinateVector[i].y)*STANDARD_FIELD_SIZE, theRenderer);
		int fieldX = xToStart + (2+coordinateVector[i].x)*STANDARD_FIELD_SIZE;
		int fieldY =  yToStart+(2+coordinateVector[i].y)*STANDARD_FIELD_SIZE;
		if(!drawProduction && fieldVector[i]!=m_associatedCity->m_whereItStands){
			if((fieldVector[i]->m_CitizenWorking()&& (&(fieldVector[i]->m_CitizenWorking()->m_home)!=m_associatedCity)||(fieldVector[i]->m_CityContained().get()!=nullptr && fieldVector[i]->m_CityContained().get()!=m_associatedCity))){
				Graphics::drawSquareLines(theRenderer, fieldX, fieldY, SDL_Color{255,0,0});
			}
			continue;
		}
		int productionIconIndex = 0;
		int productionTotal = fieldVector[i]->m_food(nation)+fieldVector[i]->m_shields(nation)+fieldVector[i]->m_trade(nation);
		int iconsPerRow = std::min(4,std::max((productionTotal+1)/2,2));
		for(int j(0); j<fieldVector[i]->m_food(nation); j++){
			Graphics::Civ::drawFood(theRenderer, fieldX + (productionIconIndex % iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*8, fieldY + (productionIconIndex / iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*8, RESOURCES_SCALEFACTOR_CITY_OVERVIEW, false);
			productionIconIndex++;
		}
		for(int j(0); j<fieldVector[i]->m_shields(nation); j++){
			Graphics::Civ::drawShield(theRenderer, fieldX + (productionIconIndex % iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*8, fieldY + (productionIconIndex / iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*8, RESOURCES_SCALEFACTOR_CITY_OVERVIEW);
			productionIconIndex++;
		}
		for(int j(0); j<fieldVector[i]->m_trade(nation); j++){
			Graphics::Civ::drawTrade(theRenderer, fieldX + (productionIconIndex % iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*8, fieldY + (productionIconIndex / iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*8, RESOURCES_SCALEFACTOR_CITY_OVERVIEW, false);
			productionIconIndex++;
		}
		if(productionTotal==0){
			Graphics::Civ::drawUnhappyFace(theRenderer, fieldX + STANDARD_FIELD_SIZE/2 - 4*RESOURCES_SCALEFACTOR_CITY_OVERVIEW,fieldY+STANDARD_FIELD_SIZE/2 - 4*RESOURCES_SCALEFACTOR_CITY_OVERVIEW, RESOURCES_SCALEFACTOR_CITY_OVERVIEW);
		}
	}
}

void CitySurface::m_drawFigures(){
	std::cout<<"figures to draw"<<std::endl;
	SDL_Color white = Graphics::Civ::brightCityBackgroundColor();
	SDL_SetRenderDrawColor(theRenderer, white);
	SDL_Rect brightRect{0,SCREEN_HEIGHT-FOOD_STORAGE_HEIGHT-FIGURE_OVERVIEW_HEIGHT,PRODUCTION_OVERVIEW_WIDTH, FIGURE_OVERVIEW_HEIGHT};
	SDL_RenderFillRect(theRenderer, &brightRect);
	std::vector<UnitCostingResources> costVector = m_associatedCity->m_unitCostVector();
	int figureCount = m_associatedCity->m_figuresOwned.size();
	int rowCount = FIGURE_OVERVIEW_HEIGHT/STANDARD_FIELD_SIZE;
	int figuresPerRow = std::min(PRODUCTION_OVERVIEW_WIDTH/10, std::max(figureCount/rowCount + 1, PRODUCTION_OVERVIEW_WIDTH/STANDARD_FIELD_SIZE));
	std::list<std::shared_ptr<Figure>>::iterator it = m_associatedCity->m_figuresOwned.begin();
	int distance = PRODUCTION_OVERVIEW_WIDTH/figuresPerRow;
	for(int row(0); row<rowCount; row++){
		int xToStart = 0;
		int yToStart = brightRect.y + row*STANDARD_FIELD_SIZE;
		for(int i(row*figuresPerRow); i<std::min(figureCount, figuresPerRow*(row+1));i++){
			int x = xToStart+(i%figuresPerRow)*distance;
			int y = yToStart+(i/figuresPerRow)*distance;
			std::cout<<"drawNew"<<std::endl;
			if((*it)->m_Image()->m_drawNew(x, y)){
				std::cout<<"well drawn"<<std::endl;
			};
			std::cout<<"SDL_error_"<<SDL_GetError()<<std::endl;
			UnitCostingResources cost{nullptr};
			for(UnitCostingResources& currentCost: costVector){
				if(currentCost.figure == it->get()){
					cost = currentCost;
				}
			}
			std::cout<<"figure: "<<cost.figure<<", foodCost: "<<cost.foodCost<<", shieldCost = "<<cost.shieldCost<<", unhappyFaces: "<<cost.unhappyFaces<<std::endl;
			if(cost.figure!=nullptr){
				int itemCount = 0;
				for(;itemCount<cost.foodCost;itemCount++){
					Graphics::Civ::drawFood(theRenderer, x+(itemCount%2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), y+(itemCount/2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), RESOURCES_SCALEFACTOR_CITY_OVERVIEW);
				}
				for(;itemCount<cost.foodCost + cost.shieldCost; itemCount++){
					Graphics::Civ::drawShield(theRenderer, x+(itemCount%2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), y+(itemCount/2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), RESOURCES_SCALEFACTOR_CITY_OVERVIEW);
				}
				for(;itemCount<cost.foodCost+cost.shieldCost+cost.unhappyFaces; itemCount++){
					Graphics::Civ::drawUnhappyFace(theRenderer, x+(itemCount%2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), y+(itemCount/2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), RESOURCES_SCALEFACTOR_CITY_OVERVIEW);
				}
			}
		}
	}
}

void CitySurface::m_drawSubsurfaceButtons(){
	for(int i(0); i<4;i++){
		SDL_SetRenderDrawColor(theRenderer, (i==(int) m_subsurface->m_state) ? Graphics::Civ::resourcesWhiteColor() : Graphics::Civ::brightCityBackgroundColor());
		SDL_Rect buttonRect{FOOD_STORAGE_WIDTH + i*(SUBSURFACE_WIDTH/4),SCREEN_HEIGHT - SUBSURFACE_HEIGHT,SUBSURFACE_WIDTH/4,SUBSURFACE_BUTTON_HEIGHT};
		std::cout<<SDL_RenderFillRect(theRenderer, &buttonRect)<<std::endl;
		std::cout<<"buttonRect.x = "<<buttonRect.x<<", buttonRect.y = "<<buttonRect.y<<", buttonRect.w = "<<buttonRect.w<<", buttonRect.h = "<<buttonRect.h<<std::endl;
		std::string text;
		switch(i){
		case (int)SUBSURFACE_INFO:{text="INFO"; break;}
		case (int)SUBSURFACE_MAP:{text="MAP";break;}
		case (int)SUBSURFACE_HAPPY:{text="HAPPY";break;}
		case (int)SUBSURFACE_VIEW: {text="VIEW";break;}
		}
		SDL_Surface* textSurface = TTF_RenderText_Solid(theFont, text.c_str(),Graphics::Civ::cityBackgroundColor());
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(theRenderer, textSurface);
		SDL_Rect textRect{buttonRect.x+(buttonRect.w-textSurface->w)/2,buttonRect.y+(buttonRect.h-textSurface->h)/2,textSurface->w,textSurface->h};
		SDL_RenderCopy(theRenderer, textTexture, nullptr, &textRect);
	}
	SDL_RenderPresent(theRenderer);
	SDL_Delay(1000);
}

void Subsurface::m_draw(){
	switch(m_state){
	case SUBSURFACE_INFO:{
		m_drawFigures();
		break;
	}
	case SUBSURFACE_HAPPY:{
		m_drawHappy();
	}
	}
}

void Subsurface::m_drawFigures(){
	int figuresDrawn = 0;;
	int xToStart = FOOD_STORAGE_WIDTH;
	int figuresPerRow = SUBSURFACE_WIDTH/STANDARD_FIELD_SIZE;
	int yToStart = SCREEN_HEIGHT - SUBSURFACE_HEIGHT + SUBSURFACE_BUTTON_HEIGHT + 2;
	for(std::shared_ptr<Figure> currentFigure: m_surface->m_associatedCity->m_WhereItStands()->m_FiguresOnField()){
		int x = xToStart + STANDARD_FIELD_SIZE*(figuresDrawn%figuresPerRow);
		int y = yToStart + figuresDrawn*STANDARD_FIELD_SIZE*(figuresDrawn/figuresPerRow);
		currentFigure->m_drawFigureSomewhere(x,y);
		figuresDrawn++;
		std::cout<<"x = "<<x<<", y = "<<y<<", figuresDrawn = "<<figuresDrawn<<std::endl;
	}
}

void Subsurface::m_drawHappy(){
	HappyVectorType flag[]={HAPPY_1,HAPPY_2,HAPPY_3,HAPPY_4,HAPPY_ALL};
	int xToStart = FOOD_STORAGE_WIDTH;
	int yToStart = SCREEN_HEIGHT - SUBSURFACE_HEIGHT + SUBSURFACE_BUTTON_HEIGHT + 2;
	for(int i(0); i<5;i++){
		m_surface->m_associatedCity->m_applyCitizenStateVector(flag[i]);
		m_surface->m_drawCitizens(theRenderer, xToStart, yToStart+CITIZENS_OVERVIEW_HEIGHT*i, SUBSURFACE_WIDTH);
	}
}
