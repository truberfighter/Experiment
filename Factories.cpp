/*
 * Factories.cpp
 *
 *  Created on: 04.02.2023
 *      Author: uwe-w
 */

#include "Factories.hpp"
#include "Field.hpp"
#include "GameSaver.hpp"
#include "GameMain.hpp"
#include "Game.hpp"
#include "City.hpp"
#include "Nation.hpp"
#include "Drawing.hpp"
#include "Settlers.hpp"
#include "NormalUnit.hpp"
#include "Artillery.hpp"
#include "Trireme.hpp"
#include "Caravan.hpp"
#include "Diplomat.hpp"
#include "Plane.hpp"

using Json = nlohmann::json;

FigureFactory theFigureFactory; FieldFactory theFieldFactory;

std::shared_ptr<Field> FieldFactory::m_createField(FieldJson& data)
{
	throw IstreamNeeded();
	Field* field = new Field(data.x,data.y,(Landscape)data.landscape,data.hasSpecialResource,data.hasShield);
	field->m_continentID = data.continentID;
	field->m_hasFortress = data.hasFortress;
	field->m_isIrrigated = data.isIrrigated;
	field->m_isMined = data.isMined;
	field->m_isPolluted = data.isPolluted;
	field->m_layer = data.layer;
	field->m_turnsUntilSwamped = data.turnsUntilSwamped;
	field->m_roadStatus = (RoadStatus) data.roadStatus;
	for(std::map<std::string, int>& current: data.nationKnowsData){
		field->m_nationFogInfo.push_back(NationKnowsField{(bool)current["isIrrigated"],(bool)current["isMined"],(bool)current["hasFortress"],(RoadStatus)current["roadStatus"],
		(bool)current["value"],(Nationality)current["nationality"],(Landscape)current["landscape"]});
	}
	//missing: figureIDs
	switch(data.roadStatus){
	case ROAD: field->m_createRoadImage(brownColor); break;
	case RAILROAD: field->m_createRoadImage(blackColor); break;
	default: break;
	}
	std::shared_ptr<Field> whatToReturn;
	whatToReturn.reset(field);
	return whatToReturn;
}

std::shared_ptr<Figure> FigureFactory::m_createFigure(FigureJson& data){
	std::shared_ptr<Figure> whatToReturn;
	switch((FigureType)data.figureType){
	case SETTLERS: throw IstreamNeeded();
	case BOMBER: throw IstreamNeeded();
	case FIGHTER: throw IstreamNeeded();
	case NUCLEAR: throw IstreamNeeded();
	case MILITIA: whatToReturn = std::make_shared<Settlers>(); break;
	case PHALANX: whatToReturn = std::make_shared<Phalanx>(); break;
	case CAVALRY: whatToReturn = std::make_shared<Cavalry>(); break;
	case TRIREME: whatToReturn = std::make_shared<Trireme>(); break;
	case CARRIER: whatToReturn = std::make_shared<Carrier>(); break;
	//case IRONCLAD: whatToReturn = std::make_shared<Ironclad>(); break;
	//case SUBMARINE: whatToReturn = std::make_shared<Submarine>(); break;
	//case CRUISER: whatToReturn = std::make_shared<Cruiser>(); break;
	//case SAIL: whatToReturn = std::make_shared<Sail>(); break;
	//case FRIGATE: whatToReturn = std::make_shared<Frigate>(); break;
	//case BATTLESHIP: whatToReturn = std::make_shared<Battleship>(); break;
	//case TRANSPORT: whatToReturn = std::make_shared<Transport>(); break;
	case ARMOR: whatToReturn = std::make_shared<Armor>(); break;
	case ARTILLERY: whatToReturn = std::make_shared<Artillery>(); break;
	case CARAVAN: whatToReturn = std::make_shared<Caravan>(); break;
	case DIPLOMAT: whatToReturn = std::make_shared<Diplomat>(); break;
	case CHARIOT: whatToReturn = std::make_shared<Chariot>(); break;
	case KNIGHTS: whatToReturn = std::make_shared<Knights>(); break;
	case CANNON: whatToReturn = std::make_shared<Cannon>(); break;
	case CATAPULT: whatToReturn = std::make_shared<Catapult>(); break;
	case MUSKETEERS: whatToReturn = std::make_shared<Musketeers>(); break;
	case LEGION: whatToReturn = std::make_shared<Legion>(); break;
	case RIFLEMEN: whatToReturn = std::make_shared<Riflemen>(); break;
	case MECH_INF: whatToReturn = std::make_shared<MechInf>(); break;
	default: throw InvalidFigureType();
	}
	m_figure = whatToReturn.get();
	m_editRawFigure(data);
	return whatToReturn;
}

void FigureFactory::m_editRawFigure(FigureJson& data){
	m_figure->m_figureID = data.figureID;
	m_figure->m_figureState = (FigureState) data.figureID;
	m_figure->m_isVeteran = data.isVeteran;
	m_figure->m_movementPoints.m_movementPoints = data.movementPoints;
	for(int currentNationality: data.visibilityInfo){
		m_figure->m_visibilityInfo.push_back((Nationality)currentNationality);
	}
}

std::shared_ptr<Figure> FigureFactory::m_createFigure(FigureJson& data, Json& data2){
	std::shared_ptr<Figure> whatToReturn;
	switch((FigureType)data.figureType){
	case SETTLERS:{ whatToReturn = std::make_shared<Settlers>();
	Settlers* createdObject = reinterpret_cast<Settlers*>(whatToReturn.get());
	createdObject->m_currentWork = (SettlersWork)data2["currentWork"];
	createdObject->m_workStepsCount = data2["workStepsCount"];
	break;}
	case BOMBER:{ whatToReturn = std::make_shared<Bomber>();
	Plane* createdObject = reinterpret_cast<Plane*>(whatToReturn.get());
	createdObject->m_turnsFlying = data2["turnsFlying"];
	break;}
	case FIGHTER:{ whatToReturn = std::make_shared<Fighter>();
	Plane* createdObject = reinterpret_cast<Plane*>(whatToReturn.get());
	createdObject->m_turnsFlying = data2["turnsFlying"]; break;}
	case NUCLEAR: {whatToReturn = std::make_shared<Nuclear>();
	Plane* createdObject = reinterpret_cast<Plane*>(whatToReturn.get());
	createdObject->m_turnsFlying = data2["turnsFlying"];
	break; }
	default: throw IstreamNotNeeded();
	}
	m_figure = whatToReturn.get();
	m_editRawFigure(data);
	return whatToReturn;
}


