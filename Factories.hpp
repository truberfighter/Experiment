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
class Field; struct FieldJson;
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
	std::shared_ptr<Field> m_createField(FieldJson& fj);
};

extern FigureFactory theFigureFactory; extern FieldFactory theFieldFactory;
#endif /* FACTORIES_HPP_ */
