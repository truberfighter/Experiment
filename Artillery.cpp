/*
 * Artillery.cpp
 *
 *  Created on: 14.09.2022
 *      Author: uwe-w
 */

#include "Artillery.hpp"
#include "Nation.hpp"
#include "Field.hpp"
#include "City.hpp"

FigureCategory NormalUnit::m_FigureCategory(){
	return GROUND;
}

std::string NormalUnit::m_orderOverview(){return "H: HomeCity, S: Sentry, P: Pillage, F: Fortify";}

NormalUnit::NormalUnit(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)
: Figure(whereToStart, nationality, home, isVeteran)
{
}
NormalUnit::~NormalUnit(){
	std::cout<<"normalunitdestruktor"<<this<<std::endl;
}


bool NormalUnit::m_takeOrder(char order){
	switch(order){
	case 's':{
		m_sentry();
		return true;
	}
	case 'f':{
		m_fortify();
		return true;
	}
	case ' ': {
		m_finishMove();
		return true;
	}
	case 'p':{
		return m_Pillage();
	}
	case 'h': return m_homeCity();
	default: return false;
}
}

THE_END_OF_TURN_FOR NormalUnit GOES_UNEVENTFUL





DECONSTRUCTOR(Artillery)
DEFAULT_MOVING_POINTS(Artillery,2)
SHIELD_COST(Artillery,60)
VISIBILITY(Artillery,1)
FigureType Artillery::m_FigureType(){return ARTILLERY;}
STRENGTH_OF Artillery WHEN_DEFENDING(2)
STRENGTH_OF Artillery WHEN_ATTACKING(12)

DEFAULT_MOVING_POINTS(Cavalry,2)
SHIELD_COST(Cavalry,2)
VISIBILITY(Cavalry,1)
DECONSTRUCTOR(Cavalry)
FigureType Cavalry::m_FigureType(){return CAVALRY;}
STRENGTH_OF Cavalry WHEN_DEFENDING(1)
STRENGTH_OF Cavalry WHEN_ATTACKING(2)

DECONSTRUCTOR(Legion)
DEFAULT_MOVING_POINTS(Legion,1)
SHIELD_COST(Legion,20)
VISIBILITY(Legion,1)
FIGURE_TYPE(Legion,LEGION)
STRENGTH_OF Legion WHEN_DEFENDING(1)
STRENGTH_OF Legion WHEN_ATTACKING(3)

DECONSTRUCTOR(Phalanx)
DEFAULT_MOVING_POINTS(Phalanx,1)
SHIELD_COST(Phalanx,20)
VISIBILITY(Phalanx,1)
FIGURE_TYPE(Phalanx,PHALANX)
STRENGTH_OF Phalanx WHEN_DEFENDING(2)
STRENGTH_OF Phalanx WHEN_ATTACKING(1)

DECONSTRUCTOR(Musketeers)
DEFAULT_MOVING_POINTS(Musketeers,1)
SHIELD_COST(Musketeers,30)
VISIBILITY(Musketeers,1)
FIGURE_TYPE(Musketeers,MUSKETEERS)
STRENGTH_OF Musketeers WHEN_DEFENDING(3)
STRENGTH_OF Musketeers WHEN_ATTACKING(2)

DECONSTRUCTOR(Riflemen)
DEFAULT_MOVING_POINTS(Riflemen,1)
SHIELD_COST(Riflemen,30)
VISIBILITY(Riflemen,1)
FIGURE_TYPE(Riflemen,RIFLEMEN)
STRENGTH_OF Riflemen WHEN_DEFENDING(5)
STRENGTH_OF Riflemen WHEN_ATTACKING(3)

DECONSTRUCTOR(Chariot)
DEFAULT_MOVING_POINTS(Chariot,2)
SHIELD_COST(Chariot,40)
VISIBILITY(Chariot,1)
FIGURE_TYPE(Chariot,CHARIOT)
STRENGTH_OF Chariot WHEN_DEFENDING(1)
STRENGTH_OF Chariot WHEN_ATTACKING(4)

DECONSTRUCTOR(Armor)
DEFAULT_MOVING_POINTS(Armor,3)
SHIELD_COST(Armor,80)
VISIBILITY(Armor,1)
FIGURE_TYPE(Armor,ARMOR)
STRENGTH_OF Armor WHEN_DEFENDING(5)
STRENGTH_OF Armor WHEN_ATTACKING(10)

DECONSTRUCTOR(MechInf)
DEFAULT_MOVING_POINTS(MechInf,3)
SHIELD_COST(MechInf,50)
VISIBILITY(MechInf,1)
FIGURE_TYPE(MechInf,MECH_INF)
STRENGTH_OF MechInf WHEN_DEFENDING(6)
STRENGTH_OF MechInf WHEN_ATTACKING(6)

DECONSTRUCTOR(Cannon)
DEFAULT_MOVING_POINTS(Cannon,1)
SHIELD_COST(Cannon,40)
VISIBILITY(Cannon,1)
FIGURE_TYPE(Cannon,CANNON)
STRENGTH_OF Cannon WHEN_DEFENDING(1)
STRENGTH_OF Cannon WHEN_ATTACKING(8)

DECONSTRUCTOR(Catapult)
DEFAULT_MOVING_POINTS(Catapult,1)
SHIELD_COST(Catapult,40)
VISIBILITY(Catapult,1)
FIGURE_TYPE(Catapult,CATAPULT)
STRENGTH_OF Catapult WHEN_DEFENDING(1)
STRENGTH_OF Catapult WHEN_ATTACKING(6)

DECONSTRUCTOR(Militia)
DEFAULT_MOVING_POINTS(Militia,1)
SHIELD_COST(Militia,10)
VISIBILITY(Militia,1)
FIGURE_TYPE(Militia,MILITIA)
STRENGTH_OF Militia WHEN_DEFENDING(1)
STRENGTH_OF Militia WHEN_ATTACKING(1)

DECONSTRUCTOR(Knights)
DEFAULT_MOVING_POINTS(Knights,2)
SHIELD_COST(Knights,40)
VISIBILITY(Knights,1)
FIGURE_TYPE(Knights,KNIGHTS)
STRENGTH_OF Knights WHEN_DEFENDING(2)
STRENGTH_OF Knights WHEN_ATTACKING(4)
