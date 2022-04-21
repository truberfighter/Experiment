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
	FigureType m_FigureType() override;
	void m_loseOneWorkPoint();
	Settlers(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	~Settlers();
	bool m_takeOrder(char order) ;
	short int m_WorkStepsCount();
	int m_drawSettlersWork(SDL_Rect& rectToDraw);
	void m_work();
	void m_work(SettlersWork work);
	void m_finishWork(); //set m_workStepsCount to 0
	SettlersWork m_CurrentWork();
	virtual std::string m_orderOverview();
	virtual float m_attackingStrength();
	float m_defensiveStrength();
	short unsigned int m_visibilityRange();
	void m_finishMove() override;
	short unsigned int m_defaultMovementPoints();
	FigureCategory m_FigureCategory()override;
	void m_foundNewCity(std::string name);
	void m_addToCity(std::string name);
	std::shared_ptr<MovableThing> m_createImage() override;
	bool m_canBuildRailroad();
	bool m_canBuildBridges();
};



#endif /* SETTLERS_HPP_ */
