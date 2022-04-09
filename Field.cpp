/*
 * Field.cpp
 *
 *  Created on: 04.03.2022
 *      Author: uwe-w
 */

#include "Field.hpp"
#include "Drawing.hpp"
#include "Settlers.hpp"
#include "FieldContainer.hpp"
#include "sdltypes.hpp"

Field::Field(int x, int y, Layer layer, bool hasSpecialResource)
: m_x(x), m_y(y), m_layer(layer), m_hasSpecialResource(hasSpecialResource)
{

}

std::shared_ptr<City> Field::m_CityContained(){
	return m_cityContained;
}

Field::~Field(){}

int Field::m_X() const{return m_x;}

int Field::m_Y() const {return m_y;}

std::shared_ptr<DrawingElement> Field::m_DrawingElement(){return m_drawingElement;}

RoadStatus Field::m_RoadStatus(){
	return m_roadStatus;
}

bool Field::m_IsMined() const{return m_isMined;}

bool Field::m_IsIrrigated() const{return m_isIrrigated;}

bool Field::m_maybeFinishWork(Settlers& settlers, SettlersWork work){
	if(m_howLongToTake(work)==SETTLERSWORK_UNAVAILABLE){
		std::cout<<"SettlersWork unavailable"<<std::endl;
	}
	if(m_howLongToTake(work)<=settlers.m_WorkStepsCount()){
		std::cout<<"m_howLongToTake("<<work<<") = "<<m_howLongToTake(work)<<", settlers.m_workStepsCount() = "<<settlers.m_WorkStepsCount()<<std::endl;
		settlers.m_finishWork();
		return true;
	}
	return false;
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

bool Field::m_MiningTemplate(SettlersWork whatWorkWillCome, Settlers& settlers){
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
				m_isMined = true;
			}
			return true;
		}
	}
	default:{
		return false;
	}
	}
}

bool Field::m_IrrigateTemplate(SettlersWork whatWorkWillCome, Settlers& settlers){
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
				m_isIrrigated = true;
			}
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

std::shared_ptr<Field> Field::m_getNeighbouringField(Direction whereToLook){
	int x = m_x/STANDARD_FIELD_SIZE;
	int y = m_y/STANDARD_FIELD_SIZE;
	//std::cout<<"Field::m_getNeighbouringField: x = "<<x<<", y = "<<y;
	switch(whereToLook){
		case UP:
			{y--; break;}
		case STANDING_STILL:
			break;
		case DOWN:
			{y++; break;}
		case RIGHT: {x++; break;}
		case LEFT: {x--; break;}
		case UP_LEFT: {y--; x--; break;}
		case UP_RIGHT: {y--; x++; break;}
		case DOWN_LEFT: {y++; x--; break;}
		case DOWN_RIGHT: {y++; x++; break;}
		}
	if(y==WORLD_HEIGHT)
			throw(PoleHitException(SOUTHERN_POLE_HIT));
		if(y==-1)
			throw(PoleHitException(NORTH_POLE_HIT));
	x = modulo(x, WORLD_LENGTH);
	//Tribut an Civi, man kann nicht vom Nord- oder Südpol weg
	y = y % WORLD_HEIGHT;
	//std::cout<<"; x = "<<x<<", y = "<<y<<std::endl;
	//Pol getroffen

	std::vector<Meridian>& fieldsOfTheWorld =  *theContainer->m_getFieldsOfTheWorld();
	return fieldsOfTheWorld[x][y];
}

bool Field::m_createRoadImage(SDL_Color& color){
	std::cout<<"road image created"<<std::endl;
	m_drawingElement->m_setAdditionalInstructions
	([](int x, int y, SDL_Renderer* renderer)->int{return Graphics::drawSquareStarLines(renderer, x, y, brownColor);});
	return true;
}



bool Field::m_road(Settlers& settlers){
	if(m_roadStatus == RAILROAD){
		std::cout<<"You cannot build some road on top of rail road"<<std::endl;
		return false;
	}
	if(m_roadStatus == NOTHING){
		if(m_Landscape()==RIVER&&(!(settlers.m_canBuildBridges()))){
			std::cout<<"Bridge building issues!"<<std::endl;
			return false;
		}
		settlers.m_work(BUILD_ROAD);
		if(!m_maybeFinishWork(settlers, BUILD_ROAD)){
			return true;
		}
	m_roadStatus = ROAD;
	return m_createRoadImage(brownColor);
	}
	//alter the drawing element

	if(m_roadStatus==ROAD){
		if(!settlers.m_canBuildRailroad()){
			std::cout<<"Settlers cannot build rail road!"<<std::endl;
			return false;
		}
		settlers.m_work(BUILD_ROAD);
		if(!m_maybeFinishWork(settlers, BUILD_ROAD)){
			return true;
		}
		m_roadStatus = RAILROAD;
		return m_createRoadImage(blackColor);
	}
	std::cout<<"For some reason, there is no return for Field::m_road"<<std::endl;
	return false;
}
