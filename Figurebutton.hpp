/*
 * Figurebutton.hpp
 *
 *  Created on: 21.05.2022
 *      Author: uwe-w
 */
#include "Drawing.hpp"
#include "Field.hpp"

class Figurebutton{
private:
	std::list<std::shared_ptr<Figure>> m_figureList;
public:
	Figurebutton(std::list<std::shared_ptr<Figure>> figureList);
	std::shared_ptr<Figure> operator()();
	bool mouseClickedOnSubbutton(std::shared_ptr<Figure>);
};


