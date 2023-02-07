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
	FieldContainer(int howHigh, int howWide);
	~FieldContainer();
	FieldContainer();
	std::vector<Meridian>* m_getFieldsOfTheWorld();
	static FieldContainer* getTheContainer();
	friend void initFieldContainer();
	void initContinentIDs();
	std::vector<FieldType>& m_FieldTypes();
	void m_initFieldTypes();

};

void initFieldContainer();

#endif /* FIELDCONTAINER_HPP_ */
