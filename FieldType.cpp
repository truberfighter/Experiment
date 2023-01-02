/*
 * FieldType.cpp
 *
 *  Created on: 18.12.2022
 *      Author: uwe-w
 */

#include "Field.hpp"
#include "FieldType.hpp"
#include "FieldContainer.hpp"



#define CASE(work,what) case work: return what;
#define ROAD_CASES(aa,ab) case BUILD_ROAD: return field->m_RoadStatus()== ROAD ? aa : SETTLERSWORK_UNAVAILABLE;\
case BUILD_RAILROAD: return field->m_RoadStatus()==RAILROAD ? ab : SETTLERSWORK_UNAVAILABLE;
#define IRRIGATE_CASE(aa) case IRRIGATE: return field->m_IsIrrigated() ? SETTLERSWORK_UNAVAILABLE : aa;
#define MINING_CASE(aa) case MAKE_MINING: return field->m_IsMined() ? SETTLERSWORK_UNAVAILABLE : aa;
short int howLongToTakePlains(SettlersWork work, Field* field){
	if(!field->m_FiguresOnField().empty())
	switch(work){
			case BUILD_RAILROAD: {if(!field->m_FiguresOnField().front()->m_Nation()->m_canBuildRailroad())
				return SETTLERSWORK_UNAVAILABLE; break;
			}
			case BUILD_FORTRESS: {if(!field->m_FiguresOnField().front()->m_Nation()->m_hasExplored(TECHNOLOGY_CONSTRUCTION)) return SETTLERSWORK_UNAVAILABLE;
			default: break;
		}
	}
	switch(work){
	MINING_CASE(/*STANDARD_FORESTING_TIME*/1) //modified for testing
	IRRIGATE_CASE(1)
	ROAD_CASES(STANDARD_ROAD_BUILDING_TIME,STANDARD_RAILROAD_BUILDING_TIME)
	CASE(BUILD_FORTRESS,STANDARD_FORTRESS_BUILDING_TIME)
	default: return SETTLERSWORK_UNAVAILABLE;
	}
}

short int howLongToTakeJungle(SettlersWork work, Field* field){
	if(!field->m_FiguresOnField().empty())
	switch(work){
			case BUILD_RAILROAD: {if(!field->m_FiguresOnField().front()->m_Nation()->m_canBuildRailroad())
				return SETTLERSWORK_UNAVAILABLE; break;
			}
			case BUILD_FORTRESS: {if(!field->m_FiguresOnField().front()->m_Nation()->m_hasExplored(TECHNOLOGY_CONSTRUCTION)) return SETTLERSWORK_UNAVAILABLE;
			default: break;
		}
	}
	switch(work){
		MINING_CASE(STANDARD_FORESTING_TIME)
		IRRIGATE_CASE(STANDARD_GRASSLANDING_TIME)
		ROAD_CASES(4,8)
		CASE(BUILD_FORTRESS,STANDARD_FORTRESS_BUILDING_TIME)
		default: return SETTLERSWORK_UNAVAILABLE;
	}
}

short int howLongToTakeForest(SettlersWork work, Field* field){
	if(!field->m_FiguresOnField().empty())
switch(work){
			case BUILD_RAILROAD: {if(!field->m_FiguresOnField().front()->m_Nation()->m_canBuildRailroad())
				return SETTLERSWORK_UNAVAILABLE; break;
			}
			case BUILD_FORTRESS: {if(!field->m_FiguresOnField().front()->m_Nation()->m_hasExplored(TECHNOLOGY_CONSTRUCTION)) return SETTLERSWORK_UNAVAILABLE;
			default: break;
		}
	}
	switch(work){
		IRRIGATE_CASE(STANDARD_GRASSLANDING_TIME)
		ROAD_CASES(4,8)
		CASE(BUILD_FORTRESS,STANDARD_FORTRESS_BUILDING_TIME)
		default: return SETTLERSWORK_UNAVAILABLE;
	}
}

short int howLongToTakeOcean(SettlersWork work, Field* field){
	return SETTLERSWORK_UNAVAILABLE;
}

short int howLongToTakeTundra(SettlersWork work, Field* field)
{
	if(!field->m_FiguresOnField().empty())
	switch(work){
		case BUILD_RAILROAD: {if(!field->m_FiguresOnField().front()->m_Nation()->m_canBuildRailroad())
			return SETTLERSWORK_UNAVAILABLE; break;
		}
		case BUILD_FORTRESS: {if(!field->m_FiguresOnField().front()->m_Nation()->m_hasExplored(TECHNOLOGY_CONSTRUCTION)) return SETTLERSWORK_UNAVAILABLE;
		default: break;
	}
	}
	switch(work){
	ROAD_CASES(STANDARD_ROAD_BUILDING_TIME,STANDARD_RAILROAD_BUILDING_TIME)
	CASE(BUILD_FORTRESS,STANDARD_FORTRESS_BUILDING_TIME)
	default: return SETTLERSWORK_UNAVAILABLE;
	}
}

short int howLongToTakeRiver(SettlersWork work, Field* field){
	if(!field->m_FiguresOnField().empty()){
	switch(work){
		case BUILD_ROAD: {if(!field->m_FiguresOnField().front()->m_Nation()->m_canBuildBridges()){
			return SETTLERSWORK_UNAVAILABLE;
		}
		}
		case BUILD_RAILROAD: {if(!field->m_FiguresOnField().front()->m_Nation()->m_canBuildRailroad())
			return SETTLERSWORK_UNAVAILABLE; break;
		}
		case BUILD_FORTRESS: {if(!field->m_FiguresOnField().front()->m_Nation()->m_hasExplored(TECHNOLOGY_CONSTRUCTION)) return SETTLERSWORK_UNAVAILABLE;
	}
		default: break;
	}}
	switch(work){
	IRRIGATE_CASE(STANDARD_IRRIGATION_TIME)
	ROAD_CASES(STANDARD_ROAD_BUILDING_TIME,STANDARD_RAILROAD_BUILDING_TIME)
	CASE(BUILD_FORTRESS,STANDARD_FORTRESS_BUILDING_TIME)
	default: return SETTLERSWORK_UNAVAILABLE;
	}
}

short int howLongToTakeMountain(SettlersWork work, Field* field){
	if(!field->m_FiguresOnField().empty())
switch(work){
		case BUILD_RAILROAD: {if(!field->m_FiguresOnField().front()->m_Nation()->m_canBuildRailroad())
			return SETTLERSWORK_UNAVAILABLE; break;
		}
		case BUILD_FORTRESS: {if(!field->m_FiguresOnField().front()->m_Nation()->m_hasExplored(TECHNOLOGY_CONSTRUCTION)) return SETTLERSWORK_UNAVAILABLE;
		default: break;
		}
	}
	switch(work){
	MINING_CASE( 10)
	ROAD_CASES(6,12)
	CASE(BUILD_FORTRESS,7)
	default: return SETTLERSWORK_UNAVAILABLE;
	}
}

short int howLongToTakeHill(SettlersWork work, Field* field){
	if(!field->m_FiguresOnField().empty())
		switch(work){
				case BUILD_RAILROAD: {if(!field->m_FiguresOnField().front()->m_Nation()->m_canBuildRailroad())
					return SETTLERSWORK_UNAVAILABLE; break;
				}
				case BUILD_FORTRESS: {if(!field->m_FiguresOnField().front()->m_Nation()->m_hasExplored(TECHNOLOGY_CONSTRUCTION)) return SETTLERSWORK_UNAVAILABLE;
				default: break;
				}
			}
switch(work){
	MINING_CASE( 10)
	CASE(BUILD_FORTRESS,7)
	IRRIGATE_CASE(7)
	ROAD_CASES(5,10)
	default: return SETTLERSWORK_UNAVAILABLE;
	}
}

void irrigateStandard(Field* field){
field->m_setIsIrrigated(true);
}

void miningStandard(Field* field){
field->m_setIsMined(true);
}

void changeTo(Field* field, Landscape ls){
	field->m_changeLandscapeTo(ls);
}

FieldType& Field::m_getFieldType(){
	return (theContainer->m_FieldTypes())[m_landscape];
}

#undef CASE
#define CASE(aa,ab) case aa: {count = ab; }
#define RESOURCE_CASE(aa,ab,bb) case aa: {count = field->m_HasSpecialResource() ? bb : ab; }
#define SHIELD_CASE(aa) case aa: {count = field->m_HasShield() ? 1 : 0;}

int shieldsGeneral(Field* field, Landscape ls){
	int count = 0;
	if(field->m_IsMined()){
		switch(ls){
		RESOURCE_CASE(HILLS,3,5)
		CASE(MOUNTAIN,2)
		CASE(DESERT,2)
		default: goto restSwitch;
		}
		goto furtherProcedure;
	}
	restSwitch: switch(ls){
	SHIELD_CASE(GRASSLAND)
	SHIELD_CASE(RIVER)
	CASE(DESERT,1)
	RESOURCE_CASE(HILLS,0,3)
	CASE(MOUNTAIN,1)
	RESOURCE_CASE(PLAINS,1,3)
	CASE(FOREST,2)
	RESOURCE_CASE(SWAMP,0,4)
	CASE(ARCTIC,0)
	CASE(TUNDRA,0)
	CASE(JUNGLE,0)
	CASE(OCEAN,0)
	}
furtherProcedure:
	return count;
}
#define IRRIGATION_CASE(aa,ab) case aa: {count = field->m_IsIrrigated() ? ab + 1 : aa; break;}
int foodGeneral(Field* field, Landscape ls){
	int count  = 0;
	switch(ls){
	RESOURCE_CASE(DESERT,0,4)
	RESOURCE_CASE(FOREST,1,3)
	RESOURCE_CASE(TUNDRA,1,3)
	RESOURCE_CASE(ARCTIC,0,2)
	RESOURCE_CASE(OCEAN,1,3)
	CASE(RIVER,2)
	CASE(PLAINS,1)
	CASE(GRASSLAND,2)
	CASE(SWAMP,1)
	CASE(JUNGLE,1)
	CASE(MOUNTAIN,0)
	CASE(HILLS,1)
	}
	if(field->m_IsIrrigated())
		count++;
	return count;
}
#define ROAD_CASE(aa) case aa: {count = field->m_RoadStatus()==NOTHING ? 0 : 1; break;}
int tradeGeneral(Field* field, Landscape ls){
	int count = 0;
	switch(ls){
	RESOURCE_CASE(MOUNTAIN,0,6)
	RESOURCE_CASE(JUNGLE,0,3)
	ROAD_CASE(PLAINS)
	ROAD_CASE(GRASSLAND)
	ROAD_CASE(DESERT)
	CASE(RIVER,1)
	CASE(OCEAN,2)
	default: break;
	}
	return count;
}
void nothing(Field* field){}
#define TEXTURES(aa) fieldTextures[aa]/*, resourceTextures[aa],shieldTextures[aa]*/
#define CHANGER(aa) FieldTypeDecorator{std::bind(changeTo, _1,aa)}
#define RESOURCES(aa) std::bind(foodGeneral,_1,aa), std::bind(shieldsGeneral,_1,aa), std::bind(tradeGeneral,_1,aa)
using namespace std::placeholders;
void FieldContainer::m_initFieldTypes(){
	FieldTypeDecorator miner{miningStandard},irrigator{irrigateStandard}, none{nothing};
	// GRASSLAND, PLAINS, OCEAN, RIVER, MOUNTAIN, HILLS, TUNDRA, ARCTIC, SWAMP, JUNGLE, FOREST, DESERT
	MovementData standardData{0,ONE_MOVEMENT_POINT};
	m_fieldTypes.push_back(FieldType{
		GRASSLAND,irrigator,CHANGER(FOREST),howLongToTakePlains, RESOURCES(GRASSLAND),
standardData, TEXTURES(GRASSLAND)
	});
	m_fieldTypes.push_back(FieldType{
			PLAINS,irrigator,CHANGER(FOREST),howLongToTakePlains, RESOURCES(PLAINS),
	standardData, TEXTURES(PLAINS)
	});
	m_fieldTypes.push_back(FieldType{
		OCEAN,none,none,howLongToTakeOcean, RESOURCES(OCEAN),
	standardData, TEXTURES(OCEAN)
	});
}
#undef TEXTURES
#undef CHANGER
#undef RESOURCES


#undef CASE
#undef RESOURCE_CASE
