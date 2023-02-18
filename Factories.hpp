/*
 * Factories.hpp
 *
 *  Created on: 04.02.2023
 *      Author: uwe-w
 */

#ifndef FACTORIES_HPP_
#define FACTORIES_HPP_

#include "Figure.hpp"
#include <stdexcept>
#include "includeJson/single_include/nlohmann/json.hpp"
class Field; struct FieldJson; struct NationJson; struct CityJson; struct CitizenJson; class Citizen;
class Game; struct GameJson; struct WonderData; class GameMain; struct GameMainJson;
class FigureFactory{
private:
	Figure* m_figure = nullptr;
public:
	std::shared_ptr<Figure> m_createFigure(FigureJson& fj);
	std::shared_ptr<Figure> m_createFigure(FigureJson& fj, nlohmann::json& j);
	void m_editRawFigure(FigureJson& data);
};

class IstreamNeeded: public std::exception{
public: virtual const char* what() const noexcept override{return "the given data is not sufficient without a further json\n";}
};
class IstreamNotNeeded: public std::exception{
public: virtual const char* what() const noexcept override{return "the given data is sufficient without a further json\n";}
};

class InvalidFigureType: public std::exception{
public: virtual const char* what() const noexcept override{return "That figure type is yet to be implemented\n";}
};

class FieldFactory{
public:
	Field* m_createField(FieldJson& fj);
};

class CityFactory{
private: City* m_city = nullptr;
public:
	std::shared_ptr<Citizen> m_createCitizen(CitizenJson& data);
	std::shared_ptr<City> m_createCity(CityJson& data, std::istream& is);
};

class NationFactory{
public:
	std::shared_ptr<Nation> m_createNation(NationJson& data);
};

class GameFactory{
public:
	Game* m_createGame(GameJson data);
	WonderData m_createWonderData(nlohmann::json& j);
	GameMain* m_createGameMain(GameMainJson& data);
};

extern FigureFactory theFigureFactory; extern FieldFactory theFieldFactory; extern CityFactory theCityFactory;
extern NationFactory theNationFactory; extern GameFactory theGameFactory;
#endif /* FACTORIES_HPP_ */
