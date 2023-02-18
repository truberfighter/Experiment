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
#include "FieldContainer.hpp"

using Json = nlohmann::json;

FigureFactory theFigureFactory; FieldFactory theFieldFactory; CityFactory theCityFactory;
NationFactory theNationFactory; GameFactory theGameFactory;
Field* FieldFactory::m_createField(FieldJson& data)
{
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
	return field;
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
	m_figure->m_figureState = (FigureState) data.figureState;
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

std::shared_ptr<City> CityFactory::m_createCity(CityJson& data, std::istream& is){
	//figures and nations and traderoutes are missing
	std::shared_ptr<City> whatToReturn = std::make_shared<City>();
	m_city = whatToReturn.get();
	m_city->m_buyInTurn = data.buyInTurn;
	m_city->m_food = data.food;
	m_city->m_shields = data.shields;
	for(int improvementData: data.improvements){
		CityImprovement temp{(ImprovementType)improvementData,m_city};
		m_city->m_improvements.push_back(temp);
	}
	m_city->m_globalIndex = data.globalIndex;
	m_city->m_industrialPollutionCoefficient = data.industrialPollutionCoefficient;
	m_city->m_name = data.name;
	m_city->m_wonderContent = data.wonderContent;
	m_city->m_wonderHappy = data.wonderHappy;
	for(std::map<std::string, int>& fogData: data.nationFogInfo){
		NationKnowsCity temp{fogData["size"],(bool)fogData["noUnits"],(bool)fogData["hasCityWalls"]
	,fogData["yearNumberRaw"],(Nationality)fogData["nationality"],(Nationality)fogData["whoKnowsThisCity"]};
		m_city->m_nationFogInfo.push_back(temp);
	}
	return whatToReturn;
}

std::shared_ptr<Citizen> CityFactory::m_createCitizen(CitizenJson& data){
	std::shared_ptr<Citizen> whatToReturn = std::make_shared<Citizen>(*m_city,(*theContainer->m_getFieldsOfTheWorld())[data.x/STANDARD_FIELD_SIZE][data.y/STANDARD_FIELD_SIZE].get());
	return whatToReturn;
}

std::shared_ptr<Nation> NationFactory::m_createNation(NationJson& data){
	//embassies missing, cities missing, figures missing
	std::shared_ptr<Nation> whatToReturn = std::make_shared<Nation>();
	whatToReturn->m_difficulty = (Difficulty) data.difficulty;
	whatToReturn->m_directlyMakingFiguresActive = false;
	whatToReturn->m_explorationProgress = data.explorationProgress;
	for(int tech: data.technologies){
		whatToReturn->m_exploredTechnologies.push_back((Technology)tech);
	}
	whatToReturn->m_government = (GovernmentType) data.government;
	whatToReturn->m_leaderName = data.leaderName;
	whatToReturn->m_luxuriesRate = data.luxuriesRate;
	whatToReturn->m_nation = (Nationality)data.nation;
	whatToReturn->m_taxRate = data.taxRate;
	whatToReturn->m_treasury = data.treasury;
	whatToReturn->m_whatToExplore = (Technology) data.whatToExplore;
	return whatToReturn;
}

WonderData GameFactory::m_createWonderData(Json& j){
	//cities missing
	return WonderData{
		(ImprovementType)j["imptype"],(Nationality)j["who"],j["isDestroyed"],j["isObsolete"],nullptr
	};
}

Game* GameFactory::m_createGame(GameJson data){
	Game* whatToReturn = new Game();
	whatToReturn->m_climateState = (ClimateState)data.climateState;
	whatToReturn->m_currentYear = Game::Year(data.yearNumber);
	whatToReturn->m_pollutionCount = data.pollutionCount;
	whatToReturn->m_currentYear.m_randomSeed = data.randomSeed;
	whatToReturn->m_nationAtCurrentTurnIndex = data.nationAtCurrentTurnIndex;
	return whatToReturn;
}

GameMain* GameFactory::m_createGameMain(GameMainJson& data){
	//whatToMove missing
	GameMain* whatToReturn = new GameMain(LOAD_GAME);
	if(!theWindow) throw std::runtime_error("theWindow is nullptr!");
	whatToReturn->m_theWindows.push_back(std::unique_ptr<Window>(theWindow));
	whatToReturn->m_currentRenderer = theRenderer;
	whatToReturn->m_blinkingIntervalTime =  (BlinkingTime)data.blinkingIntervalTime;
	whatToReturn->m_topLeftCorner = Coordinate(data.topLeftX,data.topLeftY);
	return whatToReturn;
}
