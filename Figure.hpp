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
#include "Drawing.hpp"
#include <exception>
#include <stdexcept>
#include "LetterTextureContainer.hpp"
#include <sstream>

class Field;
class Nation;
class City;
class FieldContainer;
class Drawing;

class Figure: public std::enable_shared_from_this<Figure> {
protected:
	std::vector<Nationality> m_visibilityInfo;
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
	bool m_isVisible(Nationality nationality);
	void m_makeVisible(Nationality nationality);
	virtual void m_makeVisibleAround(std::shared_ptr<Field> fieldBase = nullptr);
	void m_setFigureState (FigureState newState){m_figureState = newState;}
	Nationality m_Nationality();
	Figure(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	virtual ~Figure();
	virtual FigureType m_FigureType() = 0;
	void m_move(Direction whereToGo);
	std::shared_ptr<City> m_Home(){return m_home;}
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
	bool m_startMove(bool activateSentried = false);
	bool m_initImage();
	virtual std::shared_ptr<MovableThing> m_createImage() =0;
	void m_setInstructionsForDrawingElement();
	void m_integrateInto(Drawing& drawing);
	void m_showTheFigureIsCurrentFigure();
	void m_wait();
	void m_drawFigure(BlinkingState blinkingState = VISIBLE());
	void m_drawFigureSomewhere(int row, int column);
	bool m_IsVeteran();
	virtual int m_shieldCost() = 0;
	std::shared_ptr<Nation> m_Nation(){return m_nationality;}
	bool m_homeCity();
	friend class Figurebutton;
};

inline bool Figure::m_IsVeteran(){return m_isVeteran;}

class TurnEndsTooEarly{
	public: std::string what() {return "Turn ends too early";}
};

class FigureElement: public MovableDrawingElement{
public:
	FigureElement(SDL_Renderer* renderer, MovableThing* movableEntity);
	virtual int m_draw(int rowShift, int columnShift, SDL_Renderer* renderer = nullptr) override;
};

#define STRENGTH_OF float
#define WHEN_ATTACKING(A) ::m_attackingStrength() {return A;}
#define WHEN_DEFENDING(A) ::m_defensiveStrength() {return A;}
#define VISIBILITY_OF short unsigned int
#define IS_NORMAL_VISIBILITY ::m_visibilityRange(){return 1;}
#define WEAK 1/1000000
#define THE_END_OF_TURN_FOR void
#define SHIELD_COST(CLASS, COST) int CLASS::m_shieldCost(){return COST;}
#define ONE_MOVEMENT_POINT 3
#define GOES_UNEVENTFUL ::m_finishMove(){std::cout<<"m_finishMove(), x = "<<m_image->getPosition().x<<", y = "<<m_image->getPosition().y<<std::endl;\
	m_figureState = DONE_WITH_TURN; \
	m_nationality->m_removeFromQueue(shared_from_this());\
return;}
#define DEFAULT_MOVING_POINTS(CLASS, DEFAULT) short unsigned int CLASS::m_defaultMovementPoints(){return DEFAULT*ONE_MOVEMENT_POINT;}
//Man beachte, dass zwecks Ermöglichung von Drittelpunkten in Dreierschritten gezählt wird.
#define NORMAL_MOVING(CLASS) DEFAULT_MOVING_POINTS(CLASS, 1)
#define CARGO_COUNT_MAX(CLASS, NUMBER) short unsigned int CLASS::m_cargoCountMax(){return NUMBER;}
#define VISIBILITY(CLASS,DEFAULT) short unsigned int CLASS::m_visibilityRange(){return DEFAULT;}
#define CREATE_IMAGE(TYPE)std::shared_ptr<MovableThing> TYPE::m_createImage(){\
		int x = m_whereItStands->m_X() + (STANDARD_FIELD_SIZE - City::figureWidth(m_FigureType()))/2, y  = m_whereItStands->m_Y()+(STANDARD_FIELD_SIZE - City::figureHeight(m_FigureType()))/2;\
		std::stringstream s;\
		s<<"bilder/Figures/"<<m_FigureType()<<"/"<<m_FigureType()<<m_nationality->m_colorString();\
		s.flush();\
		std::string imageString = s.str();\
		m_image = std::make_shared<MovableThing>(theRenderer, City::figureWidth(m_FigureType()), City::figureHeight(m_FigureType()), imageString.c_str(), x, y);\
		return m_image;\
}
#define DECONSTRUCTOR(CLASS) CLASS::~CLASS(){}\
CLASS::	CLASS(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)\
:NormalUnit(whereToStart,nationality,home,isVeteran){\
	m_resetMovementPoints();\
	if(!m_initImage()) std::cout<<"Fatal error: MovableThing not created"<<std::endl;\
}\
CREATE_IMAGE(CLASS)
#define FIGURE_TYPE(CLASS,TYPE)FigureType CLASS::m_FigureType(){return TYPE;}
#endif /* FIGURE_HPP_ */
