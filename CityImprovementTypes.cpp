/*
 * CityImprovementTypes.cpp
 *
 *  Created on: 17.09.2022
 *      Author: uwe-w
 */

#include "City.hpp"
#include <sstream>
#include "Field.hpp"
#include "Artillery.hpp"
#include "Ship.hpp"
#include "Trireme.hpp"
#include "Diplomat.hpp"
#include "GameMain.hpp"
#include "Plane.hpp"

int City::figureWidth(FigureType figtype){
	switch(figtype){
	case DIPLOMAT: return 36;
	default: return 60;
	}
}

int City::figureHeight(FigureType figtype){
	switch(figtype){
	case SETTLERS: return 60;
	case DIPLOMAT: return 60;
	case ARTILLERY: return 60;
	case TRIREME: return 44;
	case CAVALRY: return 60;
	case CARRIER: return 36;
	default: return 60;
	}
}

int City::shieldsNeeded(ImprovementType imptype){
	switch(imptype){
	case IMPROVEMENT_BOMBER: return 120;
	case IMPROVEMENT_FIGHTER: return 60;
	case IMPROVEMENT_ARTILLERY: return 60;
	case IMPROVEMENT_SETTLERS: return 40;
	case IMPROVEMENT_TRIREME: return 40;
	case IMPROVEMENT_DIPLOMAT: return 30;
	case IMPROVEMENT_CAVALRY: return 20;
	case IMPROVEMENT_CARRIER: return 160;
	case TEMPLE: return 40;
	case COURTHOUSE: return 80;
	case PALACE: return 200;
	case GRANARY: return 60;
	case AQUEDUCT: return 120;
	case BANK: return 120;
	case LIBRARY: return 80;
	case MFG_PLANT: return 320;
	case CATHEDRAL: return 160;
	case BARRACKS: return 40;
	case NUCLEAR_PLANT: return 160;
	case COLOSSEUM: return 100;
	case POWER_PLANT: return 160;
	case MARKETPLACE: return 80;
	case MASS_TRANSIT: return 160;
	case RECYCLING_CENTER: return 200;
	case SDI_DEFENSE: return 200;
	case CITY_WALLS: return 120;
	case UNIVERSITY: return 160;
	case HYDRO_PLANT: return 240;
	case APOLLO_PROGRAM: return 600;
	case GREAT_WALL: return 300;
	case DARWINS_VOYAGE: return 300;
	case MAGELLANS_EXPEDITION: return 400;
	case WOMENS_SUFFRAGE: return 600;
	case GREAT_LIBRARY: return 300;
	case HOOVER_DAM: return 600;
	case HANGING_GARDENS: return 300;
	case ISAAC_NEWTONS_COLLEGE: return 400;
	case BACH_CATHEDRAL: return 400;
	case MICHELANGELOS_CHAPEL: return 300;
	case COLOSSUS: return 300;
	case COPERNICUS_OBSERVATORY: return 300;
	case CURE_FOR_CANCER: return 600;
	case LIGHTHOUSE: return 300;
	case MANHATTAN_PROJECT: return 600;
	case ORACLE: return 300;
	case PYRAMIDS: return 300;
	case SETI_PROGRAM: return 600;
	case SHAKESPEARES_THEATRE: return 400;
	case UNITED_NATIONS: return 600;
	default:
	{
		std::stringstream s;
		s<<imptype <<" does not have any shield cost data given!";s.flush();
		throw NoGivenData(s.str());
	}
	}
}

int City::maintenanceNeeded(ImprovementType imptype){
	if(!isBuildingType(imptype)){
		throw imptype;
	}
	switch(imptype){
	case TEMPLE: return 1;
	case GRANARY: return 1;
	case AQUEDUCT: return 3;
	case BANK: return 3;
	case LIBRARY: return 1;
	case MFG_PLANT: return 6;
	case CATHEDRAL: return 3;
	case BARRACKS: return 0;
	case NUCLEAR_PLANT: return 2;
	case COLOSSEUM: return 4;
	case POWER_PLANT: return 4;
	case MARKETPLACE: return 1;
	case MASS_TRANSIT: return 4;
	case RECYCLING_CENTER: return 2;
	case SDI_DEFENSE: return 4;
	case CITY_WALLS: return 2;
	case UNIVERSITY: return 3;
	case HYDRO_PLANT: return 4;
	case COURTHOUSE: return 2;
	case PALACE: return 0; //forget Civilopedia!
	default:
	{
		std::stringstream s;
		s<<imptype <<" does not have any maintanance data given!";s.flush();
		throw NoGivenData(s.str());
	}
	}
}



bool City::isWonderType(ImprovementType imptype){
	if(imptype>=WONDER_MIN && imptype <= WONDER_MAX)
		return true;
	return false;
}

bool City::isBuildingType(ImprovementType imptype){
	if(imptype>=BUILDING_MIN && imptype <= BUILDING_MAX)
		return true;
	return false;
}

bool City::isFigureType(ImprovementType imptype){
	return !(isBuildingType(imptype)||isWonderType((imptype)));
}

#define CASE_FOR_BUILDING(TYPE,CLASS) case TYPE:\
	{\
		newFigure = std::make_shared<CLASS>(m_whereItStands, m_owningNation, shared_from_this(), gonnaBeVeteran);\
		goto figureMade;\
	}

bool City::m_maybeFinishProduction(){
	std::shared_ptr<Figure> newFigure;
	if((m_shields+=m_shieldProduction())<shieldsNeeded(m_whatIsBuilt)){
		return false;
	}
	m_shields = 0;
	bool gonnaBeVeteran  = false;
	std::shared_ptr<CityImprovement> newImprovement = m_maybeBuild(m_whatIsBuilt);
	if(newImprovement != nullptr){
		m_improvements.push_back(*newImprovement);
	}
	switch(m_whatIsBuilt){
	case IMPROVEMENT_SETTLERS:
	{
		newFigure = std::make_shared<Settlers>(m_whereItStands, m_owningNation, shared_from_this(), gonnaBeVeteran);
	figureMade:	m_owningNation->m_addFigure(newFigure);
		m_whereItStands->m_takeFigure(newFigure);
		newFigure->m_integrateInto(*someDrawing);
		std::cout<<"lolj"<<std::endl;
		return true;
	}
	CASE_FOR_BUILDING(IMPROVEMENT_ARTILLERY,Artillery)
	CASE_FOR_BUILDING(IMPROVEMENT_BOMBER,Bomber)
	CASE_FOR_BUILDING(IMPROVEMENT_CARRIER,Carrier)
	CASE_FOR_BUILDING(IMPROVEMENT_CAVALRY,Cavalry)
	default: return false;
	}
}

int City::m_buyingPrice(ImprovementType imptype){
	if(m_shields!=0){
		return (shieldsNeeded(imptype)-m_shields)/2;
	}
	if(!City::isFigureType(imptype)){
		return 4*shieldsNeeded(imptype);
	}
	if(shieldsNeeded(imptype)==40){
		return 320;
	}
	switch((FigureType)imptype){
	case ARTILLERY: return 600;
	case BOMBER: return 1920;
	case FIGHTER: return 600;
	case CAVALRY: return 120;
	case DIPLOMAT: return 210;
	case CARRIER: return 1200;
	default:
	throw imptype;
	}
}

#define IF_UNBUILT(A,B) if(!m_contains(A)&&B && m_owningNation->m_hasExplored(Science::techInfo(A).neededTech1) && m_owningNation->m_hasExplored(Science::techInfo(A).neededTech2))whatToReturn.push_back(A);
std::vector<ImprovementType> City::m_whatCanBeBuilt(){
	std::vector<ImprovementType> whatToReturn;
	whatToReturn.push_back(IMPROVEMENT_SETTLERS);
	whatToReturn.push_back(IMPROVEMENT_CAVALRY);
	whatToReturn.push_back(IMPROVEMENT_BOMBER);
	if(m_whereItStands->m_closeToLandscape(OCEAN)){
		whatToReturn.push_back(IMPROVEMENT_CARRIER);
	}
	/*for(int imptypeInt = BUILDING_MIN; imptypeInt <= BUILDING_MAX; imptypeInt++){
		switch(imptypeInt){
		case HYDRO_PLANT:
			IF_UNBUILT(HYDRO_PLANT,(m_whereItStands->m_closeToLandscape(MOUNTAIN)||m_whereItStands->m_closeToLandscape(RIVER)))
		}
	}*/
	/*IF_UNBUILT(PALACE,true)
	IF_UNBUILT(TEMPLE,true)
	IF_UNBUILT(GRANARY,true)
	IF_UNBUILT(COURTHOUSE,true)*/
	return whatToReturn;
}
