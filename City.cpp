/*
 * City.cpp
 *
 *  Created on: 11.05.2022
 *      Author: uwe-w
 */

#include "City.hpp"
#include "Settlers.hpp"
#include "Field.hpp"
#include "Drawing.hpp"
#include "Game.hpp"
#include <algorithm>

City::~City() {
	// TODO Auto-generated destructor stub
}

int drawCity(int x, int y, SDL_Renderer* renderer){
	fieldToDraw->m_CityContained()->m_drawCity(x, y, renderer);
}


CitySurface City::m_createCitySurface(){
	return CitySurface(this);
}

City::City(std::shared_ptr<Field> whereToPlace, std::shared_ptr<Nation> owningNation, std::string name)
: m_owningNation(owningNation), m_whereItStands(whereToPlace), m_name(name)
{
	//Imprecise. But I do not care that much about limiting the cities right now.
	m_whereItStands->m_DrawingElement()->m_climbToTop(CITY_LAYER);
	if(m_owningNation->m_Cities().size()>=CITIES_PER_NATION){
		throw(TooManyCities());
	}
	m_citizens.push_back(Citizen(*this,m_whereItStands->m_getNeighbouringField(UP)));
	/*SDL_Surface* surface = SDL_CreateRGBSurface(0, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE, 8,0,0,0,0);
	std::cout<<"SDL_Error: "<<SDL_GetError()<<std::endl;
int i = m_citizens.size();
	std::stringstream stream;
	stream<<i; stream.flush();
	SDL_Surface* textSurface = TTF_RenderText_Solid(theFont, stream.str().c_str(), brownColor);
	std::cout<<"SDL_Error: "<<SDL_GetError()<<std::endl;
SDL_Rect r{0,0,surface->w, surface->h};
	SDL_BlitSurface(textSurface, nullptr, surface, nullptr);
std::cout<<"SDL_Errorstr: "<<stream.str()<<std::endl;
	SDL_Texture* texture = SDL_CreateTextureFromSurface(theRenderer, surface);
	std::shared_ptr<Texture> s = std::make_shared<Texture>(texture, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE);
	whereToPlace->m_DrawingElement()->setTexture(s);
	SDL_RenderCopy(theRenderer, s.get()->theTexture(), NULL, NULL);
    SDL_RenderPresent(theRenderer);
	SDL_FreeSurface(surface);
	SDL_FreeSurface(textSurface);
	*/
	std::cout<<"City-Konstruktor"<<std::endl;
	m_whereItStands->m_DrawingElement()->m_setAdditionalInstructions(drawCity);
}

Citizen::Citizen(City& home, std::shared_ptr<Field> whereToWork):m_home(home)
{
	m_whereItWorks = nullptr;
	Nation& myNation = *m_home.m_OwningNation();
	int production = 0;
	int temp;
	for(std::shared_ptr<Field> currentField: m_home.m_WhereItStands()->m_cityFieldsAround()){
		temp = currentField->m_shields(myNation) + currentField->m_trade(myNation) + currentField->m_food(myNation);
		if(production < temp && !currentField->m_CityContained() && currentField!=m_home.m_WhereItStands()&& !currentField->m_CitizenWorking()){
			currentField->m_setCitizenWorking(this);
			production = temp;
			if(m_whereItWorks){
				m_whereItWorks->m_setCitizenWorking(nullptr);
			}
			m_whereItWorks = currentField;
		}
	}
	m_state = CONTENT;
}

int City::m_drawCity(int x, int y, SDL_Renderer* renderer){
	int whatToReturn = 0;
	int m_citizensSize = m_citizens.size();
	std::stringstream stream;
	stream<<m_citizensSize;
	stream.flush();
	SDL_Color newRenderColor = Nation::standardNationColor(m_owningNation->m_Nation());
	SDL_Surface* textSurface = TTF_RenderText_Solid(citySizeFont, stream.str().c_str(), blackColor);
	SDL_Rect rectToFill{x,y,STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE};
	whatToReturn += SDL_SetRenderDrawColor(theRenderer, newRenderColor.r, newRenderColor.g, newRenderColor.b, newRenderColor.a);
	whatToReturn += SDL_RenderFillRect(theRenderer, &rectToFill);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(theRenderer, textSurface);
	SDL_FreeSurface(textSurface);
	whatToReturn += SDL_RenderCopy(theRenderer, texture, NULL, &rectToFill);
	SDL_DestroyTexture(texture);
	m_drawCityName(x,y,renderer);
	if(m_whereItStands->m_FiguresOnField().empty()){
		return whatToReturn;
	}
	whatToReturn += Graphics::drawThickerHorizontalLine(renderer, x, y, STANDARD_FIELD_SIZE/12);
	whatToReturn += Graphics::drawThickerHorizontalLine(renderer, x, y + 11*STANDARD_FIELD_SIZE/12, STANDARD_FIELD_SIZE/12);
	whatToReturn += Graphics::drawThickerVerticalLine(renderer, x, y, STANDARD_FIELD_SIZE/12);
	whatToReturn += Graphics::drawThickerVerticalLine(renderer, x + 11*STANDARD_FIELD_SIZE/12, y, STANDARD_FIELD_SIZE/12);
	if(whatToReturn != 0) std::cout<<"SDL_Error in drawCity: "<<SDL_GetError()<<std::endl;
	return whatToReturn;
}
int City::m_drawCityName(int x, int y,SDL_Renderer* renderer){
	int whatToReturn(0);
	SDL_Color colorToUse = Graphics::Civ::cityOccupiedColor();
	SDL_SetRenderDrawColor(theRenderer, colorToUse.r, colorToUse.g,colorToUse.b,colorToUse.a);
	SDL_Surface* textSurface = TTF_RenderText_Solid(theFont, m_name.c_str(), Graphics::Civ::cityNameColor());
	SDL_Rect rectToFill{x - textSurface->w/2,y+STANDARD_FIELD_SIZE,textSurface->w, textSurface->h};
	SDL_Texture* texture = SDL_CreateTextureFromSurface(theRenderer, textSurface);
	SDL_FreeSurface(textSurface);
	whatToReturn += SDL_RenderCopy(theRenderer, texture, NULL, &rectToFill);
	SDL_DestroyTexture(texture);
	return whatToReturn;
}

bool City::m_releaseFigure(std::shared_ptr<Figure> figureToRelease){
	int sizeBefore = m_figuresOwned.size();
	m_figuresOwned.remove(figureToRelease);
	int sizeAfter = m_figuresOwned.size();
	if(sizeAfter - sizeBefore != -1){
		std::cout<<"City::m_releaseFigureFail: sizeBefore = "<<sizeBefore<<", sizeAfter = "<<sizeAfter<<std::endl;
		return false;
	}
	return true;
}

bool City::m_takeFigure(std::shared_ptr<Figure> figureToTake){
	int sizeBefore = m_figuresOwned.size();
	m_figuresOwned.push_back(figureToTake);
	m_figuresOwned.sort([](std::shared_ptr<Figure> figure1, std::shared_ptr<Figure> figure2)->bool{return figure1->m_shieldCost()<=figure2->m_shieldCost();});
	m_figuresOwned.unique();
	int sizeAfter = m_figuresOwned.size();
	if(sizeAfter - sizeBefore != 1){
		std::cout<<"City::m_takeFigureFail: sizeBefore = "<<sizeBefore<<", sizeAfter = "<<sizeAfter<<std::endl;
		return false;
	}
	return true;
}

std::vector<CitizenState> City::m_applyCitizenStateVector(HappyVectorType flag){
	int contentBaseApplied = 0;
	for(unsigned int index(0); index<m_citizens.size();index++){
		if(m_citizens[index].m_state == ENTERTAINER && m_citizens[index].m_state == SCIENTIST && m_citizens[index].m_state == TAX_COLLECTOR){
			contentBaseApplied++;
		}
	}
	int size = m_size();
	int contentApplied = 0;
	int luxuriesToApply = m_luxuriesProduction()/2;
	int luxuriesApplied = 0;
	int unhappyApplied = 0;
	std::vector<CitizenState> whatToReturn;
	int contentByUnitCount = 0;
	int unhappyByUnitCount = 0;
	for(UnitCostingResources& cost: m_unitCostVector()){
		(cost.unhappyFaces < 0 ? contentByUnitCount : unhappyByUnitCount) -= cost.unhappyFaces;
	}
	for(Citizen& citizen: m_citizens){
		if(citizen.m_state != ENTERTAINER && citizen.m_state != TAX_COLLECTOR && citizen.m_state != SCIENTIST){
			if(contentBaseApplied<CONTENT_BASE){
				citizen.m_state = CONTENT;
				contentBaseApplied++;
			}
			else{
				citizen.m_state = UNHAPPY;
			}
		}
		else{
		}
	}
	if(flag==HAPPY_1){
		std::cout<<"happy1goto"<<std::endl;
		goto beginReturn;
	}
	for(unsigned int i(0);i<m_citizens.size();i++){
		Citizen& citizen = m_citizens[size-1-i];
		try{
			if(luxuriesApplied>=luxuriesToApply){
				break;
			}
			++citizen.m_state;
			luxuriesApplied++;
			std::cout<<"luxuriesApplied = "<<luxuriesApplied<<", luxuriesToApply = "<<luxuriesToApply<<std::endl;
			if(luxuriesApplied>=luxuriesToApply){
				break;
			}
			++citizen.m_state;
			luxuriesApplied++;
			std::cout<<"luxuriesApplied = "<<luxuriesApplied<<", luxuriesToApply = "<<luxuriesToApply<<std::endl;
		}
		catch(InertCitizenState& exception){
			std::cout<<"inert luxury state ";
			std::cout.flush();
			continue;
		}
	}
	if(flag==HAPPY_2){
		std::cout<<"happy2goto"<<std::endl;
		goto beginReturn;
	}
	for(unsigned int i(0);i<m_citizens.size();i++){
		Citizen& citizen = m_citizens[size-1-i];
		try{
		if(contentApplied >= contentByUnitCount){
			break;
		}
		if(citizen.m_state == CONTENT){
			throw(InertCitizenState(CONTENT));
		}
		if(citizen.m_state!=CONTENT){
			++citizen.m_state;
			contentApplied++;
		}
		}
		catch(InertCitizenState& exception){
			std::cout<<"InertCitizenState when making content! ";
			std::cout.flush();
			continue;
		}
	}

	if(flag==HAPPY_3){
		std::cout<<"happy3goto"<<std::endl;
		goto beginReturn;
	}
	for(Citizen& citizen: m_citizens){
		try{
		if(unhappyApplied >= unhappyByUnitCount){
			break;
		}
			--citizen.m_state;
			unhappyApplied++;
		}
		catch(InertCitizenState& exception){
			std::cout<<"InertCitizenState when making unhappy! ";
			std::cout.flush();
			continue;
		}
	}
	beginReturn:	for(unsigned int i(0); i<m_citizens.size();i++){
		for(unsigned int j(0); j < m_citizens.size() - 1;j++){
			if((int) m_citizens[j].m_state > (int) m_citizens[j+1].m_state){
				CitizenState temp = m_citizens[j].m_state;
				m_citizens[j].m_state = m_citizens[j+1].m_state;
				m_citizens[j+1].m_state = temp;
			}
		}
	}
	for(Citizen& citizen: m_citizens){
		whatToReturn.push_back(citizen.m_state);
	}
	return whatToReturn;
}

int City::m_foodStorageSize(){
	return (m_citizens.size()+1)*10;
}

int City::m_size(){
	return m_citizens.size();
}

int City::m_foodCost(){
	int whatToReturn = m_size()*2;
	std::vector<UnitCostingResources> theVector(m_unitCostVector());
	for(const UnitCostingResources& cost: theVector){
		whatToReturn += cost.foodCost;
	}
	return whatToReturn;
}

int City::m_shieldCost(){
	int whatToReturn = 0;
	for(UnitCostingResources& cost: m_unitCostVector()){
		whatToReturn += cost.shieldCost;
	}
	return whatToReturn;
}

template<typename T>
bool isInVector(std::vector<T>& theVector, T& whatToFind, bool (*equals) (T& t1, T& t2)){
	for(T& t: theVector){
		if(equals(whatToFind, t)){
			return true;
		}
	}
	return false;
}

std::vector<UnitCostingResources> City::m_unitCostVector(){
	std::vector<UnitCostingResources> whatToReturn;
	int foodPerSettlers = 2;
	int freeUnits = 0;
	std::vector<FigureType> freeFigureTypes;
	freeFigureTypes.push_back(DIPLOMAT);
	freeFigureTypes.push_back(CARAVAN);
	int militaryLawMaximum = 0;
	int unhappyFacesPerUnit = 0;
	switch(m_owningNation->m_Government()){
	case DESPOTISM:
	{
		foodPerSettlers = 1;
		freeUnits = m_size();
		militaryLawMaximum = freeUnits;
		break;
	}
	case ANARCHY:
	{
		militaryLawMaximum = 3;
		freeUnits = m_size();
		foodPerSettlers = 1;
		break;
	}
	case COMMUNISM:
	{
		militaryLawMaximum = m_owningNation->m_Figures().size();
		break;
	}
	case MONARCHY:
	{
		militaryLawMaximum = 3;
		break;
	}
	case REPUBLIC:
	{
		unhappyFacesPerUnit = 1;
		break;
	}
	case DEMOCRACY:
	{
		unhappyFacesPerUnit = 2;
		break;
	}
	}
		int militaryLawAppliedCount = 0;
		std::vector<FigureType> irrelevantTypes = freeFigureTypes;
		irrelevantTypes.push_back(SETTLERS);
	for(std::shared_ptr<Figure> currentFigure: m_whereItStands->m_FiguresOnField()){
		if(militaryLawAppliedCount >= militaryLawMaximum){
			break;
		}
		FigureType currentFigureType = currentFigure->m_FigureType();
		if(!isInVector<FigureType>(irrelevantTypes, currentFigureType, [](FigureType& f1, FigureType& f2){return f1==f2;})){
		whatToReturn.push_back(UnitCostingResources{currentFigure.get(), 0, 0, 0});
			whatToReturn.back().unhappyFaces = -1;
			militaryLawAppliedCount++;
		}
	}
	int freeUnitsCount = 0;
	for(std::shared_ptr<Figure>& currentFigure: m_figuresOwned){
		whatToReturn.push_back(UnitCostingResources{currentFigure.get(), 0, 0, 0});
		FigureType currentFigureType = currentFigure->m_FigureType();
		if(currentFigureType==SETTLERS){
			whatToReturn.back().foodCost = foodPerSettlers;
			goto gonnaCostOneShield;
		}
		if(!isInVector<FigureType>(freeFigureTypes, currentFigureType, [](FigureType& f1, FigureType& f2){return f1==f2;})){
			gonnaCostOneShield: if(freeUnitsCount < freeUnits){
				whatToReturn.back().shieldCost = 0;
				freeUnitsCount++;
			}
			else{
				whatToReturn.back().shieldCost = 1;
			}
			if(&(currentFigure->m_WhereItStands())!=m_whereItStands.get()){
				whatToReturn.back().unhappyFaces = unhappyFacesPerUnit;
			}
		}
	}
	return whatToReturn;
}

int City::m_foodProduction(){
	int count = m_whereItStands->m_food(*m_owningNation);
	for(Citizen& citizen: m_citizens){
		if(citizen.m_whereItWorks){
			count+=citizen.m_whereItWorks->m_food(*m_owningNation);
		}
	}
	return count;
}

int City::m_shieldProduction(){
	int count = m_whereItStands->m_shields(*m_owningNation);
	for(Citizen& citizen: m_citizens){
		if(citizen.m_whereItWorks){
			count+=citizen.m_whereItWorks->m_shields(*m_owningNation);
		}
	}
	return count;
}

void City::m_sortFiguresByValue(){
	m_figuresOwned.sort([](std::shared_ptr<Figure> f1, std::shared_ptr<Figure> f2)->bool{
		if(f2->m_FigureType()==SETTLERS){
			return true;
		}
		std::vector<FigureType> freeFigureTypes;
		freeFigureTypes.push_back(DIPLOMAT);
		freeFigureTypes.push_back(CARAVAN);
		FigureType f = f2->m_FigureType();
		if(isInVector<FigureType>(freeFigureTypes, f, [](FigureType& f1, FigureType& f2){return f1==f2;})){
			return false;
		}
		return f1->m_shieldCost()<=f2->m_shieldCost();
	});
}

void City::m_startNewTurn(){
	//The case of food storage exhaustion is gonna be handled some days later
	if(m_food == m_foodStorageSize()){
		m_food = 0;
		m_grow();
	}
	m_sortFiguresByValue();
	m_food = std::min((m_foodProduction()+m_food - m_foodCost()), m_foodStorageSize());
	std::cout<<"food = "<<m_food<<std::endl;
}

void City::m_grow(){
	m_citizens.push_back(Citizen(*this));
}

int City::m_tradeProduction(){
	int count = m_whereItStands->m_trade(*m_owningNation);
	for(Citizen& citizen: m_citizens){
		if(citizen.m_whereItWorks){
			count+=citizen.m_whereItWorks->m_trade(*m_owningNation);
		}
	}
	return count;
}

int City::m_luxuriesProduction(){
	return m_tradeProduction();
}

bool City::m_placeCitizen(std::shared_ptr<Field> fieldClickedOn){
	if(fieldClickedOn->m_CityContained() != nullptr)
	{
		return false;
	}
	for(int i(m_citizens.size()-1); i>=0;i--){
		if(m_citizens[i].m_whereItWorks!=nullptr){
			if(m_citizens[i].m_whereItWorks == fieldClickedOn){
				m_citizens[i].m_whereItWorks->m_setCitizenWorking(nullptr);
				m_citizens[i].m_whereItWorks = nullptr;
				std::cout<<"make entertainer"<<std::endl;
				m_citizens[i].m_state = ENTERTAINER;
				return true;
			}
				continue;
		}
		m_citizens[i].m_whereItWorks = fieldClickedOn;
		m_citizens[i].m_state = CONTENT;
		std::cout<<"make fieldClickedOn"<<std::endl;
		return true;
	}
	return false;
}
