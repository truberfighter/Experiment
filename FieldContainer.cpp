#include "EventHandler.hpp"
#include <iostream>
#include "sdltypes.hpp"
#include "Field.hpp"
#include <vector>
#include "FieldContainer.hpp"
#include "Grassland.hpp"
#include "Plains.hpp"
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
			if((i+j)%2==0)
				newMeridian.push_back(make_unique<Grassland>(STANDARD_FIELD_SIZE*i, STANDARD_FIELD_SIZE*j));
			else
				newMeridian.push_back(make_unique<Plains>(STANDARD_FIELD_SIZE*i, STANDARD_FIELD_SIZE*j));

		}
	//	cout<<"Zeile 27"<<endl;
m_fieldsOfTheWorld->push_back(newMeridian);
}
	theContainer = this;
}

std::vector<Meridian>* FieldContainer::m_getFieldsOfTheWorld(){
	//if(m_fieldsOfTheWorld) cout <<"kein nullptr"<<endl;
	//cout<<"bla"<<g.size()<<endl;
	return m_fieldsOfTheWorld;
}

void initFieldContainer(){
theContainer = new FieldContainer(WORLD_HEIGHT, WORLD_LENGTH);
//FieldContainer &d = *FieldContainer::theContainer;cout<<"defined reference"<<endl;
if(FieldContainer::getTheContainer()->m_fieldsOfTheWorld) cout<<"kein nullptr Z. 41"<<endl;
}

FieldContainer::FieldContainer(): FieldContainer(WORLD_HEIGHT, WORLD_LENGTH){}

void FieldContainer::initContinentIDs(){
	unsigned int continentCounter(0);
	std::vector<Meridian>& vectorToWorkWith = *m_fieldsOfTheWorld;
	unsigned int xsize = vectorToWorkWith.size();
	unsigned int ysize = vectorToWorkWith[0].size();
	for(unsigned int i(0); i< xsize;i++){
		for(unsigned int j(0); j<ysize; j++){

			shared_ptr<Field>& currentField = vectorToWorkWith[i][j];
			Coordinate neighbouringCoordinates[4] = {Coordinate(i,j-1), Coordinate(i,j+1)
			, Coordinate(i+1,j), Coordinate(i-1,j)
			};
			bool singleIsland = true;
			for(Coordinate& compCoord: neighbouringCoordinates){
				shared_ptr<Field> comparedField = vectorToWorkWith[modulo(compCoord.x, xsize)][ modulo(compCoord.y, ysize)];
				if((comparedField->m_Landscape()!=LANDMASS_SEPARATOR)
				&&(currentField->m_Landscape()!=LANDMASS_SEPARATOR)){
					//if both fields are on a landmass,
					//set their index as the same8int
					ContinentID minimumContinentID = min((unsigned int)comparedField->m_continentID, min((unsigned int) currentField->m_continentID, (unsigned int) continentCounter));
					cout<<"minimumContinentID ="<<minimumContinentID<<endl;
					if(minimumContinentID==continentCounter)continentCounter++;
					currentField->m_continentID=minimumContinentID;
					comparedField->m_continentID=minimumContinentID;
					singleIsland = false;
				}
			}
			//Catch single field islands
			if(singleIsland && currentField->m_Landscape()!=LANDMASS_SEPARATOR)
				currentField->m_continentID = continentCounter++;
		}
	}
	for(unsigned int i(0); i< xsize;i++){
		for(unsigned int j(0); j<ysize; j++){
			//cout<<"i = "<<i<<", j = "<<j<<", m_continentID = "<<vectorToWorkWith[i][j]->m_continentID<<endl;
		}
	}
}

