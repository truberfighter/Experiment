/*
 * NormalUnit.hpp
 *
 *  Created on: 14.09.2022
 *      Author: uwe-w
 */

#include "sdltypes.hpp"
#include "Figure.hpp"
class Nation;
class City;

#ifndef NORMALUNIT_HPP_
#define NORMALUNIT_HPP_

class NormalUnit: public Figure{
private:
public:
	virtual FigureType m_FigureType() override = 0;
	NormalUnit(){}
	NormalUnit(Field* whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	virtual ~NormalUnit();
	bool m_takeOrder(char order);
	virtual std::string m_orderOverview();
	virtual float m_attackingStrength() = 0;
	virtual float m_defensiveStrength() = 0;
	virtual short unsigned int m_visibilityRange() = 0;
	void m_finishMove() override;
	virtual short unsigned int m_defaultMovementPoints() = 0;
	FigureCategory m_FigureCategory()override;
	std::shared_ptr<MovableThing> m_createImage() override = 0;
	virtual int m_shieldCost() override = 0;
};



#endif /* NORMALUNIT_HPP_ */
