#include "EventHandler.hpp"
#include <iostream>
#include "sdltypes.hpp"
#include "Field.hpp"
#include <vector>
#include "FieldContainer.hpp"
#include "Grassland.hpp"
using namespace std;

FieldContainer* theContainer = nullptr;
shared_ptr<Texture>* fieldTextures = nullptr;

FieldContainer* FieldContainer::getTheContainer(){
	return theContainer;
}

FieldContainer::~FieldContainer(){
	delete m_fieldsOfTheWorld;
	cout<<"Destruktor des FieldContainers"<<endl;
}


FieldContainer::FieldContainer(int howHigh, int howWide)
{
	//cout<<"FieldContainer-Konstruktor"<<endl;
	m_fieldsOfTheWorld=(new vector<Meridian>);
	//cout<<"Um Himmlers Willen: Das soll noch ordentlich initialisiert werden!"<<endl;
	for(int i(0); i<howWide; i++){
		Meridian newMeridian;
		for(int j(0); j<howHigh; j++){
			newMeridian.push_back(make_unique<Grassland>(i*STANDARD_FIELD_SIZE*i, STANDARD_FIELD_SIZE*j));
		}
	//	cout<<"Zeile 27"<<endl;
m_fieldsOfTheWorld->push_back(newMeridian);
}
	theContainer = this;
}

std::vector<Meridian>* FieldContainer::m_getFieldsOfTheWorld(){
	if(m_fieldsOfTheWorld) cout <<"kein nullptr"<<endl;
	auto& g = (*m_fieldsOfTheWorld);
	//cout<<"bla"<<g.size()<<endl;
	return m_fieldsOfTheWorld;
}

void initFieldContainer(){
theContainer = new FieldContainer(WORLD_HEIGHT, WORLD_LENGTH);
//FieldContainer &d = *FieldContainer::theContainer;cout<<"defined reference"<<endl;
if(FieldContainer::getTheContainer()->m_fieldsOfTheWorld) cout<<"kein nullptr Z. 41"<<endl;
}

FieldContainer::FieldContainer(): FieldContainer(WORLD_LENGTH, WORLD_HEIGHT){}
