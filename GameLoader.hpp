/*
 * GameLoader.hpp
 *
 *  Created on: 29.01.2023
 *      Author: uwe-w
 */

#ifndef GAMELOADER_HPP_
#define GAMELOADER_HPP_

#include "Game.hpp"
#include "includeJson/single_include/nlohmann/json.hpp"
#include <fstream>
#include <map>
struct FieldJson; struct FigureJson; struct GameMainJson; struct GameJson; struct CityJson; struct CitizenJson;
struct CitizenJson; class Citizen; struct NationJson; class GameMain;
class AdditionalLoadingInfo{
public:
	std::string type;
	AdditionalLoadingInfo(std::string string):type(string){}
	virtual ~AdditionalLoadingInfo(){std::cout<<"AdditionalLoadingInstructionsdestruktor: this = "<<this<<std::endl;}
};
class GameLoader{
private:
	std::ifstream nationStream,cityStream,gameStream,gameMainStream, fieldStream,figureStream;
	int m_slotNumber;
	int m_nationsPlayingCount = -1;
	std::string m_basicFileString;
	Game* m_tempGame = nullptr;
	std::map<int,std::shared_ptr<City>> m_cities;
	std::map<int,std::shared_ptr<Figure>> m_figures; //maps figureids to figures
	std::vector<Meridian> m_fields;
	std::map<Nationality,std::shared_ptr<Nation>> m_nations;
public:
	GameMain* m_load();
	GameLoader(int slotNumber):m_slotNumber(slotNumber){}
	Field* m_loadField(std::istream& os);
	std::shared_ptr<Figure> m_createFigure(std::istream& os);
	std::shared_ptr<City> m_createCity(CityJson data, std::istream& os);
	std::shared_ptr<Citizen> m_createCitizen(std::istream& os, City* city);
	std::shared_ptr<Nation> m_createNation(NationJson data);
	Game* m_createTheGame(std::istream& is);
	GameMain* m_createTheGameMain(std::istream& is);
	GameMainJson m_loadGameMain(std::istream& is);
	NationJson m_loadNation(std::istream& is);
	CityJson m_loadCity(std::istream& is);
	GameJson m_loadGame(std::istream& is);
	void m_readGlobalVariables(std::istream& is);
	void m_createNations(std::istream& is);
	void m_createFields(std::istream& is);
	void m_createFigures(std::istream& is);
	void m_createCities(std::istream& is);
	void m_makeRemainingConnections();
};

void parseJson(nlohmann::json& j, std::istream& is);


#endif /* GAMELOADER_HPP_ */
