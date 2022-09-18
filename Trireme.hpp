/*
 * Trireme.hpp
 *
 *  Created on: 07.05.2022
 *      Author: uwe-w
 */

#ifndef TRIREME_HPP_
#define TRIREME_HPP_

#include "sdltypes.hpp"
#include "Figure.hpp"
#include "Ship.hpp"

class Nation;
class City;

#define SHIP_CLASS(CLASS) class CLASS: public Ship{\
private:\
public:\
	FigureType m_FigureType() override;\
	CLASS(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);\
	~CLASS();\
	float m_attackingStrength();\
	float m_defensiveStrength();\
	int m_shieldCost();\
	short unsigned int m_visibilityRange();\
	short unsigned int m_defaultMovementPoints();\
	std::shared_ptr<MovableThing> m_createImage() override;\
	short unsigned int m_cargoCountMax();\
};




#endif /* TRIREME_HPP_ */
