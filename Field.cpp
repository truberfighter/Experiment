/*
 * Field.cpp
 *
 *  Created on: 04.03.2022
 *      Author: uwe-w
 */

#include "Field.hpp"
#include "Drawing.hpp"
#include "Settlers.hpp"

Field::Field(int x, int y, Layer layer, bool hasSpecialResource)
: m_x(x), m_y(y), m_layer(layer), m_hasSpecialResource(hasSpecialResource)
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

int Field::m_shields(Nation& nation){
	int count = m_shields();
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

bool Field::m_HasSpecialResource(){return m_hasSpecialResource;}

bool Field::m_MiningTemplate(SettlersWork whatWorkWillCome, Settlers settlers){
	switch(whatWorkWillCome){
	case MAKE_MINING:{
		if(m_isMined){
				return false;//do not mine again
			}
		else{ //if(!m_isMined)
			settlers.m_work(MAKE_MINING);
			//Mining complete
			if(m_maybeFinishWork(settlers, MAKE_MINING)){
				//set to mined
				m_isMined = true;				}
			return true;
		}
	}
	default:{
		return false;
	}
	}
}

bool Field::m_IrrigateTemplate(SettlersWork whatWorkWillCome, Settlers settlers){
	switch(whatWorkWillCome){
	case IRRIGATE:{
		if(m_isIrrigated){
				return false;//do not mine again
			}
		else{ //if(!m_isIrrigated)
			settlers.m_work(IRRIGATE);
			//Irrigation complete
			if(m_maybeFinishWork(settlers, IRRIGATE)){
				//set to irrigated
				m_isIrrigated = true;				}
			return true;
		}
	}
	default:{
		return false;
	}
	}
}

int Field::m_roadTradeResult(){
	if(m_roadStatus == ROAD || m_roadStatus == RAILROAD)
			return 1;
		else //m_roadStatus == NOTHING
			return 0;
}

bool Field::m_Pillage(){
	if(m_isMined){
		m_isMined = false;
		return true;
	}
	if(m_isIrrigated){
		m_isIrrigated = false;
		return true;
	}
	else{
		return false;
	}
}
