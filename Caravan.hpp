/*
 * Caravan.hpp
 *
 *  Created on: 07.01.2023
 *      Author: uwe-w
 */

#ifndef CARAVAN_HPP_
#define CARAVAN_HPP_

#include "Figure.hpp"
#include "Field.hpp"

class CaravanDead{

};


class Caravan: public Figure{
public:
	virtual FigureType m_FigureType() override;
	Caravan(){}
	Caravan(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	virtual ~Caravan();
	bool m_takeOrder(char order);
	virtual std::string m_orderOverview();
	virtual float m_attackingStrength();
	virtual float m_defensiveStrength();
	virtual short unsigned int m_visibilityRange();
	void m_finishMove() override;
	virtual short unsigned int m_defaultMovementPoints();
	FigureCategory m_FigureCategory(){return GROUND;}
	std::shared_ptr<MovableThing> m_createImage() override;
	virtual int m_shieldCost() override;
	bool m_doCaravanThings(Field& fieldWhereToGo);
	bool m_enterCity(City& city);
};


#endif /* CARAVAN_HPP_ */
