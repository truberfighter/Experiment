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
#include "ButtonElement.hpp"
#include <functional>

class City;
class Subsurface;


class CitySurface{
private:
	City* m_associatedCity;
	std::shared_ptr<Subsurface> m_subsurface;
	std::vector<std::shared_ptr<ButtonElement>> m_sellingElements;
	int m_indexForImprovementOverview =0;
public:
	CitySurface(City* city);
	bool m_handleEvent(const SDL_Event& event) ;
	bool m_handleKeyboardEvent(const SDL_Event& event) ;
	~CitySurface(){}
	bool m_handleLeftClick(const SDL_MouseButtonEvent& event);
	bool m_handleSellingButtonClick(int effectiveIndex);
	void m_drawSurface(SDL_Renderer* renderer);
	void m_drawSubsurfaceButtons();
	void m_drawImprovementBackground();
	void m_displaySurface(SDL_Renderer* renderer);
	void m_drawCitizens(SDL_Renderer* renderer, int x, int y, int backgroundWidth = CITIZENS_OVERVIEW_WIDTH, int backgroundHeight = CITIZENS_OVERVIEW_HEIGHT, HappyVectorType flag = HAPPY_ALL);
	void m_drawFoodProduction();
	void m_drawShieldProduction();
	void m_drawTradeProduction();
	void m_drawRevenueProduction();
	bool m_change();
	bool m_sell();
	bool m_buy();
	void m_drawProduction();
	void m_drawFoodStorage();
	void m_drawCityFields();
	void m_drawFigures();
	void m_drawShields();
	void m_drawShieldOverview();
	bool m_changeWhatIsBuilt();
	bool m_handleCitizenClick(const SDL_MouseButtonEvent& event);
	void m_createSellingButtonElements();
	void m_drawSellingOverview();
	friend class Subsurface;
};

class ImprovementRightClick: public std::function<void()>{
public:
	void operator()();
	std::string m_filename;
	ImprovementRightClick(std::string name = "");
	ImprovementRightClick(ImprovementType imptype);
};

extern ButtonElement changeButton;
extern ButtonElement buyButton;

#endif /* CITYSURFACE_HPP_ */
