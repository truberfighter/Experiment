/*
 * Figure.cpp
 *
 *  Created on: 16.03.2022
 *      Author: uwe-w
 */
bool debug = false;
#include <algorithm>
#include "Figure.hpp"
#include <memory>
#include "FieldContainer.hpp"
#include "Drawing.hpp"
#include <functional>
#include "Field.hpp"
#include <sstream>
#include "Nation.hpp"
#include "Ship.hpp"
#include "City.hpp"
#include "Diplomat.hpp"
#include "Caravan.hpp"
#include "FieldType.hpp"

using namespace std;


Figure::Figure(Field* whereToStart, std::shared_ptr<Nation> nationality,  std::shared_ptr<City> home, bool isVeteran)
: enable_shared_from_this<Figure>(), m_nationality(nationality), m_whereItStands(whereToStart), m_home(home), m_isVeteran(isVeteran)
{
	m_figureID = figureCountGlobal;
	figureCountGlobal++;
	figureToDraw = this;
	cout<<"figurekonstruktor, figureID "<<m_figureID<<", pointer: "<<this<<endl;
}

Figure::~Figure(){
	std::cout<<"Figure-Destruktor"<<this<<std::endl;
}

Nationality Figure::m_Nationality(){
	if(!this->m_nationality)
		return NO_NATIONALITY;
	return m_nationality->m_Nation();
}

void Figure::m_resetMovementPoints(){
	m_movementPoints.m_movementPoints = m_defaultMovementPoints();
}

bool Figure::m_Pillage(){
	if(m_movementPoints.m_movementPoints!=0){
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

Field& Figure::m_WhereItStands(){
	return *m_whereItStands;
}

bool Figure::m_fortify(){
	if(m_movementPoints.m_movementPoints!=0 && m_defensiveStrength()!=0){
		m_figureState = FORTIFYING;
		m_finishMove();
		return true;
	}
	else return false;
}

bool Figure::m_sentry(){
	if(m_movementPoints.m_movementPoints!=0){
		m_finishMove();
		m_figureState = SENTRYING;
		return true;
	}
	else return false;
}
//Start a turn. Sentried units are kinda ignored for the first part.
bool Figure::m_startMove(bool activateSentried){
	m_makeFiguresVisibleAround(m_whereItStands);
	cout<<"Figure::m_startMove: visibilitySize = "<<m_visibilityInfo.size()<<", figureState = "<<m_figureState<<", visibilityInfoSize = "<<m_visibilityInfo.size()<<"nationality = "<<m_nationality->m_Nation()<<"m_figureType = "<<m_FigureType()<<", this = "<<this<<std::endl;
	switch(m_figureState){
	case DONE_WITH_TURN:{
	activate:	m_resetMovementPoints();
		m_figureState = MOVING;
		cout<<"FigureState: ";cout<<m_figureState;cout<< ", MovementPoints: "<<m_movementPoints<<std::endl;
		return true;
	}
	case FORTIFYING: {m_figureState = FORTIFIED; return true;}
	case FORTIFIED: {m_figureState = COMPLETELY_FORTIFIED;return true;}
	case SENTRYING: {m_figureState = SENTRIED;return true;}
	case SENTRIED: {
		if(activateSentried)
			goto activate;
		Direction directions[] = {DOWN_LEFT, DOWN, DOWN_RIGHT, LEFT,RIGHT, UP_LEFT, UP, UP_RIGHT};
		for(int i(0); i<8;i++){
			if(!(m_whereItStands->m_getNeighbouringField(directions[i])->m_FiguresOnField().empty()) &&
			m_whereItStands->m_getNeighbouringField(directions[i])->m_FiguresOnField().front()->m_Nationality() != m_nationality->m_Nation()){
				goto activate;
			}
		}
		return true;
	}
	case PILLAGE_IN_PROGRESS:{
		goto activate;
	}
	case COMPLETELY_FORTIFIED: return true;
	case SETTLERS_AT_WORK: {
		if(m_FigureType()!=SETTLERS)
			throw("Big fail");
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
	Field* aim =  m_whereItStands->m_getNeighbouringField(whereToGo);
	if(aim->m_CityContained()&&aim->m_FiguresOnField().empty()&&m_FigureCategory()!=GROUND){
		if(aim->m_CityContained()->m_OwningNation()->m_Nation()!=m_nationality->m_Nation()){
			return MOVE_PROHIBITED;
		}
	}
	switch(m_FigureCategory()){
	case SEA:
		if(aim->m_Landscape()==OCEAN){
			//std::cout<<"normal moving in the ocean"<<std::endl;
			return ONE_MOVEMENT_POINT;
		}
		if(!aim->m_FiguresOnField().empty()){
			std::cout<<"field aimed at is not empty"<<std::endl;
			return (aim->m_FiguresOnField().front()->m_Nationality()==m_Nationality()) ? (aim->m_CityContained() ? m_movementPoints : MOVE_PROHIBITED) : ((reinterpret_cast<Ship*>(this))->m_MayBombardGroundTroops() ? ONE_MOVEMENT_POINT : MOVE_PROHIBITED);
		}
		if(aim->m_CityContained()!=nullptr){
			return MovementPoints(m_movementPoints);
		}
		std::cout<<"standardMovement"s<<std::endl;
		return m_movementPoints.m_movementPoints >= ONE_MOVEMENT_POINT
	&& m_whereItStands->m_getNeighbouringField(whereToGo)->m_Landscape()==OCEAN ?
						ONE_MOVEMENT_POINT : MOVE_PROHIBITED;
	case FLIGHT:
		if(aim->m_CityContained()!=nullptr){
			return MovementPoints(m_movementPoints);
		}
		if(m_whereItStands->m_getNeighbouringField(whereToGo)->m_Landscape()!=OCEAN){
			return ONE_MOVEMENT_POINT;
		}
		else{
			bool allowed = m_whereItStands->m_getNeighbouringField(whereToGo)->m_getCargoCapability(*this) > 0 ? true : false;
			if(allowed){
				m_figureState = SENTRIED;
			}
			return  allowed ? m_movementPoints : ONE_MOVEMENT_POINT;
		}
case GROUND:
	{
		if(m_whereItStands->m_getNeighbouringField(whereToGo)->m_Landscape()!=OCEAN)
		return m_calculateMoveCostGround(whereToGo);
		else{
			bool allowed = m_whereItStands->m_getNeighbouringField(whereToGo)->m_getCargoCapability(*this) > 0;
			if(allowed){
				m_figureState = SENTRIED;
			}
			return  allowed ? ONE_MOVEMENT_POINT : MOVE_PROHIBITED;
		}
	}
	}
	return MOVE_PROHIBITED;
}

bool Figure::m_tryMoveToField(Direction whereToGo){
	try{
	Direction directions[] = {DOWN_LEFT, DOWN, DOWN_RIGHT, LEFT,RIGHT, UP_LEFT, UP, UP_RIGHT};
	std::shared_ptr<Figure> thisFigure = shared_from_this();
	Field& fieldWhereToGo = *(m_whereItStands->m_getNeighbouringField(whereToGo));
	if(m_FigureType()==DIPLOMAT){
		if((reinterpret_cast<Diplomat*>(this)->m_doDiplomatThings(fieldWhereToGo))==false){
			return false;
		}
	}
	if(m_FigureType()==CARAVAN){
		if((reinterpret_cast<Caravan*>(this)->m_doCaravanThings(fieldWhereToGo))==false){
			return false;
		}
	}
	if(m_FigureType()!=FIGHTER){
	for(const std::shared_ptr<Figure>& currentFigure: fieldWhereToGo.m_FiguresOnField()){
		if(currentFigure->m_FigureCategory()==FLIGHT && currentFigure->m_Nationality()!=m_Nationality()&&fieldWhereToGo.m_CityContained()!=nullptr){
			return false;
		}
	}
	}
	MovementPoints movementCost = m_calculateMoveCost(whereToGo);
	std::cout<<"figure tries move to field: nation = "<<m_nationality->m_Nation()<<", this = "<<this<<"2, figureState = "<<m_figureState<<", figureType = "<<m_FigureType()<<endl;
	if(movementCost == MOVE_PROHIBITED){
		return false;
	}
	if(m_figureState == SENTRIED){
		goto beginMove;
	}
	//cout<<*m_whereItStands<<(*(m_whereItStands->m_getNeighbouringField(whereToGo)))<<std::endl;
	//cout<<"moveCost: "<<movementCost<<", MovementPoints: "<<m_movementPoints<<endl;
	if(movementCost.m_movementPoints > m_movementPoints.m_movementPoints){
		return false;
	}
	if(fieldWhereToGo.m_FiguresOnField().empty()){
		bool surroundedByMilitaryUnit = false;
		for(Direction currentDirection: directions){
			if(m_whereItStands->m_getNeighbouringField(currentDirection)->m_militaryProblem(thisFigure)){
				//some unit next to you
				surroundedByMilitaryUnit = true;
				break;
			}
		}
		if(surroundedByMilitaryUnit){
			for(Direction currentDirection: directions){
				if(fieldWhereToGo.m_getNeighbouringField(currentDirection)->m_militaryProblem(thisFigure)){
					//some unit next to you
					return false;
				}
			}
		}
	}
	beginMove: m_whereItStands->m_getNeighbouringField(whereToGo)->m_takeFigure(thisFigure);
	m_move(whereToGo);
	if((m_movementPoints -= movementCost).m_movementPoints <= 0){
		m_finishMove();
		if(!m_nationality->m_removeFromQueue(shared_from_this())){
			cout<<"removefromqueueProblems, or it just has been done before:  listsize = "<<m_nationality->m_queueSize()<<", this = "<<this;
		}
	}
	return true;
	}
	catch(PoleHitException& pHEx){
		cout<<pHEx.what();
		flush(cout);
		return false;
	}
	catch(Fight& fight){
		cout<<fight.what();
		flush(cout);
		this->m_finishWinningAttack(m_whereItStands->m_getNeighbouringField(whereToGo));
		if(fight.m_result == ATTACKER_LOSES || fight.m_result == KAMIKAZE){
			// You died!
			m_figuresAttacked.clear();
			throw;
		}
		m_movementPoints.m_movementPoints = max(m_movementPoints.m_movementPoints - ONE_MOVEMENT_POINT, 0);
		if(m_whereItStands->m_getNeighbouringField(whereToGo)->m_CityContained()){
			m_whereItStands->m_getNeighbouringField(whereToGo)->m_CityContained()->m_shrink();
		}
		if(m_movementPoints == 0){
			m_finishMove();
			if(!m_nationality->m_removeFromQueue(shared_from_this())){
				cout<<"catching fight in tryMoveToField:  listsize = "<<m_nationality->m_queueSize()<<", this = "<<this;
			}
		}
		m_figuresAttacked.clear();
		throw;
	}
	catch(CaravanDead& cd){
		m_nationality->m_destroyFigure(shared_from_this());
		return true;
	}
	catch(DiplomatDead& dd){
		m_nationality->m_destroyFigure(shared_from_this());
		return true;
	}
}

void Figure::m_move(Direction whereToGo){
	figureToDraw = this;
	m_figureState = MOVING;
	m_setInstructionsForDrawingElement();
	m_image->m_setMoveToDirection(whereToGo);
	m_image->m_move(whereToGo);
	switch(m_FigureCategory()){
	case SEA:
	{
		for(std::shared_ptr<Figure> figureToTakeWithMe: reinterpret_cast<Ship*>(this)->m_getCargo()){
			figureToTakeWithMe->m_tryMoveToField(whereToGo);
		}
		m_whereItStands = (m_whereItStands->m_getNeighbouringField(whereToGo));
		break;
	}
	case FLIGHT:
	{
		m_whereItStands = (m_whereItStands->m_getNeighbouringField(whereToGo));
		if(m_whereItStands->m_getCargoCapability(*this) > 0 || m_whereItStands->m_CityContained()!= nullptr){
			if(m_whereItStands->m_getCargoCapability(*this)>0){
				std::cout<<"m_whereItStands->cargo: "<<m_whereItStands->m_getCargoCapability(*this)<<std::endl;
				m_figureState = SENTRIED;
		}
		}
		break;
	}
	case GROUND:
	{
		m_whereItStands = (m_whereItStands->m_getNeighbouringField(whereToGo));
		if(m_whereItStands->m_Landscape()==OCEAN){
			m_figureState = SENTRIED;}
		break;
	}
	}
}

int Figure::m_desertationCost(){
	return City::shieldsNeeded((ImprovementType)m_FigureType())*(int)(m_nationality->m_Treasury()+750)/(int)(m_whereItStands->m_distanceTo(m_nationality->m_CapitalCity()))/10/(m_FigureType()==SETTLERS ? 1 : 2);
}

MovementPoints Figure::m_calculateMoveCostGround(Direction whereToGo){
	Field& fieldToVisit = *(m_whereItStands->m_getNeighbouringField(whereToGo));
	if(m_whereItStands->m_Landscape()==OCEAN && !fieldToVisit.m_FiguresOnField().empty() && fieldToVisit.m_FiguresOnField().front()->m_Nationality()!=m_Nationality()){
		SDL_Surface* textSurface = TTF_RenderText_Solid(theFont, "You can't launch a land based attack from a sea vessel", whiteColor);
		SDL_Rect rectToFill{INFO_TEXT_X & 0, INFO_TEXT_Y,textSurface->w,textSurface->h};
		SDL_SetRenderDrawColor(theRenderer, 255,0,0,0);
		SDL_RenderFillRect(theRenderer, &rectToFill);
		SDL_Texture* texture = SDL_CreateTextureFromSurface(theRenderer, textSurface);
		SDL_RenderCopy(theRenderer, texture, NULL, &rectToFill);
		SDL_RenderPresent(theRenderer);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(texture);
		return MOVE_PROHIBITED;
	}
	switch(m_whereItStands->m_RoadStatus()){
		case NOTHING:{
			fieldToVisit.m_getFieldType().m_texture;
				std::cout<<"tandardmovecostground kommt"<<std::endl;
			return m_calculateStandardMoveCostGround(fieldToVisit);
		}
		case ROAD:{
			if(fieldToVisit.m_RoadStatus()!=NOTHING){
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
&& fieldToVisit.m_movementPoints().m_movementPoints < m_movementPoints.m_movementPoints){
		return MOVE_PROHIBITED;
	}
	else {
			return m_movementPoints.m_movementPoints<fieldToVisit.m_movementPoints().m_movementPoints ? m_movementPoints : fieldToVisit.m_movementPoints();
		//es soll hier dann  um Kampfhandlungen gehen
	}
}

std::shared_ptr<MovableThing> Figure::m_Image(){
	return m_image;
}

MovementPoints Figure::m_MovementPoints(){
	return m_movementPoints;
}

ostream& operator<<(ostream& os, Landscape ls){
	switch(ls){
	case PLAINS:
		os<<"PLAINS"; break;
	case GRASSLAND:
		os<<"GRASSLAND"; break;
	case OCEAN:
		os<<"OCEAN"; break;
	case DESERT: os<<"DESERT";break;
	case TUNDRA: os<<"TUNDRA";break;
	case ARCTIC: os<<"ARCTIC";break;
	case RIVER: os<<"RIVER";break;
	case SWAMP: os<<"SWAMP";break;
	case FOREST: os<<"FOREST";break;
	case MOUNTAIN: os<<"MOUNTAIN";break;
	case HILLS: os<<"HILLS";break;
	case JUNGLE: os<<"JUNGLE"; break;
	default:
		os<<(int)ls;
	}
	return os;
}

ostream& operator<<(ostream& os, Field& field){
	os<<"Field on x = ";
	os<<field.m_x;
	os<<", y = ";
	os<<field.m_y;
	os<<":\n";
	os<<"Landscape = ";
	os<<field.m_Landscape();
	os<<", Layer: ";
	os<<field.m_layer;
	os<<", this = ";
	os<<&field;
	os<<"\n";
	if(field.m_cityContained){
		os<<"cityContained! ";
	}
	if(field.m_hasSpecialResource)os<<"Special resource!\n";
	os<<(field.m_isMined ? std::string("It's mined!") :  field.m_isIrrigated ? string("It's irrigated!") : string(""))<<"\n";
	return os;
}

int Figure::m_drawFigureState(int x, int y, SDL_Renderer* renderer){
#ifdef DRAW_LETTER
#undef DRAW_LETTER
#endif
#define DRAW_LETTER(letter) { \
return SDL_RenderCopy(theRenderer, theLetterTextureContainer->m_getLetterTexture(letter)->theTexture(), nullptr, &rectToDraw);\
}
SDL_Rect rectToDraw{x+STANDARD_FIELD_SIZE/4,y+STANDARD_FIELD_SIZE/4,FIGURESTATE_TEXTURE_WIDTH, FIGURESTATE_TEXTURE_HEIGHT};
//cout<<"figureState: "<<m_figureState<<endl;
	switch(m_figureState){
	case MOVING: {	return 0;}
	case DONE_WITH_TURN: {if(true) DRAW_LETTER('D') break;}
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
	std::cout<<"image: "<<m_image.get()<<std::endl;
	shared_ptr<DrawingElement> temp = make_shared<FigureElement>(theRenderer, m_image.get());
	drawing.m_add(temp);
	for(MovableDrawingElement* currentElement: m_image->m_HowDrawn()){
		currentElement->m_setFigure(this);
	}
}

void Figure::m_showTheFigureIsCurrentFigure(){

}

void Figure::m_wait(){
	if(!m_nationality){
		throw("Waiting without a nation!");
	}
	m_nationality->m_makeFigureWait();
}

void Figure::m_drawFigure(BlinkingState blinkingState){
if(debug)
	return;
try{
	if(blinkingState == VISIBLE()){
	for(MovableDrawingElement* currentDrawingElement: m_image->m_HowDrawn()){
		currentDrawingElement->m_drawAsRemembered(theRenderer);
	}
	}
	if(blinkingState == INVISIBLE()){
		m_whereItStands->m_drawField();
	}
	}
	catch(DrawingFail& theException){
		cout<<theException.what()<<endl;
		cout<<this<<endl;
	}
}

std::string Figure::m_figureOverview(){
	std::stringstream theStringstream;
	if(m_nationality){
		theStringstream<<m_nationality->m_Nation()<<std::endl;
	}
	theStringstream<<m_FigureType();
	if(m_isVeteran){
		theStringstream<<"(V)";
	}
		theStringstream<<endl;
	if(!m_home){
		theStringstream<<"NONE";
	}
	else{
		theStringstream<<m_home->m_Name();
	}
	theStringstream.flush();
	return theStringstream.str();
}

void Figure::m_drawFigureSomewhere(int row, int column){
	m_image->m_drawNew(row, column);
	m_drawFigureState(row,column,theRenderer);
}

bool Figure::m_homeCity(){
	if(m_whereItStands->m_CityContained()){
		if(m_home){
			m_home->m_releaseFigure(shared_from_this());
		}
		m_home = m_whereItStands->m_CityContained();
		m_home->m_takeFigure(shared_from_this());
		return true;
	}
	return false;
}

bool Figure::m_isVisible(Nationality nationality){
	if(nationality == m_Nationality())
		return true;
	return isInVector<Nationality>(m_visibilityInfo, nationality, [](const Nationality& n1, const Nationality& n2){return n1==n2;});
}

void Figure::m_makeFiguresVisibleAround(Field* fieldBase){
	if(fieldBase == nullptr){
		fieldBase = m_whereItStands;
	}
	std::vector<Coordinate> coordinates = coordinatesAroundField(m_visibilityRange());
	for(Coordinate& currentCoordinate: coordinates){
		std::cout<<*fieldBase->m_getNeighbouringField(currentCoordinate)<<std::endl;
		for(const std::shared_ptr<Figure>& currentFigure: fieldBase->m_getNeighbouringField(currentCoordinate)->m_FiguresOnField()){
			if(currentFigure->m_Nationality()!=m_Nationality()){
				if(currentFigure->m_canBeSeenBy(this)){
					m_makeVisible(currentFigure->m_Nationality());
				}
			}
		}
	}
	std::vector<Coordinate> coordinates2 = coordinatesAroundField(Figure::maxVisibilityRange());
	for(Coordinate& currentCoordinate: coordinates2){
		std::cout<<*fieldBase->m_getNeighbouringField(currentCoordinate)<<std::endl;
		for(const std::shared_ptr<Figure>& currentFigure: fieldBase->m_getNeighbouringField(currentCoordinate)->m_FiguresOnField()){
			if(currentFigure->m_Nationality()!=m_Nationality()){
				if(m_canBeSeenBy(currentFigure.get())){
					currentFigure->m_makeVisible(m_Nationality());
				}
			}
		}
	}
}

void Figure::m_makeVisible(Nationality nationality){
	if(!m_isVisible(nationality)){
		std::cout<<"makeFigureVisible: figureType = "<<m_FigureType()<<", nation1 = "<<m_Nationality()<<", nation 2 = "<<nationality<<std::endl;
		m_visibilityInfo.push_back(nationality);
	}
}

FigureElement::FigureElement(SDL_Renderer* renderer, MovableThing* movableEntity):MovableDrawingElement(renderer, movableEntity){}

int FigureElement::m_draw(int rowShift, int columnShift, SDL_Renderer* renderer){
	if(!m_figure)
		return 0;
	if(m_figure->m_isVisible(theGame->m_NationAtCurrentTurn()->m_Nation())){
		return MovableDrawingElement::m_draw(rowShift, columnShift, renderer);
	}
	else{
		return 0;
	}
}

void Figure::m_hideFrom(Nationality nationality){
	std::cout<<"hide from "<<nationality<<", size: "<<m_visibilityInfo.size();
	for(Nationality& n: m_visibilityInfo){
		if(n==nationality){
			n = m_visibilityInfo.back();
			m_visibilityInfo.pop_back();
			break;
		}
	}
	std::cout<<", later: "<<m_visibilityInfo.size()<<std::endl;
}

void Figure::m_changeNationTo(std::shared_ptr<Nation> newNation, std::shared_ptr<City> newCity){
	m_nationality->m_removeFigure(shared_from_this());
	newNation->m_addFigure(shared_from_this());
	if(m_home){
	m_home->m_releaseFigure(shared_from_this());
	}
	newCity->m_takeFigure(shared_from_this());
	m_home = newCity;
	m_nationality = newNation;
	if(m_FigureType()==SETTLERS){
		reinterpret_cast<Settlers*>(this)->m_currentWork = NONE;
	}
	std::stringstream s;
	s<<"bilder/Figures/"<<m_FigureType()<<"/"<<m_FigureType()<<m_nationality->m_colorString();
	s.flush();
	std::string imageString = s.str();
	SDL_Texture* newTexture = IMG_LoadTexture(theRenderer,imageString.c_str());
	m_image->m_setTexture(
			std::make_shared<Texture>(newTexture,
	City::figureWidth(m_FigureType()), City::figureHeight(m_FigureType())));

}

bool Figure::m_canBeSeenBy(Figure* figureLooking){
	return m_whereItStands->m_maxNormDistanceTo(figureLooking->m_whereItStands) <= figureLooking->m_visibilityRange();
}

void Figure::m_finishWinningAttack(Field* battlefield){

}

