/*
 * Ocean.hpp
 *
 *  Created on: 10.04.2022
 *      Author: uwe-w
 */

#include "Field.hpp"
#ifndef OCEAN_HPP_
#define OCEAN_HPP_

#include "Field.hpp"
#include "Settlers.hpp"

class Ocean: public Field{
private:
	bool m_hasSpecialResource;
public:
	~Ocean();
	Ocean(int x, int y, bool hasSpecialResource = false);
	MovementPoints m_movementPoints() override;
	float m_defenseBonus() override;
	bool m_Mining(Settlers& settlers) override;
	bool m_Irrigate(Settlers& settlers) override;
	int m_shields() override;
	int m_food() override;
	int m_trade() override;
	std::string m_resourceOverview()override;
	Landscape m_Landscape() const override;
	short int m_howLongToTake(SettlersWork work) override;
};



#endif /* OCEAN_HPP_ */
