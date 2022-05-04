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
#include <exception>
#include <stdexcept>
#include "LetterTextureContainer.hpp"


class Field;
class Nation;
class City;
class FieldContainer;
class Drawing;

class Figure: public std::enable_shared_from_this<Figure> {
protected:
	std::shared_ptr<Field> m_whereItStands;
	std::shared_ptr<City> m_home;
	std::shared_ptr<Nation> m_nationality;
	std::shared_ptr<MovableThing> m_image;
	MovementPoints m_movementPoints;
	bool m_isVeteran = false;
	FigureState m_figureState =DONE_WITH_TURN;
	MovementPoints m_calculateMoveCostGround(Direction whereToGo);
	MovementPoints m_calculateMoveCost(Direction whereToGo);
	MovementPoints m_calculateStandardMoveCostGround(Field& fieldToVisit);
	int m_drawFigureState(int, int, SDL_Renderer*);
public:
	Nationality m_Nationality();
	Figure(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	virtual ~Figure();
	virtual FigureType m_FigureType() = 0;
	void m_move(Direction whereToGo);
	virtual bool m_takeOrder(char order)=0;
	virtual std::string m_orderOverview()=0;
	std::string m_figureOverview();
	void m_resetMovementPoints();
	bool m_tryMoveToField(Direction whereToGo);
	Field& m_WhereItStands();
	MovementPoints m_MovementPoints();
	std::shared_ptr<MovableThing> m_Image();
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
	bool m_initImage();
	virtual std::shared_ptr<MovableThing> m_createImage() =0;
	void m_setInstructionsForDrawingElement();
	void m_integrateInto(Drawing& drawing);
	void m_showTheFigureIsCurrentFigure();
	void m_wait();
	void m_drawFigure(BlinkingState blinkingState = VISIBLE());
	void m_drawFigureSomewhere(int row, int column);
	bool m_IsVeteran();
	std::shared_ptr<Nation> m_Nation(){return m_nationality;}
};

inline bool Figure::m_IsVeteran(){return m_isVeteran;}

class TurnEndsTooEarly{
	public: std::string what() {return "Turn ends too early";}
};

#define STRENGTH_OF float
#define WHEN_ATTACKING(A) ::m_attackingStrength() {return A;}
#define WHEN_DEFENDING(A) ::m_defensiveStrength() {return A;}
#define VISIBILITY_OF short unsigned int
#define IS_NORMAL_VISIBILITY ::m_visibilityRange(){return 1;}
#define WEAK 1/1000000
#define THE_END_OF_TURN_FOR void
#define ONE_MOVEMENT_POINT 3
#define GOES_UNEVENTFUL ::m_finishMove(){std::cout<<"m_finishMove(), x = "<<m_image->getPosition().x<<", y = "<<m_image->getPosition().y<<std::endl;\
	m_figureState = DONE_WITH_TURN; \
	m_nationality->m_removeFromQueue(shared_from_this());\
return;}
#define DEFAULT_MOVING_POINTS(CLASS, DEFAULT) short unsigned int CLASS::m_defaultMovementPoints(){return DEFAULT*3;}
//Man beachte, dass zwecks Ermöglichung von Drittelpunkten in Dreierschritten gezählt wird.
#define NORMAL_MOVING(CLASS) DEFAULT_MOVING_POINTS(CLASS, 1)
#endif /* FIGURE_HPP_ */
