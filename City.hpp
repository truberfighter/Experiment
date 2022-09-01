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
#include "CitySurface.hpp"
//#include "Nation.hpp"
//#include "Field.hpp"

class Field;
class Nation;
class Improvement;
class TradeRoute;
class City;
class Subsurface;
class CitySurface;

struct CityProduction{
	int luxuries, gold, science;
};

class SellException{
public:
	ImprovementType m_what;
};

class CityImprovement{
public:
	CityImprovement(ImprovementType whart, City* home);
	ImprovementType m_what;
	City* m_home;
};

struct LayerString{
	int layer;
	std::string content;
};

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

class Subsurface{
public:
	CitySurface* m_surface;
	Subsurface(CitySurface* surface);
	SubSurfaceState m_state = SUBSURFACE_INFO;
	void m_draw();
	void m_drawFigures();
	void m_drawHappy();
	bool m_handleMouseClick(int x, int y);
};

class City: public std::enable_shared_from_this<City> {
public:

private:
	std::vector<CityImprovement> m_improvements;
	ImprovementType m_whatIsBuilt = IMPROVEMENT_SETTLERS;
	std::shared_ptr<Nation> m_owningNation;
	std::shared_ptr<Field> m_whereItStands;
	std::string m_name;
	std::vector<Citizen> m_citizens;
	std::list<std::shared_ptr<Figure>> m_figuresOwned;
	int m_shields = 0;
	int m_food = 0;
	std::vector<std::shared_ptr<TradeRoute>> m_tradeRoutes;

public:
	static std::string improvementString(ImprovementType imptype);
	static std::vector<ImprovementType> buildingTypes();
	static std::vector<ImprovementType> figureTypes();
	static std::vector<ImprovementType> wonderTypes();
	static int shieldsNeeded(ImprovementType imptype);
	static bool isWonderType(ImprovementType imptype);
	bool m_contains(ImprovementType imptype);
	std::vector<ImprovementType> m_whatCanBeBuilt();
	ImprovementType m_WhatIsBuilt(){return m_whatIsBuilt;}
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
	CitySurface m_createCitySurface();
	int m_foodStorageSize();
	std::vector<CitizenState> m_applyCitizenStateVector(HappyVectorType flag = HAPPY_ALL);
	int m_foodCost();
	int m_shieldCost();
	std::vector<UnitCostingResources> m_unitCostVector();
	int m_size();
	int m_luxuriesRevenue();
	int m_goldRevenue();
	int m_scienceRevenue();
	int m_foodProduction();
	int m_shieldProduction();
	int m_tradeProduction();
	int m_corruptionProduction();
	CityProduction m_revenueProduction();
	void m_startNewTurn();
	void m_sortFiguresByValue();
	bool m_maybeFinishProduction();
	void m_grow();
	std::shared_ptr<CityImprovement> m_maybeBuild(ImprovementType imptype);
	bool m_placeCitizen(std::shared_ptr<Field> fieldClickedOn);
	int m_distanceTo(std::shared_ptr<City> comparedCity);
	bool m_sell(int index);
};

template<typename T>
bool isInVector(std::vector<T>& theVector, T& whatToFind, bool (*equals) (T& t1, T& t2));

#endif /* CITY_HPP_ */
