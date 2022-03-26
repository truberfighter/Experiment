/*
 * Nation.cpp
 *
 *  Created on: 19.03.2022
 *      Author: uwe-w
 */

#include "Nation.hpp"
#include "sdltypes.hpp"

using namespace std;

Nation::Nation(Nationality nationality): m_nation(nationality)
{
	cout<<"Nation-Konstruktor mit Nationalität "<< nationality<<endl;
}

bool Nation::m_canBuildRailroad(){
	return true;
}

bool Nation::m_canBuildBridges(){
	return true;
}


