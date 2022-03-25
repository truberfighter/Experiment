/*
 * Settlers.cpp
 *
 *  Created on: 08.03.2022
 *      Author: uwe-w
 */
#include "Settlers.hpp"
#include "FieldContainer.hpp"
#include "movableThing.hpp"
#include "GameMain.hpp"
#include "Figure.hpp"

using namespace std;

Settlers::Settlers(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality
, std::shared_ptr<City> home, bool isVeteran)
: Figure(whereToStart, nationality, home, isVeteran), m_workStepsCount(0)
{
	m_resetMovementPoints();
if(!m_initImage()) cout<<"Fatal error: MovableThing for Settlers not created"<<endl;
cout<<"Settlerskonstruktor"<<theRenderer<<endl;
}


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
	m_figureState = SETTLERS_AT_WORK;
	m_currentWork = work;
	m_workStepsCount++;
	cout<<"Settlers working on work "<<m_currentWork<<endl;
}

Settlers::~Settlers(){
	this->~Figure();
}

bool Settlers::m_takeOrder(char order){
	switch(order){
	case 'H': {if(m_whereItStands->m_cityContained){m_home = m_whereItStands->m_CityContained(); return true;}return false;}
	case 'I': return m_whereItStands->m_Irrigate(*this);
	case 'M': return m_whereItStands->m_Mining(*this);
	case ' ': {m_finishMove(); return true;}
	default: return false;
}
}

FigureCategory Settlers::m_FigureCategory(){
	return GROUND;
}

std::string Settlers::m_orderOverview(){return "H: HomeCity, B: Add to/Found new City, I: Irrigate, M: Mining, S: Sentry, P: Pillage";}

std::shared_ptr<MovableThing> Settlers::m_createImage(){
	int x = m_whereItStands->m_X(), y = m_whereItStands->m_Y();
	cout<<"Settlers::createImage"<<theRenderer<<endl;
	//ändere das mit dem filename
	cout<<"m_createImage direkt vor return"<<endl;
	m_image = make_shared<MovableThing>(theRenderer, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE, "bilder/Figures/Settlers/RomanRussianSettlers.png", x, y);
	return m_image;
}

int Settlers::m_drawSettlersWork(SDL_Rect& rectToDraw){
	cout<<"m_drawSettlersWork"<<endl;
#ifdef DRAW_LETTER
#undef DRAW_LETTER
#endif
#define DRAW_LETTER(letter) { \
return SDL_RenderCopy(theRenderer, theLetterTextureContainer->m_getLetterTexture(letter)->theTexture(), nullptr, &rectToDraw);\
}

	switch(m_currentWork){
	case IRRIGATE: DRAW_LETTER('I')
	case MAKE_MINING: DRAW_LETTER('M')
	default: return 0;
	}
#undef DRAW_LETTER
}

VISIBILITY_OF Settlers IS_NORMAL_VISIBILITY
STRENGTH_OF Settlers WHEN_ATTACKING(0)
STRENGTH_OF Settlers WHEN_DEFENDING(WEAK)
THE_END_OF_TURN_FOR Settlers GOES_UNEVENTFUL
NORMAL_MOVING(Settlers)
