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
struct FieldJson; struct FigureJson; struct GameMainJson; struct GameJson; struct CityJson; struct CitizenJson;
class AdditionalLoadingInfo{
public:
	std::string type;
	AdditionalLoadingInfo(std::string string):type(string){}
	virtual ~AdditionalLoadingInfo(){std::cout<<"AdditionalLoadingInstructionsdestruktor: this = "<<this<<std::endl;}
};
class GameLoader{
private:
	std::fstream& m_stream;
	std::string m_filename;
	int m_slotNumber;
public:
	GameLoader(std::fstream& stream, std::string filename):m_stream(stream),m_filename(filename),m_slotNumber(slotNumber){}
	std::shared_ptr<Field> m_loadField(std::istream& os);
	std::shared_ptr<Figure> m_createFigure(std::istream& os);
};



#endif /* GAMELOADER_HPP_ */
