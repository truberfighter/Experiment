/*
 * Figure.hpp
 *
 *  Created on: 14.03.2022
 *      Author: uwe-w
 */

#ifndef FIGURE_HPP_
#define FIGURE_HPP_

#include "sdltypes.hpp"
#include "movableThing.hpp"
#include "FieldContainer.hpp"

class Field;
class Nation;
class City;

class Figure: public std::enable_shared_from_this<Figure> {
protected:
	std::shared_ptr<Nation> m_nationality;
	std::shared_ptr<City> m_home;
	Field& m_whereItStands;
	std::shared_ptr<MovableThing> m_image;
	short unsigned int m_movementPoints;
	bool m_isVeteran = false;
	FigureState m_figureState =MOVING;
public:
	Figure(std::shared_ptr<MovableThing>& image, Field& whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	virtual ~Figure();
	virtual bool m_takeOrder(char order)=0;
	virtual std::string m_orderOverview()=0;
	void m_resetMovementPoints();
	bool m_tryMoveToField(Direction whereToGo);
	Field& m_WhereItStands();
	short unsigned int m_MovementPoints();
	virtual float m_attackingStrength()=0;
	virtual float m_defensiveStrength()=0;
	virtual short unsigned int m_visibilityRange() = 0;
	virtual void m_finishMove()=0;
	bool m_Pillage();
	bool m_sentry();
	bool m_fortify();
	virtual short unsigned int m_defaultMovementPoints()=0;
	bool m_explore();
	virtual FigureCategory m_FigureCategory()=0;
	FigureState m_FigureState();
	bool m_startMove();
};



#endif /* FIGURE_HPP_ */
