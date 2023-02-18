/*
 * FieldContainer.hpp
 *
 *  Created on: 04.03.2022
 *      Author: uwe-w
 */

#ifndef FIELDCONTAINER_HPP_
#define FIELDCONTAINER_HPP_

#include "EventHandler.hpp"
#include <iostream>
#include "sdltypes.hpp"
#include "Field.hpp"
#include <vector>

//typedef std::vector<std::shared_ptr<Field>> Meridian;

class FieldContainer{
	std::vector<Meridian>* m_fieldsOfTheWorld;
	std::vector<FieldType> m_fieldTypes;
	//static  FieldContainer* theContainer;
public:
	static void initFieldContainer(int height, int length);
	FieldContainer(int howHigh, int howWide);
	~FieldContainer();
	FieldContainer();
	std::vector<Meridian>* m_getFieldsOfTheWorld();
	static FieldContainer* getTheContainer();
	void initContinentIDs();
	std::vector<FieldType>& m_FieldTypes();
	void m_initFieldTypes();
	void m_initFigureImages();
	void m_initFogInfos(std::vector<Nationality>& nationsToPlay);
};


#endif /* FIELDCONTAINER_HPP_ */
