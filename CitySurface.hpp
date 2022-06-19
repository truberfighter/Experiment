/*
 * CitySurface.hpp
 *
 *  Created on: 24.05.2022
 *      Author: uwe-w
 */

#ifndef CITYSURFACE_HPP_
#define CITYSURFACE_HPP_
#include "sdltypes.hpp"
#include "EventHandler.hpp"

class City;
class Subsurface;


class CitySurface{
private:
	City* m_associatedCity;
	std::shared_ptr<Subsurface> m_subsurface;
public:
	CitySurface(City* city);
	bool m_handleEvent(const SDL_Event& event) ;
	bool m_handleKeyboardEvent(const SDL_Event& event) ;
	~CitySurface(){}
	bool m_handleLeftClick(const SDL_MouseButtonEvent& event);
	void m_drawSurface(SDL_Renderer* renderer);
	void m_drawSubsurfaceButtons();
	void m_displaySurface(SDL_Renderer* renderer);
	void m_drawCitizens(SDL_Renderer* renderer, int x, int y, int backgroundWidth = CITIZENS_OVERVIEW_WIDTH, int backgroundHeight = CITIZENS_OVERVIEW_HEIGHT, HappyVectorType flag = HAPPY_ALL);
	void m_drawFoodProduction();
	void m_drawShieldProduction();
	void m_drawTradeProduction();
	void m_drawProduction();
	void m_drawFoodStorage();
	void m_drawCityFields();
	void m_drawFigures();
	bool m_handleCitizenClick(const SDL_MouseButtonEvent& event);
	friend class Subsurface;
};



#endif /* CITYSURFACE_HPP_ */
