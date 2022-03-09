/*
 * Field.hpp
 *
 *  Created on: 04.03.2022
 *      Author: uwe-w
 */

#ifndef FIELD_HPP_
#define FIELD_HPP_

#include "sdltypes.hpp"
#include "Nation.hpp"
//#include "Drawing.cpp"
#include <String>

class City;
class Settlers;
class Figure;
class ImmovableDrawingElement;
class FieldContainer;

class Field{
protected:
	std::shared_ptr<ImmovableDrawingElement> m_drawingElement;
	int m_x;
	int m_y;
	Layer m_layer;
	bool m_isIrrigated = false;
	std::shared_ptr<City> m_cityContained;
	bool m_isMined = false;
	bool m_maybeFinishWork(Settlers& settlers, SettlersWork work);
	virtual short int m_howLongToTake(SettlersWork work) = 0;
	RoadStatus m_roadStatus = NOTHING;
	void m_railRoadProductionEffect(int& count);

public:
	virtual ~Field();
	Field(int x, int y, Layer layer);
	int m_X() const;
	int m_Y() const;
	std::shared_ptr<ImmovableDrawingElement> m_DrawingElement();
	virtual int m_movementPoints() =0;
	virtual float m_defenseBonus()=0;
	bool m_IsMined() const;
	bool m_IsIrrigated() const;
	virtual bool m_Mining(Settlers& settlers)=0;
	virtual bool m_Irrigate(Settlers& settlers)=0;
	virtual int m_shields(Nation& nation)=0;
	virtual int m_shields()=0;
	int m_food(Nation& nation);
	virtual int m_food()=0;
	int m_trade(Nation& nation);
	virtual int m_trade()=0;
	virtual std::string m_resourceOverview()=0;
	virtual Landscape m_Landscape() = 0;
	RoadStatus m_RoadStatus();
};
#define ACKNOWLEDGE_RAILROAD m_railRoadProductionEffect(count);
#define ACKNOWLEDGE_DEMOCRACY if(count>2 && (nation.m_Government()==REPUBLIC || nation.m_Government()==DEMOCRACY))count++;
#define ACKNOWLEDGE_DESPOTISM if(count>2 && (nation.m_Government()==ANARCHY || nation.m_Government()==DESPOTISM))count--;
extern FieldContainer* theContainer;

#endif /* FIELD_HPP_ */
