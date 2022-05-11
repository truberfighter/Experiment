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

class Citizen{
public:
	Citizen(std::shared_ptr<City> home, std::shared_ptr<Field> whereToWork = nullptr);
	std::shared_ptr<Field> m_whereItWorks;
	CitizenState m_state;
	std::shared_ptr<City> m_home;
	CitizenState m_changeState();
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
	City(std::shared_ptr<Field> whereToPlace, std::shared_ptr<Nation> owningNation, std::string name);
	virtual ~City();

};

#endif /* CITY_HPP_ */
