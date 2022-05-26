/*
 * City.hpp
 *
 *  Created on: 11.05.2022
 *      Author: uwe-w
 */

#ifndef CITY_HPP_
#define CITY_HPP_

#include <SDL2\SDL.h>
#include <iostream>
#include <SDL2\SDL_image.h>
#include "sdltypes.hpp"
#include <functional>
#include <list>
#include "movableThing.hpp"
#include <memory>
//#include "Nation.hpp"
//#include "Field.hpp"

class Field;
class Nation;
class Improvement;
class TradeRoute;
class City;

struct UnitCostingResources{
	Figure* figure;
	int foodCost;
	int shieldCost;
	int unhappyFaces = 0;
};

class Citizen{
public:
	Citizen(City& home, std::shared_ptr<Field> whereToWork = nullptr);
	std::shared_ptr<Field> m_whereItWorks;
	CitizenState m_state;
	City& m_home;
	CitizenState m_changeState();

};

class CitySurface{
private:
	City* m_associatedCity;
public:
	CitySurface(City* city);
	void m_displaySurface(SDL_Renderer* renderer);
	void m_drawCitizens(SDL_Renderer* renderer, int x, int y);
	void m_drawFoodProduction();
	void m_drawShieldProduction();
	void m_drawTradeProduction();
	void m_drawProduction();
};

class City: public std::enable_shared_from_this<City> {
public:

private:
	std::shared_ptr<Nation> m_owningNation;
	std::shared_ptr<Field> m_whereItStands;
	std::string m_name;
	std::vector<Citizen> m_citizens;
	std::list<std::shared_ptr<Figure>> m_figuresOwned;
	unsigned int m_shields = 0;
	unsigned int m_food = 0;
	std::vector<std::shared_ptr<TradeRoute>> m_tradeRoutes;

public:
	std::shared_ptr<Field> m_WhereItStands(){return m_whereItStands;}
	std::shared_ptr<Nation> m_OwningNation(){return m_owningNation;}
	bool m_takeFigure(std::shared_ptr<Figure> figureToTake);
	bool m_releaseFigure(std::shared_ptr<Figure> figureToRelease);
	City(std::shared_ptr<Field> whereToPlace, std::shared_ptr<Nation> owningNation, std::string name);
	virtual ~City();
	int m_drawCity(int x, int y, SDL_Renderer* renderer = theRenderer);
	int m_drawCityName(int x, int y,SDL_Renderer* renderer);
	std::string m_Name(){return this->m_name;}
	friend class CitySurface;
	CitySurface m_createCitySurface(){return CitySurface(this);}
	int m_foodStorageSize();
	std::vector<CitizenState> m_applyCitizenStateVector();
	int m_foodCost();
	int m_shieldCost();
	std::vector<UnitCostingResources> m_unitCostVector();
	int m_size();
	int m_foodProduction();
	int m_shieldProduction();
	int m_tradeProduction();
	int m_corruptionProduction();
	int m_goldProduction();
	int m_sciencdProduction();
	int m_luxuriesPRoduction();
};

#endif /* CITY_HPP_ */
