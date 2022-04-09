/*
 * Player.hpp
 *
 *  Created on: 27.03.2022
 *      Author: uwe-w
 */

#ifndef PLAYER_HPP_
#define PLAYER_HPP_

#include "Nation.hpp"

class Player{
private:
	std::shared_ptr<Nation> m_nation;
public:
	const std::shared_ptr<Nation> m_Nation();
	void m_setNation(std::shared_ptr<Nation> nation){m_nation = nation;}
};



#endif /* PLAYER_HPP_ */
