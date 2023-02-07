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
	CLASS(){}\
	~CLASS();\
	float m_attackingStrength();\
	float m_defensiveStrength();\
	int m_shieldCost();\
	short unsigned int m_visibilityRange();\
	short unsigned int m_defaultMovementPoints();\
	std::shared_ptr<MovableThing> m_createImage() override;\
	short unsigned int m_cargoCountMax();\
};

SHIP_CLASS(Trireme)
SHIP_CLASS(Carrier)
#define DECONSTRUCTOR_SHIP(CLASS)CLASS::~CLASS(){std::cout<<m_FigureType()<<"-Destruktor, this = "<<this<<std::endl;}\
\
CLASS::CLASS(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)\
:Ship(whereToStart, nationality, home, isVeteran)\
{\
	m_resetMovementPoints();\
	if(!m_initImage()) cout<<"Fatal error: MovableThing for "<<m_FigureType()<<" not created"<<endl;\
	cout<<m_FigureType()<<"konstruktor"<<this<<endl;\
}

#endif /* TRIREME_HPP_ */
