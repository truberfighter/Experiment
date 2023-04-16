/*
 * Plane.hpp
 *
 *  Created on: 18.09.2022
 *      Author: uwe-w
 */

#ifndef PLANE_HPP_
#define PLANE_HPP_
#include "Figure.hpp"

class Plane: public Figure{
protected:
	int m_turnsFlying = 0;
public:
	virtual FigureType m_FigureType() override = 0;
	Plane(){}
	Plane(Field* whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	virtual ~Plane();
	bool m_takeOrder(char order);
	virtual std::string m_orderOverview();
	virtual float m_attackingStrength() = 0;
	virtual float m_defensiveStrength() = 0;
	virtual short unsigned int m_visibilityRange() = 0;
	void m_finishMove() override;
	virtual short unsigned int m_defaultMovementPoints() = 0;
	FigureCategory m_FigureCategory()override;
	std::shared_ptr<MovableThing> m_createImage() override = 0;
	virtual int m_maximumTurnsFlying() = 0;
	virtual int m_shieldCost() override = 0;
	virtual void m_printData(std::ostream& os) override;
	virtual void m_finishWinningAttack(Field* battlefield);
	friend class FigureFactory;
};

#define PLANE(CLASS) class CLASS: public Plane{\
public:\
	virtual FigureType m_FigureType() override;\
	CLASS(){}\
	CLASS(Field* whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);\
	virtual ~CLASS();\
	virtual float m_attackingStrength();\
	virtual float m_defensiveStrength();\
	virtual short unsigned int m_visibilityRange();\
	virtual short unsigned int m_defaultMovementPoints();\
	std::shared_ptr<MovableThing> m_createImage() override;\
	int m_maximumTurnsFlying();\
	int m_shieldCost();\
};
PLANE(Bomber)
PLANE(Fighter)
class Nuclear: public Plane{
public:
	virtual FigureType m_FigureType() override;
	Nuclear(){}
	Nuclear(Field* whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	virtual ~Nuclear();
	virtual float m_attackingStrength();
	virtual float m_defensiveStrength();
	virtual short unsigned int m_visibilityRange();
	virtual short unsigned int m_defaultMovementPoints();
	std::shared_ptr<MovableThing> m_createImage() override;
	int m_maximumTurnsFlying();
	int m_shieldCost();
	virtual void m_finishWinningAttack(Field* battlefield);
	virtual bool m_canBeSeenBy(Figure* figureLooking);
};

#define MAXIMUM_TURNS_FLYING(CLASS,TURNS) int CLASS::m_maximumTurnsFlying(){return TURNS;}
#define DECONSTRUCTOR_PLANE(CLASS)CLASS::~CLASS(){std::cout<<m_FigureType()<<"-Destruktor, this = "<<this<<std::endl;}\
\
CLASS::CLASS(Field* whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)\
:Plane(whereToStart, nationality, home, isVeteran)\
{\
	m_resetMovementPoints();\
	if(!m_initImage()) std::cout<<"Fatal error: MovableThing for "<<m_FigureType()<<" not created"<<std::endl;\
	std::cout<<m_FigureType()<<"konstruktor"<<this<<std::endl;\
}



#endif /* PLANE_HPP_ */
