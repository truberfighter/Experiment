/*
 * ButtonElement.hpp
 *
 *  Created on: 27.06.2022
 *      Author: uwe-w
 */

#ifndef BUTTONELEMENT_HPP_
#define BUTTONELEMENT_HPP_

#include "Drawing.hpp"

class ButtonElement : public DrawingElement {
private:
	unsigned int m_width;
	unsigned int m_height;
	SDL_Color m_backgroundColor = Graphics::Civ::brightCityBackgroundColor();
	SDL_Color m_fontColor = Graphics::Civ::cityBackgroundColor();
	std::string m_name;
public:
	ButtonElement(int row, int column, unsigned int width, unsigned int height, SDL_Color backgroundColor, SDL_Color fontColor, std::string name, SDL_Renderer* renderer = theRenderer);
	bool m_isClicked(unsigned int x, unsigned int y);
	virtual ~ButtonElement();
	int m_draw();
	virtual Drawing_Element m_DrawingElement();
	virtual bool m_equals(DrawingElement& comparedDrEl);
	virtual bool m_updatePosition();
	virtual int m_draw(int rowShift, int columnShift, SDL_Renderer* renderer = nullptr);
};

#endif /* BUTTONELEMENT_HPP_ */
