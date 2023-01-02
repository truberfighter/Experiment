/*
 * Field.cpp
 *
 *  Created on: 04.03.2022
 *      Author: uwe-w
 */

#include "Field.hpp"
#include "City.hpp"
#include "Drawing.hpp"
#include "Settlers.hpp"
#include "FieldContainer.hpp"
#include "sdltypes.hpp"
#include <list>
#include "Ship.hpp"
#include "FieldType.hpp"

Field::Field(int x, int y, Landscape ls, bool hasSpecialResource, bool hasShield)
: enable_shared_from_this(),m_x(x), m_y(y), m_landscape(ls),m_layer(STANDARD_FIELD_LAYER), m_hasSpecialResource(hasSpecialResource), m_hasShield(hasShield)
{
	m_drawingElement = std::make_shared<FieldElement>(theRenderer, fieldTextures[ls], x, y, STANDARD_FIELD_LAYER);
	m_drawingElement->m_setField(this);
}

std::shared_ptr<City> Field::m_CityContained(){
	return m_cityContained;
}

Field::~Field(){}

int Field::m_X() const{return m_x;}

int Field::m_Y() const {return m_y;}

std::shared_ptr<FieldElement> Field::m_DrawingElement(){return m_drawingElement;}

RoadStatus Field::m_RoadStatus(){
	return m_roadStatus;
}

bool Field::m_IsMined() const{return m_isMined;}

bool Field::m_IsIrrigated() const{return m_isIrrigated;}

bool Field::m_maybeFinishWork(Settlers& settlers, SettlersWork work){
	if(m_howLongToTake(work)==SETTLERSWORK_UNAVAILABLE){
		std::cout<<"SettlersWork unavailable"<<std::endl;
throw(SettlersworkUnavailable());
	}
	if(m_howLongToTake(work)<=settlers.m_WorkStepsCount()){
		std::cout<<"m_howLongToTake("<<work<<") = "<<m_howLongToTake(work)<<", settlers.m_workStepsCount() = "<<settlers.m_WorkStepsCount()<<std::endl;
		settlers.m_finishWork();
		return true;
	}
	return false;
}

void Field::m_railRoadProductionEffect(int& count){
	if(m_roadStatus == RAILROAD)
		count = count + count/2;
}

int Field::m_food(Nation& nation){
	int count = m_food();
	ACKNOWLEDGE_RAILROAD
	ACKNOWLEDGE_DESPOTISM
	return count;
}

int Field::m_shields(Nation& nation){
	int count = m_shields();
	ACKNOWLEDGE_RAILROAD
	ACKNOWLEDGE_DESPOTISM
	return count;
}

int Field::m_trade(Nation& nation){
	int count = m_trade();
	ACKNOWLEDGE_RAILROAD
	ACKNOWLEDGE_DESPOTISM
	ACKNOWLEDGE_DEMOCRACY
	return count;
}

bool Field::m_HasSpecialResource(){return m_hasSpecialResource;}

bool Field::m_MiningTemplate(SettlersWork whatWorkWillCome, Settlers& settlers){
	if(m_howLongToTake(whatWorkWillCome)==SETTLERSWORK_UNAVAILABLE){
			throw(SettlersworkUnavailable());
		}
	switch(whatWorkWillCome){
	case MAKE_MINING:{
		if(m_isMined){
				return false;//do not mine again
			}
		else{ //if(!m_isMined)
			settlers.m_work(MAKE_MINING);
			//Mining complete
			if(m_maybeFinishWork(settlers, MAKE_MINING)){
				//set to mined
				m_isMined = true;
			}
			return true;
		}
	}
	default:{
		return false;
	}
	}
}



int Field::m_roadTradeResult(){
	if(m_roadStatus == ROAD || m_roadStatus == RAILROAD)
			return 40;
		else //m_roadStatus == NOTHING
			return 0;
}

bool Field::m_Pillage(){
	if(m_isMined){
		m_isMined = false;
		return true;
	}
	if(m_isIrrigated){
		m_isIrrigated = false;
		return true;
	}
	else{
		return false;
	}
}

std::shared_ptr<Field> Field::m_getNeighbouringField(Direction whereToLook){
	int x = m_x/STANDARD_FIELD_SIZE;
	int y = m_y/STANDARD_FIELD_SIZE;
	switch(whereToLook){
		case UP:
			{y--; break;}
		case STANDING_STILL:
			break;
		case DOWN:
			{y++; break;}
		case RIGHT: {x++; break;}
		case LEFT: {x--; break;}
		case UP_LEFT: {y--; x--; break;}
		case UP_RIGHT: {y--; x++; break;}
		case DOWN_LEFT: {y++; x--; break;}
		case DOWN_RIGHT: {y++; x++; break;}
		}
	if(y==WORLD_HEIGHT)
			throw(PoleHitException(SOUTHERN_POLE_HIT));
		if(y==-1)
			throw(PoleHitException(NORTH_POLE_HIT));
	x = modulo(x, WORLD_LENGTH);
	//Tribut an Civi, man kann nicht vom Nord- oder Südpol weg
	y = y % WORLD_HEIGHT;
	//Pol getroffen

	std::vector<Meridian>& fieldsOfTheWorld =  *theContainer->m_getFieldsOfTheWorld();
	return fieldsOfTheWorld[x][y];
}

bool Field::m_createRoadImage(SDL_Color& color){
	m_drawingElement->m_setAdditionalInstructions
	([](int x, int y, SDL_Renderer* renderer)->int{return Graphics::drawSquareStarLines(renderer, x, y, brownColor);});
	return true;
}



bool Field::m_road(Settlers& settlers){
	try{
	if(m_roadStatus == RAILROAD){
		std::cout<<"You cannot build some road on top of rail road"<<std::endl;
		return false;
	}
	if(m_roadStatus == NOTHING){
		if(m_Landscape()==RIVER&&(!(settlers.m_canBuildBridges()))){
			std::cout<<"Bridge building issues!"<<std::endl;
			return false;
		}
		settlers.m_work(BUILD_ROAD);
		if(!m_maybeFinishWork(settlers, BUILD_ROAD)){
			return true;
		}
	m_roadStatus = ROAD;
	return m_createRoadImage(brownColor);
	}

	//alter the drawing element

	if(m_roadStatus==ROAD){
		if(!settlers.m_canBuildRailroad()){
			std::cout<<"Settlers cannot build rail road!"<<std::endl;
			return false;
		}
		settlers.m_work(BUILD_ROAD);
		if(!m_maybeFinishWork(settlers, BUILD_ROAD)){
			return true;
		}
		m_roadStatus = RAILROAD;
		return m_createRoadImage(blackColor);
	}
	std::cout<<"For some reason, there is no return for Field::m_road"<<std::endl;
	return false;
	}
		catch(SettlersworkUnavailable& theException){
			settlers.m_loseOneWorkPoint();
			throw(theException);
		}
}

void Field::m_drawField(){
	try{
	m_drawingElement->m_drawAsRemembered(theRenderer);
	}
	catch(DrawingFail& theDrawingFail){
		std::cout<<"catching DrawingFail in Field::_drawField()"<<std::endl;
		throw(theDrawingFail);
	}
}

void Field::m_releaseFigure(std::shared_ptr<Figure> movingFigure){
	unsigned int listSize = m_figuresOnField.size();
	m_figuresOnField.remove_if([&movingFigure](std::shared_ptr<Figure> f)->bool{return f.get()==movingFigure.get();});
	if(listSize == m_figuresOnField.size())
		std::cout<<("Fail: Released figure not removed!")<<std::endl;
}

int Field::m_distanceTo(std::shared_ptr<City> comparedCity){
	if(!comparedCity){
		std::cout<<"compared City not found"<<std::endl;
		return 32;
	}
	int horizontalDistance = std::abs(comparedCity->m_WhereItStands()->m_x - m_x)/STANDARD_FIELD_SIZE;
	horizontalDistance = std::min(horizontalDistance, WORLD_LENGTH - 1 - horizontalDistance);
	int verticalDistance = std::abs(comparedCity->m_WhereItStands()->m_y - m_y)/STANDARD_FIELD_SIZE;
	std::cout<<"horizontal distance: "<<horizontalDistance<<"verticaldistance: "<<verticalDistance<<std::endl;
	return std::max(horizontalDistance, verticalDistance);
}

void Field::m_takeFigure(std::shared_ptr<Figure> movingFigure){
	std::cout<<"m_takeFigure aufgerufen: this = "<<*this<<", previousField: "<<movingFigure->m_WhereItStands()<<std::endl;
	Field& previousField = movingFigure->m_WhereItStands();
	if(m_figuresOnField.empty()){
		std::cout<<"auf leeren Stack"<<std::endl;
		if(m_cityContained && m_cityContained->m_OwningNation()->m_Nation()!=movingFigure->m_Nationality()){
			movingFigure->m_Nation()->m_captureCity(m_cityContained);
			if(m_cityContained){
				m_cityContained->m_createCitySurface().m_displaySurface(theRenderer);
			}
		}
		previousField.m_releaseFigure(movingFigure);
		m_figuresOnField.push_front(movingFigure);
		m_makeVisibleAround(movingFigure->m_visibilityRange());
		movingFigure->m_makeFiguresVisibleAround(shared_from_this());
		if(m_citizenWorking){
			std::cout<<"handle ctizen working"<<std::endl;
			std::cout<<m_citizenWorking->m_state<<std::endl;
			if(m_citizenWorking->m_home.m_OwningNation()->m_Nation()!=movingFigure->m_Nationality()){
				std::cout<<"handle ctizen working"<<std::endl;
				m_citizenWorking->m_state = ENTERTAINER;
				m_citizenWorking->m_whereItWorks = nullptr;
				m_citizenWorking = nullptr;
			}
		}
		return;
	}
	std::shared_ptr<Figure> frontFigure = m_figuresOnField.front();
	if(frontFigure->m_Nationality()==movingFigure->m_Nationality()){
		std::cout<<"auf eigenen Stack"<<", listPrevious: "<<m_figuresOnField.size();
		previousField.m_releaseFigure(movingFigure);
		m_figuresOnField.push_front(movingFigure);
		m_figuresOnField.sort([](std::shared_ptr<Figure> figure1, std::shared_ptr<Figure> figure2)->bool {return figure1->m_defensiveStrength()<figure1->m_defensiveStrength();});
		std::cout<<", newSize: "<<m_figuresOnField.size()<<std::endl;
		m_makeVisibleAround(movingFigure->m_visibilityRange());
		movingFigure->m_makeFiguresVisibleAround(shared_from_this());
		return;
	}
	if(frontFigure->m_Nationality()!=movingFigure->m_Nationality()){
		//Fight. Consider emitting some kind of signal and/or logging the random value.
		Nationality winningNationality = theGame->m_calculateWinnerInFight(movingFigure, m_figuresOnField.front());
		std::cout<<"fightwinner is "<<winningNationality<<std::endl;
		FightResult result;
		if(winningNationality == frontFigure->m_Nationality()){
			result = ATTACKER_LOSES;
			previousField.m_releaseFigure(movingFigure);
			movingFigure->m_Nation()->m_destroyFigure(movingFigure);
		}
		else{
			result = DEFENDER_LOSES;
			if(m_hasFortress){
			frontFigure->m_WhereItStands().m_releaseFigure(frontFigure);
			frontFigure->m_Nation()->m_destroyFigure(frontFigure);
			}
			else{
				while(!m_figuresOnField.empty()){
					std::cout<<"begin clearing loop for attacked field"<<std::endl;
					std::shared_ptr<Figure> currentFigure = m_figuresOnField.front();
					currentFigure->m_WhereItStands().m_releaseFigure(currentFigure);
					currentFigure->m_Nation()->m_destroyFigure(currentFigure);
					std::cout<<"release and destroy figure: figure.this = "<<currentFigure<<std::endl;
				}
			}
		}
		throw(Fight(result));
	}
}

bool Field::m_militaryProblem(std::shared_ptr<Figure> movingFigure){
	if(movingFigure->m_FigureType() == DIPLOMAT || movingFigure->m_FigureType() == CARAVAN || movingFigure->m_FigureCategory() != GROUND){
		return false;
	}
	for(std::shared_ptr<Figure> currentFigure: m_figuresOnField){
		if(currentFigure->m_Nationality() == movingFigure->m_Nationality()){
			return false;
		}
		if(currentFigure->m_FigureType() == DIPLOMAT || currentFigure->m_FigureType() == CARAVAN || currentFigure->m_FigureCategory() != GROUND){
			continue;
		}
		return true;
	}
	return false;
}

short int Field::m_getCargoCapability(Figure& figureToEnter){
	if(figureToEnter.m_FigureCategory()==SEA)
		return 0;
	if(m_figuresOnField.empty() || m_figuresOnField.front()->m_Nationality()!=figureToEnter.m_Nationality())
		return 0;
	short unsigned int cargoCount = 0;
	for(std::shared_ptr<Figure> currentFigure: m_figuresOnField){
		if((currentFigure->m_FigureCategory() == SEA) && figureToEnter.m_FigureCategory() == (currentFigure->m_FigureType() == CARRIER) ? FLIGHT : GROUND){
			std::cout<<"increasing capability via"<<currentFigure->m_FigureType()<<std::endl;
			cargoCount += reinterpret_cast<Ship*>(currentFigure.get())->m_cargoCountMax();
			continue;
		}
		if(currentFigure->m_FigureCategory()==figureToEnter.m_FigureCategory()){
			cargoCount--;
			continue;
		}
	}
	return cargoCount;
}

std::vector<std::shared_ptr<Field>> Field::m_cityFieldsAround(){
	std::vector<std::shared_ptr<Field>> whatToReturn;
	Direction direction1[] = {UP,UP,UP,UP_LEFT,UP,UP_RIGHT,UP_LEFT,UP_RIGHT,LEFT,LEFT,RIGHT,RIGHT,STANDING_STILL,DOWN_LEFT,DOWN_RIGHT,DOWN,DOWN_LEFT,DOWN_RIGHT,DOWN,DOWN,DOWN};
	Direction direction2[] = {UP_LEFT,UP,UP_RIGHT,STANDING_STILL,STANDING_STILL,STANDING_STILL,LEFT,
RIGHT,LEFT,STANDING_STILL,STANDING_STILL,RIGHT,
STANDING_STILL,LEFT,STANDING_STILL,STANDING_STILL,STANDING_STILL,RIGHT,DOWN_LEFT,DOWN,DOWN_RIGHT};
	for(int i(0); i<21; i++){
		try{
			whatToReturn.push_back(m_getNeighbouringField(direction1[i])->m_getNeighbouringField(direction2[i]));
		}
		catch(PoleHitException& poleHit){

		}
	}
	return whatToReturn;
}

std::vector<Coordinate> Field::coordinatesAroundCity(){
	int y[]={-2,-2,-2,-1,-1,-1,-1,-1,0,0,0,0,0,1,1,1,1,1,2,2,2};
	int x[]={-1,0,1,-1,0,1,-2,2,-2,-1,1,2,0,-2,1,0,-1,2,-1,0,1};
	std::vector<Coordinate> whatToReturn;
	for(int i(0); i<21; i++){
		whatToReturn.push_back(Coordinate(x[i],y[i]));
	}
	return whatToReturn;
}

bool Field::m_irrigationBonus(){
	return m_isIrrigated || m_cityContained != nullptr;
}

std::shared_ptr<Field> Field::m_getNeighbouringField(Coordinate differenceCoordinate){
	try{
	std::shared_ptr<Field> whatToReturn = shared_from_this();
	for(int i(0); i<differenceCoordinate.x;i++){
		whatToReturn = whatToReturn->m_getNeighbouringField(RIGHT);
	}
	for(int i(0); i>differenceCoordinate.x;i--){
			whatToReturn = whatToReturn->m_getNeighbouringField(LEFT);
	}
	for(int i(0); i<differenceCoordinate.y;i++){
		whatToReturn = whatToReturn->m_getNeighbouringField(DOWN);
	}
	for(int i(0); i>differenceCoordinate.y;i--){
		whatToReturn = whatToReturn->m_getNeighbouringField(UP);
	}
	return whatToReturn;
	}
	catch(PoleHitException& theException){
		throw theException;
	}
}

void Field::m_initNationFogInfo(std::vector<Nationality>& nationalities){
	for(Nationality nationality: nationalities){
		m_nationFogInfo.push_back(NationKnows{false,nationality});
	}
}

FieldElement::FieldElement(SDL_Renderer* renderer, std::shared_ptr<Texture> texture, int row, int column, Layer layer):ImmovableDrawingElement(renderer, texture, row, column, layer)
{

}

int FieldElement::m_draw(int rowShift, int columnShift, SDL_Renderer* renderer){
	if(m_field->m_isVisible(theGame->m_NationAtCurrentTurn()->m_Nation())){
		return ImmovableDrawingElement::m_draw(rowShift, columnShift, renderer) + (m_field->m_IsIrrigated() ? Graphics::Civ::drawIrrigation(renderer, m_row + rowShift, m_column + columnShift) : 0);
	}
	if(SDL_SetRenderDrawColor(theRenderer, blackColor)==0){
		SDL_Rect rectToFill{m_row + rowShift, m_column + columnShift, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE};
		if(SDL_RenderFillRect(theRenderer, &rectToFill)==0){
			return 1;
		}
		else{
			return 0;
		}
	}
	return 0;
}

void Field::m_makeVisible(Nationality nationality){
	for(NationKnows& nk: m_nationFogInfo){
		if(nk.nationality==nationality){
			std::cout<<"valuer becomes true for "<<nationality<<std::endl;
			nk.value = true;
			return;
		}
	}
}

void Field::m_makeVisibleAround(int visibilityRange){
	std::vector<Coordinate> coordinates = coordinatesAroundField(visibilityRange);
	for(Coordinate& currentCoordinate: coordinates){
		try{
		m_getNeighbouringField(currentCoordinate)->m_makeVisible(Graphics::Civ::currentNationality);
		}
		catch(PoleHitException& phe){

		}
	}
}

bool Field::m_isVisible(Nationality nationality){
	for(NationKnows& nk: m_nationFogInfo){
		if(nk.nationality==nationality){
			return nk.value;
		}
	}
	return false;
}

bool Field::m_closeToLandscape(Landscape ls){
std::vector<Coordinate> coordinates = coordinatesAroundField();
for(Coordinate& currentCoordinate: coordinates){
	try{
	if(m_getNeighbouringField(currentCoordinate)->m_Landscape()==ls){
		return true;
	}
	}
	catch(PoleHitException& pex){
		continue;
	}
}
return false;
}

short int Field::m_howLongToTake(SettlersWork work){
	return m_getFieldType().m_howLongToTake(work,this);
}

bool Field::m_Irrigate(Settlers& settlers){
	if(m_howLongToTake(IRRIGATE)==SETTLERSWORK_UNAVAILABLE){
		return false;
	}
	//if(!m_isIrrigated)
	settlers.m_work(IRRIGATE);
//Irrigation complete
	if(m_maybeFinishWork(settlers, IRRIGATE)){
		//set to irrigated
		m_getFieldType().m_irrigator.m_whatToDo(this);
	}
	return true;
}

bool Field::m_Mining(Settlers& settlers){
	if(m_howLongToTake(MAKE_MINING)==SETTLERSWORK_UNAVAILABLE){
		return false;
	}
	//if(!m_isIrrigated)
	settlers.m_work(MAKE_MINING);
//Irrigation complete
	if(m_maybeFinishWork(settlers, MAKE_MINING)){
		//set to irrigated
		 m_getFieldType().m_miningMaker.m_whatToDo(this);
	}
	return true;
}

void Field::m_setCityContained(std::shared_ptr<City> newCity){
	m_cityContained = nullptr; //BE CCCAREFUL USING THIS
}

int Field::m_shields(){
	return m_getFieldType().m_shields(this);
}
int Field::m_food(){
	return m_getFieldType().m_food(this);
}
int Field::m_trade(){
	return m_getFieldType().m_trade(this);
}

MovementPoints Field::m_movementPoints(){

	MovementData m =  m_getFieldType().m_movementData;

	return m.m_movementCost;
}

double Field::m_defenseBonus(){
	return m_getFieldType().m_movementData.defenseBonus;
}

void Field::m_changeLandscapeTo(Landscape landscape){
	m_landscape = landscape;
	m_isIrrigated = false;
	m_isMined = false;
	m_drawingElement->setTexture(m_getFieldType().m_texture);
}

std::string Field::m_resourceOverview(){return "Until very much later, forget about that nonsense!";}
