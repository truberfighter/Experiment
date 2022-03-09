/*
 * Grassland.cpp
 *
 *  Created on: 08.03.2022
 *      Author: uwe-w
 */

#include "Grassland.hpp"
#include <memory>
#include "Drawing.hpp"

Grassland::Grassland(int x, int y, bool hasShield)
:Field(x,y,STANDARD_FIELD_LAYER), m_hasShield(false)
{
	m_drawingElement = std::make_shared<ImmovableDrawingElement>(theRenderer, fieldTextures[GRASSLAND], x, y, STANDARD_FIELD_LAYER);
}

Grassland::~Grassland(){

}

int Grassland::m_movementPoints(){
	return 1;
}

float Grassland::m_defenseBonus(){
	return 0;
}

//returns false if it can't be mined or if it is already mined
//returns true in amy other case, along with making the settlers mine
bool Grassland::m_Mining(Settlers& settlers){
	if(m_isMined){
		return false;//do not mine again
	}
	else{ //if(!m_isMined)
		settlers.m_work(MAKE_MINING);
		//Mining complete
		if(m_maybeFinishWork(settlers, MAKE_MINING)){
			//set to mined
			m_isMined = true;
		}
		return true;
	}
}
bool Grassland::m_Irrigate(Settlers& settlers){
	if(m_isIrrigated){
			return false;//do not mine again
	}
	else{ //if(!m_isIrrigated)
		settlers.m_work(IRRIGATE);
		//Irrigation complete
		if(m_maybeFinishWork(settlers, IRRIGATE)){
			//set to irrigated
			m_isMined = true;
		}
		return true;
	}
}
int Grassland::m_shields(Nation& nation){
	return m_hasShield ? 1 : 0;
}
int Grassland::m_shields(){
	return m_hasShield ? 1 : 0;
}

int Grassland::m_food(){
	int count = 2;
		if(m_isIrrigated)
			count++;
		return count;
}

int Grassland::m_trade(){
	if(m_roadStatus == ROAD || m_roadStatus == RAILROAD)
		return 1;
	else //m_roadStatus == NOTHING
		return 0;
}
std::string Grassland::m_resourceOverview(){}
Landscape Grassland::m_Landscape(){}
short int Grassland::m_howLongToTake(SettlersWork work){
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
