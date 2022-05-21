/*
 * City.cpp
 *
 *  Created on: 11.05.2022
 *      Author: uwe-w
 */

#include "City.hpp"
#include "Settlers.hpp"
#include "Field.hpp"
#include "Drawing.hpp"
#include "Game.hpp"

City::~City() {
	// TODO Auto-generated destructor stub
}

int drawCity(int x, int y, SDL_Renderer* renderer){
	fieldToDraw->m_CityContained()->m_drawCity(x, y, renderer);
}

City::City(std::shared_ptr<Field> whereToPlace, std::shared_ptr<Nation> owningNation, std::string name)
: m_owningNation(owningNation), m_whereItStands(whereToPlace), m_name(name)
{
	//Imprecise. But I do not care that much about limiting the cities right now.
	m_whereItStands->m_DrawingElement()->m_climbToTop(CITY_LAYER);
	if(m_owningNation->m_Cities().size()>=CITIES_PER_NATION){
		throw(TooManyCities());
	}
	m_citizens.push_back(Citizen(*this,m_whereItStands->m_getNeighbouringField(UP)));
	/*SDL_Surface* surface = SDL_CreateRGBSurface(0, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE, 8,0,0,0,0);
	std::cout<<"SDL_Error: "<<SDL_GetError()<<std::endl;
int i = m_citizens.size();
	std::stringstream stream;
	stream<<i; stream.flush();
	SDL_Surface* textSurface = TTF_RenderText_Solid(theFont, stream.str().c_str(), brownColor);
	std::cout<<"SDL_Error: "<<SDL_GetError()<<std::endl;
SDL_Rect r{0,0,surface->w, surface->h};
	SDL_BlitSurface(textSurface, nullptr, surface, nullptr);
std::cout<<"SDL_Errorstr: "<<stream.str()<<std::endl;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(theRenderer, surface);
	std::shared_ptr<Texture> s = std::make_shared<Texture>(texture, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE);
	whereToPlace->m_DrawingElement()->setTexture(s);
	SDL_RenderCopy(theRenderer, s.get()->theTexture(), NULL, NULL);
    SDL_RenderPresent(theRenderer);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(textSurface);
	*/
	std::cout<<"City-Konstruktor"<<std::endl;
	m_whereItStands->m_DrawingElement()->m_setAdditionalInstructions(drawCity);
}

Citizen::	Citizen(City& home, std::shared_ptr<Field> whereToWork):m_home(home)
{

}

int City::m_drawCity(int x, int y, SDL_Renderer* renderer){
	int whatToReturn = 0;
	int m_citizensSize = m_citizens.size();
	std::stringstream stream;
	stream<<m_citizensSize;
	stream.flush();
	SDL_Color newRenderColor = Nation::standardNationColor(m_owningNation->m_Nation());

	SDL_Surface* textSurface = TTF_RenderText_Solid(citySizeFont, stream.str().c_str(), blackColor);
	SDL_Rect rectToFill{x,y,STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE};
	whatToReturn += SDL_SetRenderDrawColor(theRenderer, newRenderColor.r, newRenderColor.g, newRenderColor.b, newRenderColor.a);
	whatToReturn += SDL_RenderFillRect(theRenderer, &rectToFill);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(theRenderer, textSurface);
	SDL_FreeSurface(textSurface);
	whatToReturn += SDL_RenderCopy(theRenderer, texture, NULL, &rectToFill);
	SDL_DestroyTexture(texture);
	m_drawCityName(x,y,renderer);
	if(m_whereItStands->m_FiguresOnField().empty()){
		return whatToReturn;
	}
	whatToReturn += Graphics::drawThickerHorizontalLine(renderer, x, y, STANDARD_FIELD_SIZE/12);
	whatToReturn += Graphics::drawThickerHorizontalLine(renderer, x, y + 11*STANDARD_FIELD_SIZE/12, STANDARD_FIELD_SIZE/12);
	whatToReturn += Graphics::drawThickerVerticalLine(renderer, x, y, STANDARD_FIELD_SIZE/12);
	whatToReturn += Graphics::drawThickerVerticalLine(renderer, x + 11*STANDARD_FIELD_SIZE/12, y, STANDARD_FIELD_SIZE/12);
	if(whatToReturn != 0) std::cout<<"SDL_Error in drawCity: "<<SDL_GetError()<<std::endl;
	return whatToReturn;
}
int City::m_drawCityName(int x, int y,SDL_Renderer* renderer){
	int whatToReturn(0);
	SDL_Color colorToUse = Graphics::Civ::cityOccupiedColor();
	SDL_SetRenderDrawColor(theRenderer, colorToUse.r, colorToUse.g,colorToUse.b,colorToUse.a);
	SDL_Surface* textSurface = TTF_RenderText_Solid(theFont, m_name.c_str(), Graphics::Civ::cityNameColor());
	SDL_Rect rectToFill{x - textSurface->w/2,y+STANDARD_FIELD_SIZE,textSurface->w, textSurface->h};
	SDL_Texture* texture = SDL_CreateTextureFromSurface(theRenderer, textSurface);
	SDL_FreeSurface(textSurface);
	whatToReturn += SDL_RenderCopy(theRenderer, texture, NULL, &rectToFill);
	SDL_DestroyTexture(texture);
	return whatToReturn;
}

bool City::m_releaseFigure(std::shared_ptr<Figure> figureToRelease){
	int sizeBefore = m_figuresOwned.size();
	m_figuresOwned.remove(figureToRelease);
	int sizeAfter = m_figuresOwned.size();
	if(sizeAfter - sizeBefore != -1){
		std::cout<<"City::m_releaseFigureFail: sizeBefore = "<<sizeBefore<<", sizeAfter = "<<sizeAfter<<std::endl;
		return false;
	}
	return true;
}

bool City::m_takeFigure(std::shared_ptr<Figure> figureToTake){
	int sizeBefore = m_figuresOwned.size();
	m_figuresOwned.push_back(figureToTake);
	m_figuresOwned.sort([](std::shared_ptr<Figure> figure1, std::shared_ptr<Figure> figure2)->bool{return figure1->m_shieldCost()<=figure2->m_shieldCost();});
	m_figuresOwned.unique();
	int sizeAfter = m_figuresOwned.size();
	if(sizeAfter - sizeBefore != 1){
		std::cout<<"City::m_takeFigureFail: sizeBefore = "<<sizeBefore<<", sizeAfter = "<<sizeAfter<<std::endl;
		return false;
	}
	return true;
}

