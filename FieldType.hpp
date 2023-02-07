/*
 * FieldType.hpp
 *
 *  Created on: 31.12.2022
 *      Author: uwe-w
 */
#include "sdltypes.hpp"
#include <memory>

#ifndef FIELDTYPE_HPP_
#define FIELDTYPE_HPP_

struct FieldTypeDecorator{
std::function<void(Field*)> m_whatToDo;
};

struct MovementData{
	double defenseBonus;
	MovementPoints m_movementCost;
};
struct FieldType{
	Landscape m_landscape;
	FieldTypeDecorator m_irrigator;
	FieldTypeDecorator m_miningMaker;
	std::function<short int (SettlersWork work, Field*)> m_howLongToTake;
	std::function<int(Field*)> m_food, m_shields, m_trade;
	MovementData m_movementData;
	std::shared_ptr<Texture> m_texture;
	std::shared_ptr<Texture> m_resourceTexture = nullptr;
	std::shared_ptr<Texture> m_shieldTexture = nullptr;
};




#endif /* FIELDTYPE_HPP_ */
