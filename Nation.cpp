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
	cout<<"Nation-Konstruktor mit Nationalit�t "<< nationality<<endl;
}


