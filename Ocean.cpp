/*
 * Ocean.cpp
 *
 *  Created on: 10.04.2022
 *      Author: uwe-w
 */

/*
 * Ocean.cpp
 *
 *  Created on: 08.03.2022
 *      Author: uwe-w
 */

#include "Ocean.hpp"
#include <memory>
#include "Drawing.hpp"

Ocean::Ocean(int x, int y, bool hasSpecialResource)
:Field(x,y,STANDARD_FIELD_LAYER), m_hasSpecialResource(hasSpecialResource)
{
	m_drawingElement = std::make_shared<ImmovableDrawingElement>(theRenderer, fieldTextures[OCEAN], x, y, STANDARD_FIELD_LAYER);
	m_drawingElement->m_setField(this);
}

Ocean::~Ocean(){

}

MovementPoints Ocean::m_movementPoints(){
	return ONE_MOVEMENT_POINT;
}

float Ocean::m_defenseBonus(){
	return 0;
}

//returns false if it can't be mined or if it is already mined
//returns true in amy other case, along with making the settlers mine
bool Ocean::m_Mining(Settlers& settlers){
	return m_MiningTemplate(IRRIGATE, settlers);
}
bool Ocean::m_Irrigate(Settlers& settlers){
	return m_IrrigateTemplate(IRRIGATE, settlers);
}

int Ocean::m_shields(){
	return 0;
}

int Ocean::m_food(){
	int count = 1;
		if(m_hasSpecialResource)
			count+=2;
		return count;
}

int Ocean::m_trade() {
return 3;
}
std::string Ocean::m_resourceOverview(){return "Until very much later, forget about that nonsense!";}
Landscape Ocean::m_Landscape() const{
	//std::cout<<"Ocean::Landscape"<<std::endl;
	return OCEAN;
}
short int Ocean::m_howLongToTake(SettlersWork work){
	return SETTLERSWORK_UNAVAILABLE;

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



