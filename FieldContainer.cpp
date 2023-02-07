#include "EventHandler.hpp"
#include <iostream>
#include "sdltypes.hpp"
#include "Field.hpp"
#include <vector>
#include "FieldContainer.hpp"
#include "FieldType.hpp"
using namespace std;

FieldContainer* theContainer = nullptr;
std::vector<shared_ptr<Texture>> fieldTextures, resourceTextures; std::vector<std::shared_ptr<Texture>>shieldTextures;

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
				newMeridian.push_back(make_unique<Field>(STANDARD_FIELD_SIZE*i, STANDARD_FIELD_SIZE*j, HILLS));
			else
				newMeridian.push_back(make_unique<Field>(STANDARD_FIELD_SIZE*i, STANDARD_FIELD_SIZE*j, OCEAN));

		}
m_fieldsOfTheWorld->push_back(newMeridian);
}
	theContainer = this;
	m_initFieldTypes();
}

std::vector<Meridian>* FieldContainer::m_getFieldsOfTheWorld(){
	return m_fieldsOfTheWorld;
}

void initFieldContainer(){
theContainer = new FieldContainer(WORLD_HEIGHT, WORLD_LENGTH);
//FieldContainer &d = *FieldContainer::theContainer;
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
			std::vector<Coordinate> neighbouringCoordinates;
			for(int k(-1); k<2;k++)
				for(int l(-1);l<2;l++)
					if(k!=0||l!=0)
						neighbouringCoordinates.push_back(Coordinate(i+k,j+l));
			bool singleIsland = true;
			for(Coordinate& compCoord: neighbouringCoordinates){
				if(compCoord.y<0||compCoord.y>=(int)ysize)
					continue;
				shared_ptr<Field> comparedField = vectorToWorkWith[modulo(compCoord.x, xsize)][compCoord.y];
				if((comparedField->m_Landscape()!=LANDMASS_SEPARATOR)
				&&(currentField->m_Landscape()!=LANDMASS_SEPARATOR)){
					//if both fields are on a landmass,
					//set their index as the same8int
					ContinentID minimumContinentID = min((unsigned int)comparedField->m_continentID, min((unsigned int) currentField->m_continentID, (unsigned int) continentCounter));
					if(minimumContinentID==continentCounter)
						continentCounter++;
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
			0;
		}
	}
}

std::vector<FieldType>& FieldContainer::m_FieldTypes(){
	return m_fieldTypes;
}
