/*
 * GameLoader.cpp
 *
 *  Created on: 29.01.2023
 *      Author: uwe-w
 */

#include "GameLoader.hpp"
#include "Game.hpp"
#include "GameSaver.hpp"
#include "Field.hpp"
#include "Figure.hpp"
#include "GameMain.hpp"
#include "Factories.hpp"
using Json = nlohmann::json;
std::shared_ptr<Field> GameLoader::m_loadField(std::istream& os){
	Json j;
	os>>j;
	FieldJson data;
	data.continentID = 6;
	data.x = 5;
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
	return theFieldFactory.m_createField(data);
}
std::shared_ptr<Figure> GameLoader::m_createFigure(std::istream& os){
	Json j;
	os>>j;
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
		os>>j2;
		return theFigureFactory.m_createFigure(data, j2);
	}
}
