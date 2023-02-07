/*
 * Artillery.hpp
 *
 *  Created on: 14.09.2022
 *      Author: uwe-w
 */

#ifndef ARTILLERY_HPP_
#define ARTILLERY_HPP_

#include "sdltypes.hpp"
#include "NormalUnit.hpp"
#define GROUND_UNIT(CLASS) class CLASS: public NormalUnit{\
private:\
public:\
	FigureType m_FigureType() override;\
	CLASS(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);\
	CLASS(){}\
	~CLASS();\
	virtual float m_attackingStrength();\
	float m_defensiveStrength();\
	short unsigned int m_visibilityRange();\
	short unsigned int m_defaultMovementPoints();\
	int m_shieldCost();\
	std::shared_ptr<MovableThing> m_createImage() override;\
};

class Nation;
class City;

GROUND_UNIT(Artillery)
GROUND_UNIT(Cavalry)
GROUND_UNIT(Chariot)
GROUND_UNIT(Phalanx)
GROUND_UNIT(Musketeers)
GROUND_UNIT(Riflemen)
GROUND_UNIT(Legion)
GROUND_UNIT(Armor)
GROUND_UNIT(MechInf)
GROUND_UNIT(Cannon)
GROUND_UNIT(Catapult)
GROUND_UNIT(Knights)
GROUND_UNIT(Militia)



















#endif /* ARTILLERY_HPP_ */
