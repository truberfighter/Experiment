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
#include "Nation.hpp"
//#include "Nation.hpp"
//#include "Field.hpp"

class Field;
class Nation;
class Improvement;
class TradeRoute{
public:
	std::shared_ptr<City> m_city1, m_city2;
	TradeRoute(std::shared_ptr<City> city1, std::shared_ptr<City> city2):m_city1(city1),m_city2(city2){}
	int m_calculateProduction();
	void m_destroyItself();
};
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
	std::function<void(City* city)> whenBuilt = [](City*){};
	std::function<void(City* city)> whenDestroyed = [](City*){};
};

class NoGivenData{
public:
	std::string m_what;
	NoGivenData(std::string s):m_what(s){}
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
	~Citizen();
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
	bool m_buyInTurn = false;
	std::vector<CityImprovement> m_improvements;
	ImprovementType m_whatIsBuilt = IMPROVEMENT_SETTLERS;
	std::shared_ptr<Nation> m_owningNation;
	std::shared_ptr<Field> m_whereItStands;
	std::string m_name;
	std::vector<std::shared_ptr<Citizen>> m_citizens;
	std::list<std::shared_ptr<Figure>> m_figuresOwned;
	int m_shields = 0;
	int m_food = 0;
	std::vector<std::shared_ptr<TradeRoute>> m_tradeRoutes;
	void m_cityEconomy();

public:
	float m_goldCoefficient();
	float m_scienceCoefficient();
	float m_luxuryCoefficient();
	int m_contentByImprovement();
	int m_templeValue();
	bool m_makeVeteran();
	float m_productionCoefficient();
	int m_wonderHappy = 0;
	int m_wonderContent = 0;
	int m_unhappyPerUnit();
	float m_industrialPollutionCoefficient;
	float m_populationPollutionCoefficient();
	static std::function<void(City*)> m_whenDestroyed(ImprovementType imptype);
	static std::function<void(City*)> m_whenBuilt(ImprovementType imptype);
	static int shieldsNeeded(ImprovementType imptype);
	static int figureWidth(FigureType figtype);
	static int figureHeight(FigureType figtype);
	static int maintenanceNeeded(ImprovementType imptype);
	static bool isFigureType(ImprovementType imptype);
	static bool isBuildingType(ImprovementType imptype);
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
	bool m_isCivilDisorder();
	int m_shieldCost();
	int m_Shields(){return m_shields;}
	std::vector<UnitCostingResources> m_unitCostVector();
	int m_buyingPrice(ImprovementType imptype);
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
	bool m_sell(int index);
	void m_buy(int price);
	void m_announceCannotMaintain(ImprovementType imptype);
	const std::vector<CityImprovement>& m_Improvements(){return m_improvements;}
	void m_shrink();
	int m_revoltingCost();
	int m_capturingValue();
	friend class Settlers;
	friend void Nation::m_destroyCity(std::shared_ptr<City> cityToDestroy);
	friend void TradeRoute::m_destroyItself();
	friend void Nation::m_captureCity(std::shared_ptr<City>);
};

template<typename T>
bool isInVector(std::vector<T>& theVector, T& whatToFind, bool (*equals) (T& t1, T& t2));

#endif /* CITY_HPP_ */
