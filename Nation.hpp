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
	Nationality m_nation;
public:
	Nation(Nationality n = ROMAN);
	GovernmentType m_Government();
	Nationality m_Nation() const;
	bool m_canBuildRailroad();
	bool m_canBuildBridges();
};

inline GovernmentType Nation::m_Government(){
	return m_government;
}

inline 	Nationality Nation::m_Nation() const{
	return m_nation;
}

#endif //NATION_HPP_
