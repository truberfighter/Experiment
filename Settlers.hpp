/*
 * Settlers.hpp
 *
 *  Created on: 08.03.2022
 *      Author: uwe-w
 */

#ifndef SETTLERS_HPP_
#define SETTLERS_HPP_

#include "sdltypes.hpp"

class Settlers{
private:
	short int m_workStepsCount;
	SettlersWork m_currentWork = NONE;
public:
	short int m_WorkStepsCount();
	void m_work();
	void m_work(SettlersWork work);
	void m_finishWork(); //set m_workStepsCount to 0
	SettlersWork m_CurrentWork();
};



#endif /* SETTLERS_HPP_ */
