/*
 * Figure.cpp
 *
 *  Created on: 16.03.2022
 *      Author: uwe-w
 */

#include "figure.hpp"
#include <memory>
#include "FieldContainer.hpp"
#include "Drawing.hpp"
#include <functional>
#include "Field.hpp"


using namespace std;


Figure::Figure(std::shared_ptr<Field> whereToStart, std::shared_ptr<Nation> nationality,  std::shared_ptr<City> home, bool isVeteran)
: enable_shared_from_this<Figure>(), m_nationality(nationality), m_whereItStands(whereToStart), m_home(home), m_isVeteran(isVeteran)
{
	settlersCount++;
	figureToDraw = this;
	cout<<"figurekonstruktor"<<this<<endl;
}

Figure::~Figure(){
	std::cout<<"Figure-Destruktor"<<this<<std::endl;
}

void Figure::m_resetMovementPoints(){
	m_movementPoints = m_defaultMovementPoints();
}

bool Figure::m_Pillage(){
	if(m_movementPoints!=0){
		m_figureState = PILLAGE_IN_PROGRESS;
		if(m_whereItStands->m_Pillage()){
			m_finishMove();
			return true;
		}
	}
	return false;
}

FigureState Figure::m_FigureState(){
	return m_figureState;
}

bool Figure::m_fortify(){
	if(m_movementPoints!=0 && m_defensiveStrength()!=0){
		m_figureState = FORTIFYING;
		return true;
	}
	else return false;
}

bool Figure::m_sentry(){
	if(m_movementPoints!=0){
		m_figureState = SENTRYING;
		return true;
	}
	else return false;
}


//Start a turn. Sentried units are kinda ignored for the first part.
bool Figure::m_startMove(){
cout<<"Figure::m_startMove: figureState = "<<m_figureState<<", nationality = "<<m_nationality->m_Nation()<<", this = "<<this<<std::endl;
	switch(m_figureState){
	case DONE_WITH_TURN:{
		m_resetMovementPoints();
		m_figureState = MOVING;
		cout<<"FigureState: "<<m_figureState<< ", MovementPoints: "<<m_movementPoints<<std::endl;
		return true;
	}
	case FORTIFYING: {m_figureState = FORTIFIED; return true;}
	case FORTIFIED: {m_figureState = COMPLETELY_FORTIFIED;return true;}
	case SENTRYING: {m_figureState = SENTRIED;return true;}
	case SENTRIED: return true;
	case COMPLETELY_FORTIFIED: return true;
	case SETTLERS_AT_WORK: {
		reinterpret_cast<Settlers*>(this)->m_takeOrder(getSettlersOrder(reinterpret_cast<Settlers*>(this)->m_CurrentWork()));
		return true;}

	case MOVING: throw(TurnEndsTooEarly());
	}
	return false;
}

bool Figure::m_initImage(){
	return m_createImage() != nullptr;
}

MovementPoints Figure::m_calculateMoveCost(Direction whereToGo){
	switch(m_FigureCategory()){
	case SEA:
		return m_movementPoints >= ONE_MOVEMENT_POINT
				&& m_whereItStands->m_getNeighbouringField(whereToGo)->m_Landscape()==LANDMASS_SEPARATOR ?
						ONE_MOVEMENT_POINT : MOVE_PROHIBITED;
	case FLIGHT:
		return m_movementPoints >= ONE_MOVEMENT_POINT ? ONE_MOVEMENT_POINT : MOVE_PROHIBITED;
	case GROUND:
	{
		if(m_whereItStands->m_getNeighbouringField(whereToGo)->m_Landscape()!=LANDMASS_SEPARATOR)
		return m_calculateMoveCostGround(whereToGo);
		else return MOVE_PROHIBITED;
	}
	}
	return MOVE_PROHIBITED;
}

bool Figure::m_tryMoveToField(Direction whereToGo){
	try{
		std::cout<<"figure tries move to field: nation = "<<m_nationality->m_Nation()<<", this = "<<this<<endl;
	cout<<*m_whereItStands<<(*(m_whereItStands->m_getNeighbouringField(whereToGo)))<<std::endl;
	MovementPoints movementCost = m_calculateMoveCost(whereToGo);
	cout<<"moveCost: "<<movementCost<<", MovementPoints: "<<m_movementPoints<<endl;
	if(movementCost == MOVE_PROHIBITED){
		return false;
	}
	if(movementCost > m_movementPoints){
		return false;
	}
	m_move(whereToGo);
	if((m_movementPoints -= movementCost) <= 0){
		m_finishMove();
		if(!m_nationality->m_removeFromQueue(shared_from_this())){
			cout<<"RemoveFromQueue-Fail"<<this;
		}
	}
	return true;
	}
	catch(PoleHitException &pHEx){
		cout<<pHEx.what();
		flush(cout);
		return false;
	}
}

void Figure::m_move(Direction whereToGo){
	figureToDraw = this;
	m_figureState = MOVING;
	m_setInstructionsForDrawingElement();
	m_image->m_setMoveToDirection(whereToGo);
	m_image->m_move(whereToGo);
	m_whereItStands = (m_whereItStands->m_getNeighbouringField(whereToGo));
}

MovementPoints Figure::m_calculateMoveCostGround(Direction whereToGo){
	Field& fieldToVisit = *(m_whereItStands->m_getNeighbouringField(whereToGo));
	switch(m_whereItStands->m_RoadStatus()){
		case NOTHING:{
			return m_calculateStandardMoveCostGround(fieldToVisit);
		}
		case ROAD:{
			if(fieldToVisit.m_RoadStatus()!=NOTHING){
				//Kampfhandlungen...
				return ONE_MOVEMENT_POINT/3;
			}
			else{
				 return m_calculateStandardMoveCostGround(fieldToVisit);
			}
		}
		case RAILROAD:{
			if(fieldToVisit.m_RoadStatus()!=NOTHING){
				return m_calculateStandardMoveCostGround(fieldToVisit);
			}
			else return 0;
		}
	}
	return MOVE_PROHIBITED;

}

MovementPoints Figure::m_calculateStandardMoveCostGround(Field& fieldToVisit){
	if(m_movementPoints< ONE_MOVEMENT_POINT
&& fieldToVisit.m_movementPoints() < m_movementPoints){
		return MOVE_PROHIBITED;
	}
	else {
		//es soll hier dann  um Kampfhandlungen gehen
		return fieldToVisit.m_movementPoints();
	}
}

std::shared_ptr<MovableThing> Figure::m_Image(){
	return m_image;
}

ostream& operator<<(ostream& os, Landscape ls){
	switch(ls){
	case PLAINS:
		os<<"PLAINS"; break;
	case GRASSLAND:
		os<<"GRASSLAND"; break;
	case OCEAN:
		os<<"OCEAN"; break;
	default:
		os<<(int)ls;
	}
	return os;
}

ostream& operator<<(ostream& os, Field& field){
	os<<"Field on x = "<<field.m_x<<", y = "<<field.m_y<<":\n";
	os<<"Landscape = "<<field.m_Landscape()<<", Layer: "<<field.m_layer<<", this = "<<&field<<"\n";
	if(field.m_hasSpecialResource)os<<"Special resource!\n";
	os<<(field.m_isMined ? std::string("It's mined!") :  field.m_isIrrigated ? string("It's irrigated!") : string(""))<<"\n";
	return os;
}

int Figure::m_drawFigureState(int x, int y, SDL_Renderer* renderer){
#ifdef DRAW_LETTER
#undef DRAW_LETTER
#endif
#define DRAW_LETTER(letter) { \
	/*cout<<letter<<endl;*/\
return SDL_RenderCopy(theRenderer, theLetterTextureContainer->m_getLetterTexture(letter)->theTexture(), nullptr, &rectToDraw);\
}
SDL_Rect rectToDraw{x+STANDARD_FIELD_SIZE/4,y+STANDARD_FIELD_SIZE/4,FIGURESTATE_TEXTURE_WIDTH, FIGURESTATE_TEXTURE_HEIGHT};
//cout<<"figureState: "<<m_figureState<<endl;
	switch(m_figureState){
	case MOVING: {cout<<"Moving"<<endl;
	return SDL_RenderCopy(theRenderer, theLetterTextureContainer->m_getLetterTexture('M')->theTexture(), nullptr, &rectToDraw);
	}
	case DONE_WITH_TURN: {if(true) DRAW_LETTER('D')}
	case PILLAGE_IN_PROGRESS:DRAW_LETTER('P')
	case SENTRYING: DRAW_LETTER('S')
	case FORTIFYING: DRAW_LETTER('F')
	case SENTRIED: {
		SDL_Color colorToUse{100,0,255};
		return Graphics::drawSquareStarLines(theRenderer,x,y,brownColor);
	}
	case FORTIFIED:{
		fortified:
		SDL_Color colorToUse{255,255,255};//zum Test verstellt
		return Graphics::drawSquareStarLines(theRenderer,x,y,colorToUse);
	}
	case COMPLETELY_FORTIFIED: goto fortified;
	case SETTLERS_AT_WORK:{
		Settlers* settlersThis = reinterpret_cast<Settlers*>(this);
		return settlersThis->m_drawSettlersWork(rectToDraw);
	}
	default: return 0;
	}
	//Now we are gonna handle some Settlers-only orders.
#undef DRAW_LETTER
}

void Figure::m_setInstructionsForDrawingElement(){
	m_image->m_setDrawingInstructions([](int x, int y, SDL_Renderer* renderer)-> int
		{
			return figureToDraw->m_drawFigureState(x,y,renderer);
		}
	);
}

void Figure::m_integrateInto(Drawing& drawing){
	drawing.m_add(m_image.get());
	for(MovableDrawingElement* currentElement: m_image->m_HowDrawn()){
		currentElement->m_setFigure(this);
	}
}

void Figure::m_showTheFigureIsCurrentFigure(){

}
