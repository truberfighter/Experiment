/*
 * CitySurface.cpp
 *
 *  Created on: 24.05.2022
 *      Author: uwe-w
 */

#include "Field.hpp"
#include "Drawing.hpp"
#include "City.hpp"
#include "CitySurface.hpp"
#include <sstream>
#include "SelectorSurface.hpp"
int selectionElementGlobalIndex = 0;
class IrrelevantClick{

};

ButtonElement changeButton(SCREEN_WIDTH - SHIELD_OVERVIEW_WIDTH, SCREEN_HEIGHT - SHIELD_OVERVIEW_HEIGHT + NORMAL_FONT_HEIGHT, CHANGEBUTTON_WIDTH, SUBSURFACE_BUTTON_HEIGHT, Graphics::Civ::brightCityBackgroundColor(), Graphics::Civ::cityBackgroundColor(), "CHANGE");
ButtonElement buyButton(SCREEN_WIDTH - BUYBUTTON_WIDTH, SCREEN_HEIGHT - SHIELD_OVERVIEW_HEIGHT + NORMAL_FONT_HEIGHT, BUYBUTTON_WIDTH, SUBSURFACE_BUTTON_HEIGHT, Graphics::Civ::brightCityBackgroundColor(), Graphics::Civ::cityBackgroundColor(), "BUY");


CitySurface::CitySurface(City* city): m_associatedCity(city)
{
	m_subsurface = std::make_shared<Subsurface>(this);

}

Subsurface::Subsurface(CitySurface* surface):m_surface(surface)
{
	m_state = SUBSURFACE_HAPPY;
}

void CitySurface::m_drawCitizens(SDL_Renderer* renderer, int x, int y, int backgroundWidth, int backgroundHeight, HappyVectorType flag){
	SDL_Color backgroundColor (Graphics::Civ::cityBackgroundColor());
	SDL_SetRenderDrawColor(theRenderer, backgroundColor);
	SDL_Rect backgroundRect{x,y,backgroundWidth, backgroundHeight};
	SDL_RenderFillRect(theRenderer, &backgroundRect);
	unsigned int currentX = x + FIRST_CITIZEN_X;
	unsigned int yToDraw = y + backgroundHeight - CITIZEN_HEIGHT*CITIZEN_SCALEFACTOR;
	bool female = false;
	std::vector<CitizenState> citizenVector = m_associatedCity->m_applyCitizenStateVector(flag);
	CitizenState currentState;
	for(unsigned int i(0); i<citizenVector.size(); i++){
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
		SDL_Rect rectToFill{(int) currentX, (int) yToDraw, CITIZEN_SCALEFACTOR*(currentState == ENTERTAINER ? 8 : 7), CITIZEN_SCALEFACTOR*CITIZEN_HEIGHT};
		SDL_RenderCopy(theRenderer, textureToDraw, NULL, &rectToFill);
		currentX+=CITIZEN_MAX_WIDTH*CITIZEN_SCALEFACTOR;
		SDL_DestroyTexture(textureToDraw);
	}
}

void CitySurface::m_displaySurface(SDL_Renderer* renderer){
	m_createSellingButtonElements();
	m_drawSurface(theRenderer);
	std::cout<<"displaySurface"<<std::endl;
	SDL_Event e;
	bool quitSurface = false;
	while(!quitSurface){
		try{
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				throw SDLQuitException();
			}
			if(m_handleEvent(e)){
				m_drawSurface(theRenderer);
			}
		}
		}
		catch(QuitSurface &qs){
			quitSurface = true;
		}
		catch(SDLQuitException& sdlqe){
			throw sdlqe;
		}
	}
	std::cout<<"displaySurfaceEnd"<<std::endl;
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
		for(std::shared_ptr<Citizen>& currentCitizen: m_associatedCity->m_citizens){
			if(currentCitizen->m_whereItWorks == fieldVector[i]){
				drawProduction = true;
			}
		}
		fieldVector[i]->m_DrawingElement()->m_drawAt(xToStart + (2+coordinateVector[i].x)*STANDARD_FIELD_SIZE, yToStart+(2+coordinateVector[i].y)*STANDARD_FIELD_SIZE, theRenderer);
		if(!fieldVector[i]->m_FiguresOnField().empty() && fieldVector[i]->m_FiguresOnField().front()->m_Nationality()!=m_associatedCity->m_OwningNation()->m_Nation() && fieldVector[i]->m_isVisible(m_associatedCity->m_OwningNation()->m_Nation())){
			fieldVector[i]->m_FiguresOnField().front()->m_drawFigureSomewhere(xToStart + (2+coordinateVector[i].x)*STANDARD_FIELD_SIZE, yToStart+(2+coordinateVector[i].y)*STANDARD_FIELD_SIZE);
		}
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
		int distance = 8*2/iconsPerRow;
		for(int j(0); j<fieldVector[i]->m_food(nation); j++){
			Graphics::Civ::drawFood(theRenderer, fieldX + (productionIconIndex % iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*distance, fieldY + (productionIconIndex / iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*8, RESOURCES_SCALEFACTOR_CITY_OVERVIEW, false);
			productionIconIndex++;
		}
		for(int j(0); j<fieldVector[i]->m_shields(nation); j++){
			Graphics::Civ::drawShield(theRenderer, fieldX + (productionIconIndex % iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*distance, fieldY + (productionIconIndex / iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*8, RESOURCES_SCALEFACTOR_CITY_OVERVIEW);
			productionIconIndex++;
		}
		for(int j(0); j<fieldVector[i]->m_trade(nation); j++){
			Graphics::Civ::drawTrade(theRenderer, fieldX + (productionIconIndex % iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*distance, fieldY + (productionIconIndex / iconsPerRow)*RESOURCES_SCALEFACTOR_CITY_OVERVIEW*8, RESOURCES_SCALEFACTOR_CITY_OVERVIEW, false);
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
			std::cout<<"figure: "<<cost.figure<<", foodCost: "<<cost.foodCost<<", shieldCost = "<<cost.shieldCost<<", unhappyFaces: "<<cost.unhappyFaces<<", unitCostVector.sizw() = "<<costVector.size()<<std::endl;
			if(cost.figure!=nullptr){
				std::cout<<"figure keine nullptr"<<std::endl;
				int itemCount = 0;
				for(;itemCount<cost.foodCost;itemCount++){
					std::cout<<"drawshield"<<cost.foodCost<<", "<<cost.shieldCost<<std::endl;
					Graphics::Civ::drawFood(theRenderer, x+(itemCount%2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), y+(itemCount/2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), RESOURCES_SCALEFACTOR_CITY_OVERVIEW);
				}
				for(itemCount = cost.foodCost;itemCount<cost.foodCost + cost.shieldCost; itemCount++){
					Graphics::Civ::drawShield(theRenderer, x+(itemCount%2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), y+(itemCount/2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), RESOURCES_SCALEFACTOR_CITY_OVERVIEW);
				}
				for(itemCount = cost.foodCost+cost.shieldCost;itemCount<cost.foodCost+cost.shieldCost+cost.unhappyFaces; itemCount++){
					Graphics::Civ::drawUnhappyFace(theRenderer, x+(itemCount%2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), y+(itemCount/2)*(8*RESOURCES_SCALEFACTOR_CITY_OVERVIEW), RESOURCES_SCALEFACTOR_CITY_OVERVIEW);
				}
			}
			it++;
		}
	}
}

void CitySurface::m_drawSubsurfaceButtons(){
	for(int i(0); i<4;i++){
		SDL_SetRenderDrawColor(theRenderer, (i==(int) m_subsurface->m_state) ? Graphics::Civ::resourcesWhiteColor() : Graphics::Civ::brightCityBackgroundColor());
		SDL_Rect buttonRect{FOOD_STORAGE_WIDTH + i*(SUBSURFACE_WIDTH/4),SCREEN_HEIGHT - SUBSURFACE_HEIGHT,SUBSURFACE_WIDTH/4,SUBSURFACE_BUTTON_HEIGHT};
		SDL_RenderFillRect(theRenderer, &buttonRect);
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
		SDL_DestroyTexture(textTexture);
		SDL_FreeSurface(textSurface);
	}
	SDL_RenderPresent(theRenderer);
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
		//std::cout<<"x = "<<x<<", y = "<<y<<", figuresDrawn = "<<figuresDrawn<<std::endl;
	}
}

void Subsurface::m_drawHappy(){
	HappyVectorType flag[]={HAPPY_1,HAPPY_2,HAPPY_3,HAPPY_4,HAPPY_ALL};
	int xToStart = FOOD_STORAGE_WIDTH;
	int yToStart = SCREEN_HEIGHT - SUBSURFACE_HEIGHT + SUBSURFACE_BUTTON_HEIGHT + 2;
	for(int i(0); i<5;i++){
		m_surface->m_drawCitizens(theRenderer, xToStart, yToStart+((SUBSURFACE_HEIGHT - SUBSURFACE_BUTTON_HEIGHT)/5)*i - 1, SUBSURFACE_WIDTH, (SUBSURFACE_HEIGHT -SUBSURFACE_BUTTON_HEIGHT)/5, flag[i]);
	}
}




void CitySurface::m_drawSurface(SDL_Renderer* renderer){
	SDL_SetRenderDrawColor(theRenderer, brownColor);
	SDL_RenderClear(renderer);
	m_drawFoodProduction();
	m_drawShieldProduction();
	m_drawTradeProduction();
	m_drawFoodStorage();
	m_drawCitizens(renderer, 0,0);
	m_drawCityFields();
	m_drawFigures();
	m_drawSubsurfaceButtons();
	m_drawShields();
	m_drawRevenueProduction();
	m_subsurface->m_draw();
	m_drawImprovementBackground();
	m_drawSellingOverview();
	std::cout<<"shield in surface: "<<m_associatedCity->m_shields<<std::endl;
	SDL_RenderPresent(theRenderer);
}



void CitySurface::m_drawShields(){
	const int shieldsNeeded = City::shieldsNeeded(m_associatedCity->m_whatIsBuilt);
	const int shieldWidth = RESOURCES_SCALEFACTOR*8;
	const int shieldsPerRow = std::max(shieldsNeeded/100,1)*10;
	const int rowCount = shieldsNeeded/shieldsPerRow;
	const int distance = std::max(1*RESOURCES_SCALEFACTOR,std::min(shieldWidth,SHIELD_OVERVIEW_WIDTH/shieldsPerRow));
	SDL_Rect backgroundRect{SCREEN_WIDTH - SHIELD_OVERVIEW_WIDTH, SCREEN_HEIGHT - SHIELD_OVERVIEW_HEIGHT, SHIELD_OVERVIEW_WIDTH, SHIELD_OVERVIEW_HEIGHT};
	SDL_SetRenderDrawColor(theRenderer, Graphics::Civ::cityBackgroundColor());
	SDL_RenderFillRect(theRenderer, &backgroundRect);
	SDL_Rect storageRect{SCREEN_WIDTH-SHIELD_OVERVIEW_WIDTH, SCREEN_HEIGHT-10*8*RESOURCES_SCALEFACTOR, SHIELD_OVERVIEW_WIDTH-shieldWidth, rowCount*RESOURCES_SCALEFACTOR*8};
	SDL_SetRenderDrawColor(theRenderer, Graphics::Civ::brightCityBackgroundColor());
	SDL_RenderFillRect(theRenderer, &storageRect);
	int shieldCount = m_associatedCity->m_Shields();
	for(int shieldIndex(0); shieldIndex<shieldCount; shieldIndex++){
		Graphics::Civ::drawShield(theRenderer, storageRect.x + distance*(shieldIndex%shieldsPerRow), storageRect.y + 8*RESOURCES_SCALEFACTOR*(shieldIndex/shieldsPerRow), RESOURCES_SCALEFACTOR);
	}
	std::stringstream s;
	s<<m_associatedCity->m_WhatIsBuilt();
	s.flush();
	SDL_Surface* infoSurface = TTF_RenderText_Solid(theFont, s.str().c_str(), whiteColor);
	SDL_Texture* infoTexture = SDL_CreateTextureFromSurface(theRenderer, infoSurface);
	int xToStart = SCREEN_WIDTH - SHIELD_OVERVIEW_WIDTH;
	int yToStart = SCREEN_HEIGHT - SHIELD_OVERVIEW_HEIGHT;
	SDL_Rect infoRect{ xToStart + (SHIELD_OVERVIEW_WIDTH - infoSurface->w)/2, yToStart, infoSurface->w, infoSurface->h};
	SDL_RenderCopy(theRenderer, infoTexture, nullptr, &infoRect);
	yToStart+= infoSurface->w;
	SDL_DestroyTexture(infoTexture);
	SDL_FreeSurface(infoSurface);
	buyButton.m_draw(0,0);
	changeButton.m_draw(0,0);
}

bool CitySurface::m_changeWhatIsBuilt(){
	if(m_associatedCity->m_buyInTurn == true){
		return false;
	}
	std::vector<std::shared_ptr<SelectionElement>> whatToSelectFrom;
	std::vector<ImprovementType> whatIsPossible = m_associatedCity->m_whatCanBeBuilt();
	for(ImprovementType imptype: whatIsPossible){
		std::stringstream namestream;
		bool isObsoleteWonder = false;
			if(isObsoleteWonder){
				namestream<<"<<";
			}
			namestream<< imptype;
			namestream.flush();
			//ImprovementRightClick coming!
			std::function<void()> irc = [](){};
			std::shared_ptr<SelectionElement> selectionElement = std::make_shared<SelectionElement>(namestream.str(), ((int) imptype < 0 ? IMPROVEMENT_UNIT_LAYER : IMPROVEMENT_LAYER),irc);
		whatToSelectFrom.push_back(selectionElement);
	}
	SelectorSurface improvementSelection(0,0,whatToSelectFrom);
	SelectionReturn result = improvementSelection.m_fetchSelection();
	m_associatedCity->m_whatIsBuilt = whatIsPossible[result.index];
	std::cout<<result.content<<", index: "<<result.index<<", building: "<<m_associatedCity->m_whatIsBuilt<<std::endl;
	return true;
}

void CitySurface::m_drawTradeProduction(){
	int amount = m_associatedCity->m_tradeProduction();
	int checkInt = 0;
	bool isThereAnyCorruption = false;
	const int tradeWidth = RESOURCES_SCALEFACTOR*7;
	int x = 0;
	const int yToStart = CITIZENS_OVERVIEW_HEIGHT + RESOURCES_TEXT_HEIGHT + 2*RESOURCE_TEXT_SHIELD_DIFFERENCE;
	const int distance = std::max(1*RESOURCES_SCALEFACTOR, std::min(tradeWidth,(int)PRODUCTION_OVERVIEW_WIDTH/std::max(m_associatedCity->m_shieldCost(), m_associatedCity->m_shieldProduction())));
	std::cout<<"before corruption production"<<std::endl;
	for(int i(0); i<amount-m_associatedCity->m_corruptionProduction(); i++){
		isThereAnyCorruption = true;
		checkInt += Graphics::Civ::drawTrade(theRenderer, x = i*distance, yToStart, RESOURCES_SCALEFACTOR, false);
	}
	for(int i(amount-m_associatedCity->m_corruptionProduction()); i<m_associatedCity->m_tradeProduction();i++){
		checkInt +=Graphics::Civ::drawTrade(theRenderer, x = i*distance + (isThereAnyCorruption ? 3*tradeWidth/2 : 0), yToStart, RESOURCES_SCALEFACTOR, true);
	}
	if(checkInt != 0){
		std::cout<<"SDL Error in trade drawing: "<<SDL_GetError()<<std::endl;
	}
}

int zeroOrOneMore(int a){
	if(a<0){
		throw(a);
	}
	if(a==0){
		return 0;
	}
	return a+1;
}

void CitySurface::m_drawRevenueProduction(){
	std::cout<<"drawRevenueProduction: "<<std::endl;
	//draw luxuries at first
	int xToStart = 0;
	const int tradeWidth = RESOURCES_SCALEFACTOR*7;
	int distance = std::max(1*RESOURCES_SCALEFACTOR, std::min(tradeWidth,PRODUCTION_OVERVIEW_WIDTH/(zeroOrOneMore(m_associatedCity->m_luxuriesRevenue())+zeroOrOneMore(m_associatedCity->m_goldRevenue())+ zeroOrOneMore(m_associatedCity->m_scienceRevenue()))));
	const int yToStart = CITIZENS_OVERVIEW_HEIGHT + RESOURCES_TEXT_HEIGHT + 3*RESOURCE_TEXT_SHIELD_DIFFERENCE;
	for(int luxuriesCount(0); luxuriesCount < m_associatedCity->m_luxuriesRevenue();luxuriesCount++){
		Graphics::Civ::drawLuxury(theRenderer, xToStart, yToStart, RESOURCES_SCALEFACTOR);
		xToStart+=distance;
	}
	if(m_associatedCity->m_luxuriesRevenue()>0){
		(xToStart-=distance)+=tradeWidth;
	}
	for(int goldCount(0); goldCount < m_associatedCity->m_goldRevenue();goldCount++){
		Graphics::Civ::drawGold(theRenderer, xToStart, yToStart, RESOURCES_SCALEFACTOR);
		xToStart+=distance;
	}
	if(m_associatedCity->m_goldRevenue()>0){
		(xToStart-=distance)+=tradeWidth;
	}
	for(int scienceCount(0); scienceCount < m_associatedCity->m_scienceRevenue();scienceCount++){
		Graphics::Civ::drawScience(theRenderer, xToStart, yToStart, RESOURCES_SCALEFACTOR);
		xToStart+=distance;
	}
}

void CitySurface::m_createSellingButtonElements(){
	m_sellingElements.clear();
	for(int i(0);i+m_indexForImprovementOverview<std::min((int)m_associatedCity->m_improvements.size(),IMPROVEMENT_OVERVIEW_HEIGHT_NORMED - 1);i++){
		std::stringstream s;
		s<<m_associatedCity->m_improvements[i+m_indexForImprovementOverview].m_what;
		s.flush();
		std::shared_ptr<ButtonElement> whatToAdd = std::make_shared<ButtonElement>(SCREEN_WIDTH-IMPROVEMENT_OVERVIEW_WIDTH, i*STANDARD_TEXT_HEIGHT, IMPROVEMENT_OVERVIEW_WIDTH,
STANDARD_TEXT_HEIGHT, Graphics::Civ::brightCityBackgroundColor(),Graphics::Civ::resourcesWhiteColor(),
s.str());
		m_sellingElements.push_back(whatToAdd);
	}
	if(m_associatedCity->m_improvements.size()>=IMPROVEMENT_OVERVIEW_HEIGHT_NORMED){
		std::shared_ptr<ButtonElement> whatToAdd = std::make_shared<ButtonElement>(SCREEN_WIDTH-IMPROVEMENT_OVERVIEW_WIDTH, IMPROVEMENT_OVERVIEW_HEIGHT_NORMED*STANDARD_TEXT_HEIGHT, IMPROVEMENT_OVERVIEW_WIDTH,
STANDARD_TEXT_HEIGHT, Graphics::Civ::brightCityBackgroundColor(),Graphics::Civ::resourcesWhiteColor(),
std::string("More"));
		m_sellingElements.push_back(whatToAdd);
	}
}

void CitySurface::m_drawSellingOverview(){
	for(std::shared_ptr<ButtonElement>& currentElement: m_sellingElements){
		currentElement->m_draw(0, 0);
	}
}

void CitySurface::m_drawImprovementBackground(){
	SDL_Rect rectToFill{SCREEN_WIDTH - IMPROVEMENT_OVERVIEW_WIDTH,0,IMPROVEMENT_OVERVIEW_WIDTH,STANDARD_TEXT_HEIGHT*IMPROVEMENT_OVERVIEW_HEIGHT_NORMED};
	SDL_SetRenderDrawColor(theRenderer, Graphics::Civ::cityBackgroundColor());
	SDL_RenderFillRect(theRenderer, &rectToFill);
}
