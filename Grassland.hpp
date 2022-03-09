/*
 * Grassland.hpp
 *
 *  Created on: 08.03.2022
 *      Author: uwe-w
 */

#ifndef GRASSLAND_HPP_
#define GRASSLAND_HPP_

#include "Field.hpp"
#include "Settlers.hpp"

class Grassland: public Field{
private:
	bool m_hasShield;
public:
	~Grassland();
	Grassland(int x, int y, bool hasShield = false);
	int m_movementPoints() override;
	float m_defenseBonus() override;
	bool m_Mining(Settlers& settlers) override;
	bool m_Irrigate(Settlers& settlers) override;
	int m_shields(Nation& nationToGetShields);
	int m_shields() override;
	int m_food(Nation& nationToGetFood);
	int m_food() override;
	int m_trade(Nation& nationToGetTrade);
	int m_trade() override;
	std::string m_resourceOverview()override;
	Landscape m_Landscape() override;
	short int m_howLongToTake(SettlersWork work) override;
};



#endif /* GRASSLAND_HPP_ */
