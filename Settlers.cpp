/*
 * Settlers.cpp
 *
 *  Created on: 08.03.2022
 *      Author: uwe-w
 */
#include "Settlers.hpp"

using namespace std;

short int Settlers::m_WorkStepsCount(){
	return m_workStepsCount;
}

SettlersWork Settlers::m_CurrentWork(){
	return m_currentWork;
}

void Settlers::m_finishWork(){
	m_workStepsCount = 0;
#ifdef DEBUG
	cout<<"Some Settlers has finished his work"<<endl;
#endif
}

void Settlers::m_work(){
	m_work(m_currentWork);
}

void Settlers::m_work(SettlersWork work){
	m_currentWork = work;
	m_workStepsCount++;
	cout<<"Settlers working on work "<<m_currentWork<<endl;
}
