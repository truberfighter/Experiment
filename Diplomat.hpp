/*
 * Diplomat.hpp
 *
 *  Created on: 17.09.2022
 *      Author: uwe-w
 */

#ifndef DIPLOMAT_HPP_
#define DIPLOMAT_HPP_
#include "Figure.hpp"
#include "Field.hpp"

class FinishMove{};
class DiplomatDead{

};

#endif /* DIPLOMAT_HPP_ */

class Diplomat: public Figure{
public:
	virtual FigureType m_FigureType() override;
	Diplomat(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	virtual ~Diplomat();
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
	bool m_doDiplomatThings(Field& fieldWhereToGo);
	bool m_enterCity(std::shared_ptr<City> city);
	bool m_approachEnemy(std::shared_ptr<Figure> target);
};
