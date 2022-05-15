/*
 * Plains.cpp
 *
 *  Created on: 12.03.2022
 *      Author: uwe-w
 */

#include "Plains.hpp"
#include <memory>
#include "Drawing.hpp"

Plains::Plains(int x, int y, bool hasSpecialResource)
:Field(x,y,STANDARD_FIELD_LAYER, hasSpecialResource)
{
	m_drawingElement = std::make_shared<ImmovableDrawingElement>(theRenderer, fieldTextures[PLAINS], x, y, STANDARD_FIELD_LAYER);
	m_drawingElement->m_setField(this);
}

Plains::~Plains(){

}

MovementPoints Plains::m_movementPoints(){
	return ONE_MOVEMENT_POINT;
}

float Plains::m_defenseBonus(){
	return 0;
}

//returns false if it can't be mined or if it is already mined
//returns true in amy other case, along with making the settlers mine
bool Plains::m_Mining(Settlers& settlers){
	return m_MiningTemplate(IRRIGATE, settlers);
}
bool Plains::m_Irrigate(Settlers& settlers){
	return m_IrrigateTemplate(IRRIGATE, settlers);
}

int Plains::m_shields(){
	return m_hasSpecialResource ? 3 : 1;
}

int Plains::m_food(){
	int count = 1;
		if(m_isIrrigated)
			count++;
		return count;
}

int Plains::m_trade()IS_CLASSICALLY_ROAD_BASED
std::string Plains::m_resourceOverview(){return "Until very much later, forget about that nonsense!";}
Landscape Plains::m_Landscape() const{
	//std::cout<<"Plains::Landscape"<<std::endl;
	return PLAINS;
}
short int Plains::m_howLongToTake(SettlersWork work){
	switch(work){
	case BUILD_ROAD:{ //soll mal ein Makro werden
		if(m_roadStatus == NOTHING){
			return STANDARD_ROAD_BUILDING_TIME;
		}
		else{ //m_roadStatus != NOTHING
			return SETTLERSWORK_UNAVAILABLE;
		}
	}
	case BUILD_RAILROAD:{
		if(m_roadStatus == NOTHING){
				return STANDARD_RAILROAD_BUILDING_TIME;
			}
		else{ //m_roadStatus != ROAD
			return SETTLERSWORK_UNAVAILABLE;
		}
	}
	case BUILD_FORTRESS:{
		return STANDARD_FORTRESS_BUILDING_TIME;
	}
	case IRRIGATE:{
		return STANDARD_IRRIGATION_TIME;
	}
	case CHANGE_TO_FOREST:{
		return STANDARD_FORESTING_TIME;
	}
	default:
		return SETTLERSWORK_UNAVAILABLE;
	}
}



