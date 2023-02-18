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
class Diplomat;
struct NationKnowsCity{
	int size;
	bool noUnits;
	bool hasCityWalls;
	int yearNumberRaw;
	Nationality home;
	Nationality whoKnowsThisCity;
};
class TradeRoute{
public:
	std::shared_ptr<City> m_city1, m_city2;
	TradeRoute(std::shared_ptr<City> city1, std::shared_ptr<City> city2):m_city1(city1),m_city2(city2){}
	int m_calculateProduction();
	void m_destroyItself();
	int m_calculateInstantRevenue();
};
class City;
class Subsurface;
class CitySurface;

struct CityProduction{
	int luxuries, gold, science,trade;
}; struct CityJson; struct CitizenJson;

class SellException{
public:
	ImprovementType m_what;
};
class InvalidWhereToWork: public std::exception{
public: const char* m_what() const noexcept {return "Invalid field where to work has been assigned to a citizen!\n";}
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


struct UnitCostingResources{
	Figure* figure;
	int foodCost;
	int shieldCost;
	int unhappyFaces = 0;
};

class Citizen{
public:
	~Citizen();
	Citizen(City& home, Field* whereToWork = nullptr);
	Field* m_whereItWorks;
	CitizenState m_state;
	City& m_home;
	CitizenState m_changeState();
	CitizenJson m_createJson();
};

class Subsurface{
public:
	CitySurface* m_surface;
	Subsurface(CitySurface* surface);
	SubSurfaceState m_state = SUBSURFACE_INFO;
	void m_draw();
	int m_drawFigures();
	void m_drawHappy();
	bool m_handleMouseClick(int x, int y);
};

class City: public std::enable_shared_from_this<City> {
public:

private:
	std::vector<NationKnowsCity> m_nationFogInfo;
	int m_globalIndex;
	bool m_buyInTurn = false;
	std::vector<CityImprovement> m_improvements;
	ImprovementType m_whatIsBuilt = IMPROVEMENT_SETTLERS;
	std::shared_ptr<Nation> m_owningNation;
	Field* m_whereItStands;
	std::string m_name;
	std::vector<std::shared_ptr<Citizen>> m_citizens;
	std::list<std::shared_ptr<Figure>> m_figuresOwned;
	int m_shields = 0;
	int m_food = 0;
	std::vector<std::shared_ptr<TradeRoute>> m_tradeRoutes;
	void m_cityEconomy();
public:
	int m_wonderContent = 0;
	int m_wonderHappy = 0;
	float m_industrialPollutionCoefficient = 0;
	int m_shieldsPerPollution();
	float m_goldCoefficient();
	float m_scienceCoefficient();
	float m_luxuryCoefficient();
	int m_contentByImprovement();
	int m_templeValue();
	bool m_makeVeteran();
	float m_productionCoefficient();
	int m_unhappyPerUnit();
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
	std::vector<std::shared_ptr<TradeRoute>>& m_TradeRoutes(){return m_tradeRoutes;}
	std::vector<std::shared_ptr<Citizen>>& m_Citizens(){return m_citizens;}
	ImprovementType m_WhatIsBuilt(){return m_whatIsBuilt;}
	int m_GlobalIndex(){return m_globalIndex;}
	Field* m_WhereItStands(){return m_whereItStands;}
	std::shared_ptr<Nation> m_OwningNation(){return m_owningNation;}
	bool m_takeFigure(std::shared_ptr<Figure> figureToTake);
	bool m_releaseFigure(std::shared_ptr<Figure> figureToRelease);
	City(Field* whereToPlace, std::shared_ptr<Nation> owningNation, std::string name);
	City(){}
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
	void m_makeVisible(Nationality nationality);
	void m_sortFiguresByValue();
	bool m_maybeFinishProduction();
	void m_grow();
	void m_clearProduction(){m_shields=0;}
	std::shared_ptr<CityImprovement> m_maybeBuild(ImprovementType imptype);
	bool m_placeCitizen(Field* fieldClickedOn);
	bool m_sell(int index);
	void m_buy(int price);
	void m_announceCannotMaintain(ImprovementType imptype);
	const std::vector<CityImprovement>& m_Improvements(){return m_improvements;}
	void m_shrink();
	int m_revoltingCost();
	int m_capturingValue();
	bool m_offerRevolt(Diplomat& diplomat);
	int m_pollutionProduction();
	int m_everydaysPollution();
	bool m_handlePollution();
	void m_initFogInfo();
	CityJson m_createJson();
	friend class Settlers;
	friend void Nation::m_destroyCity(std::shared_ptr<City> cityToDestroy);
	friend void TradeRoute::m_destroyItself();
	friend void Nation::m_captureCity(std::shared_ptr<City>);
	friend class CityFactory; friend class GameLoader;
};

template<typename T>
bool isInVector(std::vector<T>& theVector, const T& whatToFind, bool (*equals) (const T& t1, const T& t2));

#endif /* CITY_HPP_ */
