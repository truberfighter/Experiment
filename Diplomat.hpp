/*
 * Diplomat.hpp
 *
 *  Created on: 17.09.2022
 *      Author: uwe-w
 */

#ifndef DIPLOMAT_HPP_
#define DIPLOMAT_HPP_
#include "Figure.hpp"




#endif /* DIPLOMAT_HPP_ */

class Diplomat: public Figure{
public:
	virtual FigureType m_FigureType() override = 0;
	Diplomat(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	virtual ~Diplomat();
	bool m_takeOrder(char order);
	virtual std::string m_orderOverview();
	virtual float m_attackingStrength() = 0;
	virtual float m_defensiveStrength() = 0;
	virtual short unsigned int m_visibilityRange() = 0;
	void m_finishMove() override;
	virtual short unsigned int m_defaultMovementPoints() = 0;
	FigureCategory m_FigureCategory(){return GROUND;}
	std::shared_ptr<MovableThing> m_createImage() override = 0;
	virtual int m_shieldCost() override = 0;
};
