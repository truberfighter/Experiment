/*
 * Nation.hpp
 *
 *  Created on: 09.03.2022
 *      Author: uwe-w
 */

#ifndef NATION_HPP_
#define NATION_HPP_

#include "Settlers.hpp"

class Nation{
private:
	GovernmentType m_government = DESPOTISM;
public:
	GovernmentType m_Government();
};

inline GovernmentType Nation::m_Government(){
	return m_government;
}

#endif //NATION_HPP_
