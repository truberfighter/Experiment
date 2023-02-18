/*
 * GameLoader.cpp
 *
 *  Created on: 29.01.2023
 *      Author: uwe-w
 */

#include "GameLoader.hpp"
#include "Game.hpp"
#include "GameMain.hpp"
#include "GameSaver.hpp"
#include "Field.hpp"
#include "Figure.hpp"
#include "GameMain.hpp"
#include "Factories.hpp"
#include "City.hpp"
#include "Embassy.hpp"
#include "FieldContainer.hpp"
using Json = nlohmann::json; using namespace GlobalWarming;

void parseJson(nlohmann::json& j, std::istream& is){
	try{
	std::string line;
	std::getline(is,line);
	std::cout<<line<<std::endl;
	j = nlohmann::json::parse(line);
	}
	catch(nlohmann::json_abi_v3_11_2::detail::parse_error& theException){
		throw theException;
	}
}

void GameLoader::m_readGlobalVariables(std::istream& is){
	if(!static_cast<bool>(is)){
		std::string errorString = "invalid istream: ";
		errorString = errorString + m_basicFileString;
		std::cerr<<errorString<<std::endl;
		throw std::runtime_error("invalid istream");
	}
	int tmp;
	is>>tmp; Graphics::Civ::currentNationality = (Nationality)tmp;
	is>>SWAMPS_TO_MAKE>>DESERTS_TO_MAKE>>WARMING_EXACERBATION>> DESERTIFICATION_BOUND >> HORIZONTAL_HOPPING_DISTANCE>> DESERTIFICATION_SPREADER>> TURNS_UNTIL_SWAMPED>>tmp;m_nationsPlayingCount=tmp;
	is>>tmp;slotNumber = tmp;
	is>>figureCountGlobal>>cityCountGlobal;
}

Field* GameLoader::m_loadField(std::istream& os){
	try{
	Json j;
	parseJson(j,os);
	FieldJson data;
	GET(data.figureIDs,"figureIDs")
	AT(data.continentID,"continentID")
	AT(data.x,"x")
	AT(data.y,"y")
	AT(data.landscape,"landscape")
	AT(data.hasSpecialResource,"hasSpecialResource")
	AT(data.hasShield,"hasShield")
	AT(data.hasFortress,"hasFortress")
	AT(data.roadStatus,"roadStatus")
	AT(data.isMined,"isMined")
	AT(data.isPolluted,"isPolluted")
	AT(data.isIrrigated,"isIrrigated")
	AT(data.layer,"layer")
	AT(data.turnsUntilSwamped,"turnsUntilSwamped")
	data.nationKnowsData = j["nationKnowsData"].get<std::vector<std::map<std::string,int>>>();
	Field* createdField = theFieldFactory.m_createField(data);
	for(int figureID: data.figureIDs){
		createdField->m_figuresOnField.push_back(m_figures[figureID]);
		m_figures[figureID]->m_whereItStands = createdField;
	}
	return createdField;
	}
	catch(nlohmann::json_abi_v3_11_2::detail::parse_error& theException){
		std::cout<<"catched in m_loadField"<<std::endl;
		throw theException;
	}
}
std::shared_ptr<Figure> GameLoader::m_createFigure(std::istream& os){
	Json j;
	parseJson(j,os);
	std::cout<<j;
	FigureJson data;
	AT(data.figureID,"figureID")
	GET(data.visibilityInfo,"visibilityInfo")
	AT(data.movementPoints,"movementPoints")
	AT(data.isVeteran,"isVeteran")
	AT(data.figureState,"figureState")
	AT(data.figureType,"figureType")
	try{
		return theFigureFactory.m_createFigure(data);
	}
	catch(IstreamNeeded& in){
		Json j2;
		parseJson(j2,os);
		std::cout<<j2<<std::endl;
		return theFigureFactory.m_createFigure(data, j2);
	}
}

CityJson GameLoader::m_loadCity(std::istream& is){
	Json j;
	parseJson(j,is);std::cout<<j<<std::endl;
	CityJson data;
	GET(data.figureIDs,"figureIDs")
	GET(data.tradeRoutePartners,"tradeRoutePartners")
	GET(data.improvements,"improvements")
	AT(data.food,"food")
	AT(data.globalIndex,"globalIndex")
	AT(data.name,"name")
	AT(data.shields,"shields")
	AT(data.size,"size")
	AT(data.x,"x")
	AT(data.y,"y")
	AT(data.wonderHappy,"wonderHappy")
	AT(data.wonderContent,"wonderContent")
	AT(data.industrialPollutionCoefficient,"industrialPollutionCoefficient")
	return data;
}

std::shared_ptr<City> GameLoader::m_createCity(CityJson data, std::istream& os){
	std::shared_ptr<City> whatToReturn = theCityFactory.m_createCity(data, os);
	whatToReturn->m_whereItStands = m_fields[data.x/STANDARD_FIELD_SIZE][data.y/STANDARD_FIELD_SIZE].get();
	for(int citizenIndex(0);citizenIndex<data.size;citizenIndex++){
		whatToReturn->m_Citizens().push_back(m_createCitizen(os,whatToReturn.get()));
		whatToReturn->m_WhereItStands()->m_cityContained = whatToReturn;
	}
	for(int figureID: data.figureIDs){
		whatToReturn->m_figuresOwned.push_back(m_figures[figureID]);
		m_figures[figureID]->m_home = whatToReturn;
	}
	whatToReturn->m_owningNation = m_nations[(Nationality)data.nationality];
	m_tempGame->m_allCities.push_back(whatToReturn);
	if(data.size>0){
		m_tempGame->m_citiesAlive.push_back(whatToReturn);
	}
	whatToReturn->m_owningNation->m_cities.push_back(whatToReturn);
	return whatToReturn;
}

std::shared_ptr<Citizen> GameLoader::m_createCitizen(std::istream& os, City* city){
	Json j;
	parseJson(j,os); std::cout<<std::endl;
	CitizenJson data;
	AT(data.homeIndex,"homeIndex")
	AT(data.state,"state")
	AT(data.x,"x")
	AT(data.y,"y")
	return theCityFactory.m_createCitizen(data);
}

NationJson GameLoader::m_loadNation(std::istream& is){
	Json j;
	parseJson(j,is);std::cout<<j<<std::endl;
	NationJson data;
	GET(data.embassies,"embassies")
	GET(data.figureIDs,"figureIDs")
	GET(data.otherFigureIDs,"otherFigureIDs")
	GET(data.technologies,"technologies")
	GET(data.foundedCityIDs,"foundedCityIDs")
	GET(data.cityIDs,"cityIDs")
	AT(data.difficulty,"difficulty")
	AT(data.explorationProgress,"explorationProgress")
	AT(data.government,"government")
	AT(data.leaderName,"leaderName")
	AT(data.nation,"nation")
	AT(data.taxRate,"taxRate")
	AT(data.luxuriesRate,"luxuriesRate")
	AT(data.treasury,"treasury")
	AT(data.whatToExplore,"whatToExplore")
	return data;
}

std::shared_ptr<Nation> GameLoader::m_createNation(NationJson data){
	std::shared_ptr<Nation> whatToReturn = theNationFactory.m_createNation(data);
	for(int figureID: data.figureIDs){
		whatToReturn->m_activeFigures.push_back(m_figures[figureID]);
	}
	for(int figureID: data.otherFigureIDs){
		m_figures[figureID]->m_nationality = whatToReturn;
		whatToReturn->m_figures.push_back(m_figures[figureID]);
	}
	for(int cityID: data.foundedCityIDs){
		whatToReturn->m_foundedCities.push_back(m_cities[cityID]);
	}
	for(int ownCityID: data.cityIDs){
		whatToReturn->m_cities.push_back(m_cities[ownCityID]);
		m_cities[ownCityID]->m_owningNation = whatToReturn;
	}

	return whatToReturn;
}

GameJson GameLoader::m_loadGame(std::istream& is){
	GameJson data;
	Json j;
	parseJson(j,is);std::cout<<j<<std::endl;
	AT(data.climateState,"climateState")
	AT(data.nationAtCurrentTurnIndex,"nationAtCurrentTurnIndex")
	AT(data.pollutionCount,"pollutionCount")
	AT(data.randomSeed,"randomSeed")
	AT(data.wonderDataSize,"wonderDataSize")
	AT(data.yearNumber,"yearNumber")
	GET(data.nationalities,"nationalities")
	return data;
}

Game* GameLoader::m_createTheGame(std::istream& is){
	GameJson data = m_loadGame(is);
	Game* whatToReturn = theGameFactory.m_createGame(data);
	for(unsigned int wonderDataIndex(0); wonderDataIndex<data.wonderDataSize;wonderDataIndex++){
		Json j2;
		parseJson(j2,is);
		std::cout<<j2<<std::endl;
		whatToReturn->m_hasWonderBeenBuilt.push_back(theGameFactory.m_createWonderData(j2));
	}
	m_tempGame = whatToReturn;
	std::string figureFileString = m_basicFileString+"Figures.json";
	figureStream = std::ifstream(figureFileString);
	m_createFigures(figureStream);
	std::string fieldFileString = m_basicFileString+"Field.json";
	fieldStream = std::ifstream(fieldFileString);
	m_createFields(fieldStream);
	std::string cityFileString = m_basicFileString+"City.json";
	cityStream = std::ifstream(cityFileString);
	m_createCities(cityStream);
	std::string nationFileString = m_basicFileString+"Nation.json";
	nationStream = std::ifstream(nationFileString);
	m_createNations(nationStream);
	m_makeRemainingConnections();
	//transfer the nations to the game. Cities are transferred when generated,
	if(!whatToReturn->m_nationsPlaying.empty()){
		throw std::runtime_error("nations in the game not empty");
	}
	for(int nationality: data.nationalities){
		whatToReturn->m_nationsPlaying.push_back(m_nations[(Nationality)nationality]);
	}
	return whatToReturn;
}

GameMain* GameLoader::m_createTheGameMain(std::istream& is){
	auto data = m_loadGameMain(is);
	GameMain* whatToReturn = theGameFactory.m_createGameMain(data);
	whatToReturn->m_basicInitGame();
	std::string gameString = m_basicFileString + "Game.json";
	std::fstream gameStream(gameString);
	Game* newGame = m_createTheGame(gameStream);
	whatToReturn->m_theGame.reset(newGame);
	whatToReturn->m_whatToMove = m_figures[data.whatToMove];
	FieldContainer::initFieldContainer(-1,-1);
	*theContainer->m_getFieldsOfTheWorld() = m_fields;
	theContainer->m_initFigureImages();
	theGame = newGame;
	whatToReturn->m_initWindow();
	return whatToReturn;
}

GameMainJson GameLoader::m_loadGameMain(std::istream& is){
	GameMainJson data;
	Json j;
	parseJson(j,is);std::cout<<j<<std::endl;
	AT(data.blinkingIntervalTime,"blinkingIntervalTime")
	AT(data.topLeftX,"topLeftX")
	AT(data.topLeftY,"topLeftY")
	AT(data.whatToMove,"whatToMove")
	return data;
}


GameMain* GameLoader::m_load(){
	std::stringstream nameStream;
	nameStream<<"data\\gameData"<<m_slotNumber<<".txt";	nameStream.flush();
	m_basicFileString = nameStream.str();
	std::string trash;
	std::fstream globalVariablesStream(nameStream.str()); std::getline(globalVariablesStream,trash);
	try{
	m_readGlobalVariables(globalVariablesStream);
	}
	catch(std::runtime_error& theException){
		if(std::strcmp(theException.what(),"invalid istream")==0){
			throw std::runtime_error("file corrupted");
		}
	}
	std::string gameMainFileName = m_basicFileString+"GameMain.json";
	std::fstream gameMainStream(gameMainFileName);
	GameMain* createdGameMain = m_createTheGameMain(gameMainStream);
	return createdGameMain;
}

void GameLoader::m_createNations(std::istream& is){
	for(int nationIndex(0);nationIndex<m_nationsPlayingCount;nationIndex++){
		std::cout<<"begin loading new nation"<<std::endl;
		NationJson data = m_loadNation(is);
		std::shared_ptr<Nation> newNation = m_createNation(data);
		m_nations[(Nationality)data.nation]=newNation;
	}
}

void GameLoader::m_createFields(std::istream& is){
	bool goodStream = true;
	std::string fieldString = m_basicFileString + "Field.json";
	std::ifstream fieldStream(fieldString);
	while(goodStream){
	try{
		Field* newField = m_loadField(fieldStream);
		if(newField->m_Y()==0){
			m_fields.push_back(Meridian());
			std::cout<<"Meridian pushed!"<<std::endl;
	std::cout<<"fields site: "<<m_fields.size()<<std::endl;
		}
		m_fields.back().push_back(std::shared_ptr<Field>(newField));
	}
	catch(nlohmann::json_abi_v3_11_2::detail::parse_error& exception){
		goodStream = false;
		std::cout<<"catched!"<<std::endl;
	}
	}
}

void GameLoader::m_createFigures(std::istream& is){
	bool goodStream = true;
	while(goodStream){
		try{
			std::shared_ptr<Figure> newFigure = m_createFigure(is);
			m_figures[newFigure->m_FigureID()]=(newFigure);
		}
		catch(nlohmann::json_abi_v3_11_2::detail::parse_error& exception){
			goodStream = false;
		}
	}
}

void GameLoader::m_createCities(std::istream& is){
	bool goodStream = true;
	std::string cityString = m_basicFileString + "Cities.json";
	std::ifstream cityStream(cityString);
	while(goodStream){
	try{
		CityJson data = m_loadCity(is);
		std::shared_ptr<City> newCity = m_createCity(data,is);
		m_cities[data.globalIndex]=(std::shared_ptr<City>(newCity));
	}
	catch(nlohmann::json_abi_v3_11_2::detail::parse_error& exception){
		goodStream = false;
	}
	}
}

void GameLoader::m_makeRemainingConnections(){
	nationStream.clear();
	nationStream.seekg(0);
	for(int nationIndex(0);nationIndex<m_nationsPlayingCount;nationIndex++){
		NationJson data = m_loadNation(nationStream);
		for(int embassyAim: data.embassies){
			m_nations[(Nationality)data.nation]->m_embassies.push_back(Embassy(m_nations[((Nationality)embassyAim)]));
		}
	}
	cityStream.clear();
	cityStream.seekg(0);
	bool goodCityStream = true;
	while(goodCityStream){
		try{
			CityJson data = m_loadCity(cityStream);
			std::shared_ptr<City> currentCity = m_cities[data.globalIndex];
			for(int partnerIndex: data.tradeRoutePartners){
				currentCity->m_tradeRoutes.push_back(std::make_shared<TradeRoute>(m_cities[data.globalIndex],m_cities[partnerIndex]));
			}
		}
		catch(nlohmann::json_abi_v3_11_2::detail::parse_error& exception){
			goodCityStream = false;
		}
	}
}
