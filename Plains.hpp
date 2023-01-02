/*
 * Plains.hpp
 *
 *  Created on: 12.03.2022
 *      Author: uwe-w

#ifndef PLAINS_HPP_
#define PLAINS_HPP_

#include "Field.hpp"
#include "Settlers.hpp"

class Plains: public Field{
private:
public:
	~Plains();
	Plains(int x, int y, bool hasSpecialResource = false);
	MovementPoints m_movementPoints() override;
	float m_defenseBonus() override;
	bool m_Mining(Settlers& settlers) override;
	bool m_Irrigate(Settlers& settlers) override;
	int m_shields() override;
	int m_food() override;
	int m_trade() override;
	std::string m_resourceOverview()override;
	Landscape m_Landscape() const override;
};



#endif /* PLAINS_HPP_ */

