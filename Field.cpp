/*
 * Field.cpp
 *
 *  Created on: 04.03.2022
 *      Author: uwe-w
 */

#include "Field.hpp"
#include "Drawing.hpp"
#include "Settlers.hpp"

Field::Field(int x, int y, Layer layer)
: m_x(x), m_y(y), m_layer(layer)
{

}

Field::~Field(){}

int Field::m_X() const{return m_x;}

int Field::m_Y() const {return m_y;}

std::shared_ptr<ImmovableDrawingElement> Field::m_DrawingElement(){return m_drawingElement;}

RoadStatus Field::m_RoadStatus(){
	return m_roadStatus;
}

bool Field::m_IsMined() const{return m_isMined;}

bool Field::m_IsIrrigated() const{return m_isIrrigated;}

bool Field::m_maybeFinishWork(Settlers& settlers, SettlersWork work){
	if(m_howLongToTake(work)>=settlers.m_WorkStepsCount()){
		settlers.m_finishWork();
	}
}

void Field::m_railRoadProductionEffect(int& count){
	if(m_roadStatus == RAILROAD)
		count = count + count/2;
}

int Field::m_food(Nation& nation){
	int count = m_food();
	ACKNOWLEDGE_DESPOTISM
	ACKNOWLEDGE_RAILROAD
	return count;
}

int Field::m_trade(Nation& nation){
	int count = m_trade();
	ACKNOWLEDGE_DESPOTISM
	ACKNOWLEDGE_RAILROAD
	ACKNOWLEDGE_DEMOCRACY
	return count;
}
