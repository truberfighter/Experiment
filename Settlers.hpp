/*
 * Settlers.hpp
 *
 *  Created on: 08.03.2022
 *      Author: uwe-w
 */

#ifndef SETTLERS_HPP_
#define SETTLERS_HPP_

#include "sdltypes.hpp"
#include "Figure.hpp"

class Nation;
class City;

class Settlers: public Figure{
private:
	short int m_workStepsCount;
	SettlersWork m_currentWork = NONE;
public:
	bool m_startFoundingNewCity();
	short int m_WorkStepsCount();
	void m_work();
	void m_work(SettlersWork work);
	void m_finishWork(); //set m_workStepsCount to 0
	SettlersWork m_CurrentWork();
	int m_drawSettlersWork(SDL_Rect& rectToDraw);
	void m_loseOneWorkPoint();
	FigureType m_FigureType() override;
	Settlers(Field* whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	Settlers(){}
	~Settlers();
	bool m_takeOrder(char order) ;
	virtual std::string m_orderOverview();
	virtual float m_attackingStrength();
	float m_defensiveStrength();
	short unsigned int m_visibilityRange();
	void m_finishMove() override;
	short unsigned int m_defaultMovementPoints();
	FigureCategory m_FigureCategory()override;
	std::shared_ptr<City> m_foundNewCity(std::string name);
	int m_shieldCost();
	std::shared_ptr<MovableThing> m_createImage() override;
	bool m_addToCity();
	bool m_canBuildRailroad();
	bool m_canBuildBridges();
	virtual void m_printData(std::ostream& os) override;
	friend class Figurebutton; friend class FigureFactory;
	friend void Figure::m_changeNationTo(std::shared_ptr<Nation>, std::shared_ptr<City>);
};



#endif /* SETTLERS_HPP_ */
