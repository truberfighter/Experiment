/*
 * GameSaver.cpp
 *
 *  Created on: 14.01.2023
 *      Author: uwe-w
 */

#include "GameSaver.hpp"
#include "includeJson/single_include/nlohmann/json.hpp"
#include "FieldContainer.hpp"
#include "Field.hpp"
#include "Figure.hpp"
#include "Embassy.hpp"
#include <algorithm>
#include <exception>
#include "Plane.hpp"
#include "City.hpp"
#include "Game.hpp"
#include "GameMain.hpp"
#include "GameLoader.hpp"
#include "Settlers.hpp"
#include "Ship.hpp"

using namespace GlobalWarming;
using nlohmann::json;
void GameSaver::m_saveGame(GameMain&  gameMain){
	m_stream<<m_slotname<<std::endl;
	m_printGlobalVariables();
	m_printGameMain();
	m_printNations();
	m_printFields();
	m_printFigures();
	m_printCities();
	m_printGame();
}

GameSaver::~GameSaver() {
}

void GameSaver::m_printGlobalVariables(){
	m_stream<<(int)Graphics::Civ::currentNationality<<std::endl;
	m_stream<<SWAMPS_TO_MAKE<<" "<<DESERTS_TO_MAKE<<" "<<WARMING_EXACERBATION<<" "<< DESERTIFICATION_BOUND <<" "<< HORIZONTAL_HOPPING_DISTANCE<<" "<< DESERTIFICATION_SPREADER<<" "<< TURNS_UNTIL_SWAMPED<<" ";
	m_stream<<(int)theGame->m_NationsPlaying().size()<<" ";
	m_stream<<slotNumber<<std::endl;
	m_stream<<figureCountGlobal<<" "<<cityCountGlobal<<std::endl;
}

void GameSaver::m_printFields(){
	std::string fileString = m_filename+"Field.json";

	std::ofstream outputStream1(fileString,std::ofstream::out|std::ofstream::trunc);
	if(!static_cast<bool>(outputStream1))
		throw FileFail{fileString};
	outputStream1.close();
	std::fstream outputStream2(fileString);
	for(int x(0); x<WORLD_LENGTH;x++){
		std::cout<<"Meridian"<<std::endl;
		for(int y(0);y<WORLD_HEIGHT;y++){
			auto& meridian = (*theContainer->m_getFieldsOfTheWorld())[x];
			m_printAField(*meridian[y].get(),outputStream2);
			outputStream2<<std::endl;
		}
	}
	std::fstream f(fileString);
	Field* ff;
}

void GameSaver::m_printAField(Field& field, std::ostream& os){
	FieldJson data = field.m_createJson();
	json theJson;
	PUSH("x",data.x)
	PUSH("y",data.y)
	PUSH("landscape",(int)data.landscape)
	PUSH("hasSpecialResource",data.hasSpecialResource)
	PUSH("hasShield",data.hasShield)
	PUSH("hasFortress",data.hasFortress)
	PUSH("roadStatus",data.roadStatus)
	PUSH("isMined",data.isMined)
	PUSH("isPolluted",data.isPolluted)
	PUSH("isIrrigated",data.isIrrigated)
	PUSH("continentID", data.continentID)
	PUSH("layer",data.layer)
	PUSH("turnsUntilSwamped",data.turnsUntilSwamped)
	PUSH("nationKnowsData",data.nationKnowsData)
	PUSH("figureIDs",data.figureIDs)
	os<<theJson;
}

NationJson Nation::m_createJson(){
	std::vector<int> technologies;
	for(Technology tech: m_exploredTechnologies){
		technologies.push_back((int) tech);
	}
	std::vector<int> actives, all;
	for(std::shared_ptr<Figure> currentFigure: m_activeFigures){
		actives.push_back(currentFigure->m_FigureID());
	}
	for(std::shared_ptr<Figure> currentFigure: m_figures){
		all.push_back(currentFigure->m_FigureID());
	}
	std::vector<int> embassies;
	for(Embassy& emb: m_embassies){
		embassies.push_back((int)emb.m_Nation()->m_Nation());
	}
	std::vector<int> foundedCityIDs,cityIDs;
	for(std::shared_ptr<City> currentCity: m_foundedCities){
		foundedCityIDs.push_back(currentCity->m_GlobalIndex());
	}
	for(std::shared_ptr<City> currentOwnCity: m_cities){
		cityIDs.push_back(currentOwnCity->m_GlobalIndex());
	}
	return NationJson{
(int) m_government, (int) m_nation,m_leaderName, actives,all,m_taxRate,m_luxuriesRate,m_treasury,technologies,
(int) m_whatToExplore, m_explorationProgress,(int) m_difficulty,embassies,foundedCityIDs,cityIDs
	};
}
void from_json(NationJson& nj, json& j){
	try{
		nj.government = j.at("government");
		nj.difficulty = j.at("difficulty");
		AT(nj.nation,"nation")
		GET(nj.figureIDs,"figureIDs")
		GET(nj.otherFigureIDs,"otherFigureIDs")
		AT(nj.taxRate,"taxRate")
		AT(nj.luxuriesRate,"luxuriesRate")
		AT(nj.treasury,"treasury")
		GET(nj.technologies,"technologies")
		AT(nj.whatToExplore,"whatToExplore")
		AT(nj.explorationProgress,"explorationProgress")
		GET(nj.embassies,"embassies")
	}
	catch(std::exception& e){
		throw FileFail();
	}
}

void GameSaver::m_printNations(){
	std::string fileString = m_filename+"Nation.json";
	std::ofstream outputStream1(fileString,std::ofstream::out|std::ofstream::trunc);
	if(!static_cast<bool>(outputStream1))
		throw FileFail{fileString};
	outputStream1.close();
	std::fstream outputStream2(fileString);
	for(std::shared_ptr<Nation> currentNation: theGame->m_NationsPlaying()){
		m_printANation(*currentNation,outputStream2);
	}
}

void GameSaver::m_printANation(Nation& nation, std::ostream& os){
	NationJson data = nation.m_createJson();
	json theJson;
	PUSH("government",data.government)
	PUSH("nation",data.nation)
	PUSH("leaderName",data.leaderName)
	PUSH("figureIDs",data.figureIDs)
	PUSH("otherFigureIDs",data.otherFigureIDs)
	PUSH("taxRate",data.taxRate)
	PUSH("luxuriesRate",data.luxuriesRate)
	PUSH("treasury",data.treasury)
	PUSH("technologies",data.technologies)
	PUSH("whatToExplore",data.whatToExplore)
	PUSH("explorationProgress",data.explorationProgress)
	PUSH("difficulty",data.difficulty)
	PUSH("embassies",data.embassies)
	PUSH("foundedCityIDs",data.foundedCityIDs)
	PUSH("cityIDs",data.cityIDs)
	os<<theJson<<std::endl;
	std::cout<<theJson<<std::endl;
}

void Figure::m_printData(std::ostream& os){
	json theJson;
	PUSH("figureID",m_figureID)
	std::vector<int> v;
	for(Nationality n: m_visibilityInfo){
		v.push_back((int)n);
	}
	PUSH("visibilityInfo",v)
	PUSH("movementPoints",m_movementPoints.m_movementPoints)
	PUSH("isVeteran",m_isVeteran)
	PUSH("figureState",m_figureState)
	PUSH("figureType",m_FigureType())
	os<<theJson<<std::endl;
	std::cout<<theJson<<std::endl;
}

void Plane::m_printData(std::ostream& os){
	Figure::m_printData(os);
	json theJson;
	PUSH("turnsFlying",m_turnsFlying)
	os<<theJson<<std::endl;
	std::cout<<theJson<<std::endl;
}

void Settlers::m_printData(std::ostream& os){
	Figure::m_printData(os);
	json theJson;
	PUSH("workStepsCount",(int)m_workStepsCount)
	PUSH("currentWork",m_currentWork)
	os<<theJson<<std::endl;
	std::cout<<theJson<<std::endl;
}

void Ship::m_printData(std::ostream& os){
	Figure::m_printData(os);
	json theJson;
	PUSH("mayBombardGroundTroops",m_mayBombardGroundTroops)
	os<<theJson<<std::endl;
	std::cout<<theJson<<std::endl;
}

void GameSaver::m_printFigures(){
	std::string fileString = m_filename+"Figures.json";
	std::ofstream outputStream1(fileString,std::ofstream::out|std::ofstream::trunc);
	if(!static_cast<bool>(outputStream1))
		throw FileFail{fileString};
	outputStream1.close();
	std::fstream outputStream2(fileString);
	for(std::shared_ptr<Nation> currentNation: theGame->m_NationsPlaying()){
		for(std::shared_ptr<Figure> currentFigure: currentNation->m_Figures()){ //barbarions? Not fully solved
			currentFigure->m_printData(outputStream2);
		}
	}
}

CityJson City::m_createJson(){
	std::vector<int> figureIDs, improvements, tradeRoutePartners;
	for(std::shared_ptr<Figure>& currentFigure: m_figuresOwned){
		figureIDs.push_back(currentFigure->m_FigureID());
	}
	for(auto& currentImp: m_improvements){
		improvements.push_back((int)currentImp.m_what);
	}
	for(std::shared_ptr<TradeRoute>& currentRoute: m_tradeRoutes){
		City* cityOfInterest = currentRoute->m_city1.get() == this ? currentRoute->m_city2.get() : currentRoute->m_city1.get();
		tradeRoutePartners.push_back(cityOfInterest->m_globalIndex);
	}
	std::vector<std::map<std::string, int>> nationFogInfos;
	for(NationKnowsCity& nkc: m_nationFogInfo){

		std::map<std::string, int> nationFogInfo =
		{
{"size",nkc.size},{"noUnits",(int)nkc.noUnits},{"hasCityWalls",(int)nkc.hasCityWalls},{"yearNumberRaw",nkc.yearNumberRaw},{"home",(int)nkc.home},{"whoKnowsThisCity",(int)nkc.whoKnowsThisCity}
		};
		nationFogInfos.push_back(nationFogInfo);
	}
	return CityJson{
		m_buyInTurn,nationFogInfos,(int)m_whatIsBuilt,m_OwningNation()->m_Nation(),
		m_size(),m_globalIndex,improvements,figureIDs,m_name,m_whereItStands->m_X(),m_whereItStands->m_Y(),
		m_shields,m_food,tradeRoutePartners,m_wonderHappy,m_wonderContent,m_industrialPollutionCoefficient
	};

}

void GameSaver::m_printCities(){
	std::string fileString = m_filename+"Cities.json";
	std::ofstream outputStream1(fileString,std::ofstream::out|std::ofstream::trunc);
	if(!static_cast<bool>(outputStream1))
		throw FileFail{fileString};
	outputStream1.close();
	std::fstream outputStream2(fileString);
	for(std::shared_ptr<City> currentCity: theGame->m_AllCities()){
		m_printACity(*currentCity,outputStream2);
	}
}

void GameSaver::m_printACity(City& city, std::ostream& os){
	CityJson data = city.m_createJson();
	json theJson;
	PUSH("size",data.size)
	PUSH("globalIndex",data.globalIndex)
	PUSH("improvements",data.improvements)
	PUSH("figureIDs",data.figureIDs)
	PUSH("name",data.name)
	PUSH("x",data.x)
	PUSH("y",data.y)
	PUSH("shields",data.shields)
	PUSH("food",data.food)
	PUSH("tradeRoutePartners",data.tradeRoutePartners)
	PUSH("wonderHappy",data.wonderHappy)
	PUSH("wonderContent",data.wonderContent)
	PUSH("industrialPollutionCoefficient",data.industrialPollutionCoefficient)
	PUSH("buyInTurn",data.buyInTurn)
	PUSH("nationFogInfo",data.nationFogInfo)
	PUSH("whatIsBuilt",data.whatIsBuilt)
	PUSH("nationality",data.nationality)
	os<<theJson<<std::endl;
	for(std::shared_ptr<Citizen>& citizen: city.m_Citizens()){
		m_printACitizen(*citizen,os);
	}
}

void GameSaver::m_printACitizen(Citizen& citizen, std::ostream& os){
	json theJson;
	CitizenJson data = citizen.m_createJson();
	PUSH("x",data.x)
	PUSH("y",data.y)
	PUSH("homeIndex",data.homeIndex)
	PUSH("state",data.state)
	os<<theJson<<std::endl;
}

CitizenJson Citizen::m_createJson(){
	return CitizenJson{
	m_whereItWorks? m_whereItWorks->m_X() : -1,m_whereItWorks? m_whereItWorks->m_Y() : -1,m_home.m_GlobalIndex(),(int)m_state
	};
}

GameJson Game::m_createJson(){
	std::vector<int> nationalities;
	for(std::shared_ptr<Nation>& currentNation: m_nationsPlaying){
		nationalities.push_back((int)currentNation->m_Nation());
	}
	return GameJson{
m_currentYear.m_yearNumberRaw,m_currentYear.m_randomSeed,m_hasWonderBeenBuilt.size(),m_nationAtCurrentTurnIndex,(int)m_climateState,m_pollutionCount,nationalities
	};
}

void GameSaver::m_printGame(){
	std::string fileString = m_filename+"Game.json";
	std::ofstream outputStream1(fileString,std::ofstream::out|std::ofstream::trunc);
	if(!static_cast<bool>(outputStream1))
		throw FileFail{fileString};
	outputStream1.close();
	std::fstream outputStream2(fileString);
	GameJson data = theGame->m_createJson();
	json theJson;
	PUSH("yearNumber",data.yearNumber)
	PUSH("randomSeed",data.randomSeed)
	PUSH("wonderDataSize",data.wonderDataSize)
	PUSH("nationAtCurrentTurnIndex",data.nationAtCurrentTurnIndex)
	PUSH("climateState",data.climateState)
	PUSH("pollutionCount",data.pollutionCount)
	PUSH("nationalities",data.nationalities)
	outputStream2<<theJson<<"\n";
	for(const WonderData& wonderData: theGame->m_HasWonderBeenBuilt()){
		json theJson;
		PUSH("imptype",wonderData.what)
		PUSH("who",wonderData.who)
		PUSH("isDestroyed",wonderData.isDestroyed)
		PUSH("isObsolete",wonderData.isObsolete)
		outputStream2<<theJson<<"\n";
	}
	outputStream2.flush();
	std::cout<<theJson<<std::endl;
}

GameMainJson GameMain::m_createJson(){
	return GameMainJson{
		m_topLeftCorner.x,m_topLeftCorner.y,m_whatToMove ? m_whatToMove->m_FigureID() : -1, m_blinkingIntervalTime
	};
}
void GameSaver::m_printGameMain(){
	std::string fileString = m_filename+"GameMain.json";
	std::ofstream outputStream1(fileString,std::ofstream::out|std::ofstream::trunc);
	if(!static_cast<bool>(outputStream1))
		throw FileFail{fileString};
	outputStream1.close();
	std::fstream outputStream2(fileString);
	GameMainJson data = m_gameMain.m_createJson();
	json theJson;
	PUSH("topLeftX",data.topLeftX)
	PUSH("topLeftY",data.topLeftY)
	PUSH("whatToMove",data.whatToMove)
	PUSH("blinkingIntervalTime",data.blinkingIntervalTime)
	outputStream2<<theJson<<std::endl;
}
