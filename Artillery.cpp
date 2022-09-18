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

