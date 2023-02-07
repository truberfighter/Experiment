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
#include "GameMain.hpp"
#include "Artillery.hpp"
#include "Diplomat.hpp"

Citizen::	~Citizen(){std::cout<<"Citizen-Destruktor: City: "<<m_home.m_Name()<<std::endl;}


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
	m_globalIndex = cityCountGlobal++;
	m_whereItStands->m_DrawingElement()->m_climbToTop(CITY_LAYER);
	if(m_owningNation->m_Cities().size()>=CITIES_PER_NATION){
		throw(TooManyCities());
	}
	m_improvements.push_back(CityImprovement{TEMPLE,this});
	m_improvements.push_back(CityImprovement{GRANARY,this});
	m_citizens.push_back(std::make_shared<Citizen>(*this,m_whereItStands->m_getNeighbouringField(UP)));
	std::cout<<"City-Konstruktor"<<m_citizens.front()->m_state<<std::endl;
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
		if(production < temp && currentField->m_isVisible(m_home.m_OwningNation()->m_Nation())&&!currentField->m_CityContained() && currentField!=m_home.m_WhereItStands()&& !currentField->m_CitizenWorking()){
			if(!currentField->m_FiguresOnField().empty())
				if(currentField->m_FiguresOnField().front()->m_Nationality()!=home.m_OwningNation()->m_Nation())
					continue;
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
	SDL_Color newRenderColor = Nation::standardNationColor(m_OwningNation()->m_Nation());
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
	std::cout<<"City::m_releaseFigureSuccess: sizeBefore = "<<sizeBefore<<", sizeAfter = "<<sizeAfter<<std::endl;
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
		if(m_citizens[index]->m_state == ENTERTAINER && m_citizens[index]->m_state == SCIENTIST && m_citizens[index]->m_state == TAX_COLLECTOR){
			contentBaseApplied++;
		}
	}
	int size = m_size();
	int contentApplied = 0;
	int luxuriesToApply = m_luxuriesRevenue()/2;
	int luxuriesApplied = 0;
	int unhappyApplied = 0;
	std::vector<CitizenState> whatToReturn;
	int contentByUnitCount = 0;
	int unhappyByUnitCount = 0;
	for(UnitCostingResources& cost: m_unitCostVector()){
		(cost.unhappyFaces < 0 ? contentByUnitCount : unhappyByUnitCount) -= cost.unhappyFaces;
	}
	for(std::shared_ptr<Citizen>& citizen: m_citizens){
		if(citizen->m_state != ENTERTAINER && citizen->m_state != TAX_COLLECTOR && citizen->m_state != SCIENTIST){
			if(contentBaseApplied<m_owningNation->m_firstUnhappyCitizen()){
				citizen->m_state = CONTENT;
				contentBaseApplied++;
			}
			else{
				citizen->m_state = UNHAPPY;
			}
		}
		else{
		}
	}
	if(flag==HAPPY_1){
		goto beginReturn;
	}
	for(unsigned int i(0);i<m_citizens.size();i++){
		Citizen& citizen = *m_citizens[size-1-i];
		try{
			if(luxuriesApplied>=luxuriesToApply){
				break;
			}
			++(citizen.m_state);
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
			continue;
		}
	}
	if(flag==HAPPY_2){
		goto beginReturn;
	}
	for(unsigned int i(0);i<m_citizens.size();i++){
		Citizen& citizen = *m_citizens[size-1-i];
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
			continue;
		}
	}

	if(flag==HAPPY_3){
		goto beginReturn;
	}
	for(std::shared_ptr<Citizen>& citizen: m_citizens){
		try{
		if(unhappyApplied >= unhappyByUnitCount){
			break;
		}
			--citizen->m_state;
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
			if((int) m_citizens[j]->m_state > (int) m_citizens[j+1]->m_state){
				CitizenState temp = m_citizens[j]->m_state;
				m_citizens[j]->m_state = m_citizens[j+1]->m_state;
				m_citizens[j+1]->m_state = temp;
			}
		}
	}
	for(std::shared_ptr<Citizen>& citizen: m_citizens){
		whatToReturn.push_back(citizen->m_state);
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
		if(!isInVector<FigureType>(irrelevantTypes, currentFigureType, [](const FigureType& f1,const FigureType& f2){return f1==f2;})){
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
		if(!isInVector<FigureType>(freeFigureTypes, currentFigureType, [](const FigureType& f1, const FigureType& f2){return f1==f2;})){
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

int City::m_foodProduction(){//modified for testing
	int count = 8*m_whereItStands->m_food(*m_owningNation);
	for(std::shared_ptr<Citizen>& citizen: m_citizens){
		if(citizen->m_whereItWorks){
			count+=citizen->m_whereItWorks->m_food(*m_owningNation);
		}
	}
	return count;
}

int City::m_shieldProduction(){
	int count = m_whereItStands->m_shields(*m_owningNation);
	for(std::shared_ptr<Citizen>& citizen: m_citizens){
		if(citizen->m_whereItWorks){
			count+=citizen->m_whereItWorks->m_shields(*m_owningNation);
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
		if(isInVector<FigureType>(freeFigureTypes, f, [](const FigureType& f1, const FigureType& f2){return f1==f2;})){
			return false;
		}
		return f1->m_shieldCost()<=f2->m_shieldCost();
	});
}

void City::m_startNewTurn(){
	m_buyInTurn = false;
	//The case of food storage exhaustion is gonna be handled some days later
	if(m_food == m_foodStorageSize()){
		m_food = 0;
		m_grow();
	}
	int figuresTooMuch = m_shieldCost()-m_shieldProduction();
	for(int i(0);i<figuresTooMuch;i++){
		std::cout<<"shieldCost: "<<m_shieldCost()<<std::endl;
		m_announceCannotMaintain((ImprovementType)m_figuresOwned.back()->m_FigureType());
		m_owningNation->m_destroyFigure(m_figuresOwned.back());
	}
	if(m_handlePollution()){
		std::stringstream stream;
		stream<<"Environmental report:\nPollution happened in "<<m_name<<std::endl;
		Miscellaneous::displayText(stream.str(),FIGURE_INFO_WIDTH,SCREEN_HEIGHT/3, Graphics::Civ::darkgreyColor());
		theGame->m_pollute();
	}
	m_sortFiguresByValue();
	m_maybeFinishProduction();
	m_food = std::min((m_foodProduction()+m_food - m_foodCost()), m_foodStorageSize());
	std::cout<<"food = "<<m_food<<std::endl;
	m_cityEconomy();
	m_owningNation->m_addProgress(m_scienceRevenue());
}

void City::m_grow(){
	m_citizens.push_back(std::make_shared<Citizen>(*this));
}

int City::m_tradeProduction(){
	int count = m_whereItStands->m_trade(*m_owningNation);
	for(std::shared_ptr<Citizen>& citizen: m_citizens){
		if(citizen->m_whereItWorks){
			count+=citizen->m_whereItWorks->m_trade(*m_owningNation);
		}
	}
	return count;
}

int City::m_luxuriesRevenue(){
	int production = m_revenueProduction().luxuries;
	return (production*m_luxuryCoefficient());
}

int City::m_goldRevenue(){
	int production = m_revenueProduction().gold;
	return (production*m_goldCoefficient());
}

int City::m_scienceRevenue(){
	int production = m_revenueProduction().science;
	return (production*m_scienceCoefficient());
}

bool City::m_placeCitizen(std::shared_ptr<Field> fieldClickedOn){
	if(fieldClickedOn->m_CityContained() != nullptr || !fieldClickedOn->m_isVisible(m_owningNation->m_Nation())
			|| (!fieldClickedOn->m_figuresOnField.empty() && fieldClickedOn->m_figuresOnField.front()->m_Nationality()!=m_owningNation->m_Nation()))
	{
		return false;
	}
	for(int i(m_citizens.size()-1); i>=0;i--){
		if(m_citizens[i]->m_whereItWorks!=nullptr){
			if(m_citizens[i]->m_whereItWorks == fieldClickedOn){
				m_citizens[i]->m_whereItWorks->m_setCitizenWorking(nullptr);
				m_citizens[i]->m_whereItWorks = nullptr;
				std::cout<<"make entertainer"<<std::endl;
				m_citizens[i]->m_state = ENTERTAINER;
				return true;
			}
				continue;
		}
		m_citizens[i]->m_whereItWorks = fieldClickedOn;
		m_citizens[i]->m_state = CONTENT;
		std::cout<<"make fieldClickedOn"<<std::endl;
		return true;
	}
	return false;
}




bool City::m_contains(ImprovementType imptype){
	for(CityImprovement& currentImprovement: m_improvements){
		if(currentImprovement.m_what == imptype){
			return true;
		}
	}
	return false;
}

CityImprovement::CityImprovement(ImprovementType what, City* home):m_what(what), m_home(home)
{
	std::cout<<"CityImprovement-Konstruktor: imptype = "<<m_what<<", home = "<<m_home<<": "<<home->m_Name()<<std::endl;
}


std::shared_ptr<CityImprovement> City::m_maybeBuild(ImprovementType imptype){
	if(m_contains(imptype)){
		return nullptr;
	}
	if(!isWonderType(imptype)||isBuildingType(imptype)){
		return nullptr;
	}
	if(City::isWonderType(imptype)){
		WonderData& wd = theGame->m_HasWonderBeenBuilt()[imptype - WONDER_MIN];
		if(wd.who != NO_NATIONALITY){
			return nullptr;
		}
		wd.who = m_owningNation->m_Nation();
		wd.city = this;
	}
	switch(imptype){
	case PALACE:
	{
		m_owningNation->m_setCapitalCity(shared_from_this());
		return std::make_shared<CityImprovement>(PALACE, this);
	}
	default:
	{
		return std::make_shared<CityImprovement>(imptype, this);
	}
	}return nullptr;
}



int City::m_corruptionProduction(){
	int governmentCoefficient;
	switch(m_owningNation->m_Government()){
	case DEMOCRACY:
	{
		return 0;
	}
	case ANARCHY:
	{
		governmentCoefficient = 12;
		break;
	}
	case DESPOTISM:
	{
		governmentCoefficient = 8;
		break;
	}
	case REPUBLIC:
	{
		governmentCoefficient = 24;
		break;
	}
	case MONARCHY:
	{
		governmentCoefficient = 16;
		break;
	}
	case COMMUNISM:
	{
		governmentCoefficient = 20;
		break;
	}
	}
	double distance;
	try{
	distance = m_whereItStands->m_distanceTo(m_owningNation->m_CapitalCity());
	}
	catch(NullPointerException& npe){
		if(npe.what() == "City"){
			distance = 32;
		}
	}
	int courthouseDivisor = m_contains(COURTHOUSE) ? 2 : 1;
	return (int)(distance*(double)(3*this->m_tradeProduction()))/(double)(courthouseDivisor * governmentCoefficient*10);
}

CityProduction City::m_revenueProduction(){
	int amount = m_tradeProduction(),
	taxRate = m_owningNation->m_TaxRate(),
	luxuriesRate = m_owningNation->m_LuxuriesRate(),
	scienceRate = TAX_RATE_STEP_COUNT - taxRate - luxuriesRate;
	for(std::shared_ptr<TradeRoute>& route: m_tradeRoutes){
		amount+=route->m_calculateProduction();
	}
	std::cout<<"tradeProsuction: "<<amount<<std::endl;
	int preluxury = ((amount*luxuriesRate)/TAX_RATE_STEP_COUNT)*TAX_RATE_STEP_COUNT;
	int pregold = ((amount*taxRate)/TAX_RATE_STEP_COUNT)*TAX_RATE_STEP_COUNT;
	int prescience = ((amount*scienceRate)/TAX_RATE_STEP_COUNT)*TAX_RATE_STEP_COUNT;
#define GOLD_ERROR amount*taxRate - pregold
#define LUXURY_ERROR amount*luxuriesRate - preluxury
#define SCIENCE_ERROR amount*scienceRate - prescience
	while(GOLD_ERROR + LUXURY_ERROR + SCIENCE_ERROR >0){
		int worstDeficit = std::max(GOLD_ERROR, std::max(SCIENCE_ERROR, LUXURY_ERROR));
		if(worstDeficit == GOLD_ERROR){
			pregold+=TAX_RATE_STEP_COUNT;
		}
		else{
			if(worstDeficit == SCIENCE_ERROR){
				prescience+=TAX_RATE_STEP_COUNT;
			}
			else{
				preluxury+=TAX_RATE_STEP_COUNT;
			}
		}
	}
	CityProduction whatToReturn{preluxury/TAX_RATE_STEP_COUNT, pregold/TAX_RATE_STEP_COUNT, prescience/TAX_RATE_STEP_COUNT, amount};
	std::cout<<"gold: "<<whatToReturn.gold<<", luxury: "<<whatToReturn.luxuries<<", science: "<<whatToReturn.science<<std::endl;
	for(std::shared_ptr<Citizen>& currentCitizen: m_citizens){
		switch(currentCitizen->m_state){
		case ENTERTAINER:
		{
			whatToReturn.luxuries+=2;
			break;
		}
		case TAX_COLLECTOR:
		{
			whatToReturn.gold+=2;
			break;
		}
		case SCIENTIST:
		{
			whatToReturn.science+=2;
			break;
		}
		default:
			break;
		}
	}
	if(m_owningNation->m_Government()==ANARCHY){
		whatToReturn.gold = 0;
		whatToReturn.science = 0;
	}
	return whatToReturn;
}

bool City::m_sell(int index){
	std::cout<<"Now the "<<m_improvements[index].m_what<<" is sold for "<<City::shieldsNeeded(m_improvements[index].m_what)<<std::endl;
	if(index >= m_improvements.size()){
		return false;
	}
	if(isWonderType(m_improvements[index].m_what)){
		return false;
	}
	ImprovementType imptype = m_improvements[index].m_what;
	std::vector<CityImprovement>::iterator deleter = m_improvements.begin()+index;
	m_improvements.erase(deleter);
	m_owningNation->m_receiveMoney(City::shieldsNeeded(imptype));
	std::cout<<"vectorsize: "<<m_improvements.size()<<std::endl;
	return true;
}

void City::m_buy(int price){
	m_buyInTurn = true;
	m_shields = shieldsNeeded(m_whatIsBuilt);
	m_owningNation->m_receiveMoney(-price);
}

void City::m_announceCannotMaintain(ImprovementType imptype){
	someDrawing->m_draw();
	SDL_RenderPresent(theRenderer);
	SDL_Delay(100);
	std::stringstream s;
	s<<"Domestic report:\n"<<m_name<<" cannot maintain\n"<<imptype<<"\n";s.flush();
	Miscellaneous::printMultipleLines(s.str(), ANNOUNCEMENT_X, ANNOUNCEMENT_Y, Graphics::Civ::resourcesWhiteColor(), true, Graphics::redColor());
	SDL_RenderPresent(theRenderer);
	SDL_Event e;
	bool quitSurface = false;
	while(!quitSurface){
		try{
		while(SDL_PollEvent(&e)){
			if(e.type == SDL_QUIT){
				throw SDLQuitException();
			}
			if(e.type==SDL_KEYDOWN || e.type == SDL_MOUSEBUTTONDOWN){
				quitSurface = true;
				break;
			}
		}
		}
		catch(QuitSurface &qs){
			quitSurface = true;
		}
		catch(SDLQuitException& sdlqe){
			throw sdlqe;
		}
	}
}

void City::m_cityEconomy(){
	std::cout<<"goldRevenue; "<<m_goldRevenue()<<", luxury revenue: "<<m_luxuriesRevenue()<<", scienceRevenue "<<m_scienceRevenue()<<std::endl;
	m_owningNation->m_receiveMoney(m_goldRevenue());
	for(int maintenanceIndex(0);maintenanceIndex<m_improvements.size();maintenanceIndex++){
		ImprovementType currentImptype = m_improvements[maintenanceIndex].m_what;
		if(City::isBuildingType(currentImptype)){
			try{
			m_owningNation->m_receiveMoney(-City::maintenanceNeeded(currentImptype));
			}
			catch(NegativeTreasury& nt){
				m_sell(maintenanceIndex);
				try{
					m_announceCannotMaintain(currentImptype);
				}
				catch(SDLQuitException& se){
					throw se;
				}
			}
		}
	}
}

void City::m_shrink(){
	m_citizens.back()->m_whereItWorks->m_setCitizenWorking(nullptr);
	m_citizens.pop_back();
	if(m_citizens.size()==0){
		m_owningNation->m_destroyCity(shared_from_this());
	}
}


int City::m_capturingValue(){
	int populationTotal = 0;
	for(std::shared_ptr<City> currentCity: m_owningNation->m_Cities()){
		populationTotal+= currentCity->m_size();
	}
	if(m_owningNation->m_CapitalCity()){
		populationTotal+=m_owningNation->m_CapitalCity()->m_size();
	}
	return m_owningNation->m_Treasury()*m_size()*(m_contains(PALACE) ? 2 : 1)/populationTotal;
}

bool City::m_isCivilDisorder(){
	int count = 0;
	for(CitizenState currentCitizenState: m_applyCitizenStateVector()){
		if(currentCitizenState == UNHAPPY){
			count--;
		}
		if(currentCitizenState == HAPPY){
			count++;
		}
	}
	return count < 0;
}

int City::m_revoltingCost(){
	return (int)(((double)(m_owningNation->m_Treasury()+1000)*m_size())/(m_isCivilDisorder() ? 2 : 1))/(3+m_whereItStands->m_distanceTo(m_owningNation->m_CapitalCity()));
}

float City::m_scienceCoefficient(){
	float whatToReturn = 10;
	if(m_contains(LIBRARY)){
		whatToReturn +=m_owningNation->m_libraryCoefficient();
	}
	if(m_contains(UNIVERSITY)){
		whatToReturn +=m_owningNation->m_libraryCoefficient();
	}
	whatToReturn*=m_owningNation->m_scienceCoefficient();
	if(m_contains(COPERNICUS_OBSERVATORY)){
		whatToReturn+=whatToReturn;
	}
	return whatToReturn;
}

float City::m_goldCoefficient(){
	float whatToReturn = 1;
	if(m_contains(BANK)){
		whatToReturn +=m_owningNation->m_libraryCoefficient();
	}
	if(m_contains(MARKETPLACE)){
		whatToReturn +=m_owningNation->m_libraryCoefficient();
	}
	return whatToReturn;
}

float City::m_luxuryCoefficient(){
	float whatToReturn = 1;
	if(m_contains(BANK)){
		whatToReturn +=m_owningNation->m_libraryCoefficient();
	}
	if(m_contains(MARKETPLACE)){
		whatToReturn +=m_owningNation->m_libraryCoefficient();
	}
	return whatToReturn;
}

bool City::m_offerRevolt(Diplomat& diplomat){
	int whatToPay = m_revoltingCost();
	std::stringstream revoltStream;
	someDrawing->m_draw();
	revoltStream<<"The city of "<<m_name<<" would revolt for "<<whatToPay; revoltStream.flush();
	SDL_Rect infoRect = Miscellaneous::printMultipleLines(revoltStream.str(), FIGURE_INFO_WIDTH, SCREEN_HEIGHT/3, Graphics::Civ::darkgreyColor());
	if(diplomat.m_Nation()->m_Treasury()<whatToPay){
		std::stringstream tooExpensive;
		tooExpensive<<"You just cannot afford it.";tooExpensive.flush();
		Miscellaneous::displayText(tooExpensive.str(), FIGURE_INFO_WIDTH, SCREEN_HEIGHT/3+infoRect.h, Graphics::Civ::darkgreyColor());
		return false;
	}
	if(!Miscellaneous::yesOrNo(FIGURE_INFO_WIDTH,SCREEN_HEIGHT/3+infoRect.h)){
		return false;
	}
	if(m_size()!=1){
		m_shrink();
	}
	this->m_owningNation = diplomat.m_Nation();
	for(std::shared_ptr<Figure> unsureFigure: m_figuresOwned){
		if(!unsureFigure->m_WhereItStands().m_isVisible(diplomat.m_Nationality()))
		{
			m_owningNation->m_destroyFigure(unsureFigure);
		}
		else{
			unsureFigure->m_changeNationTo(diplomat.m_Nation(),shared_from_this());
		}
	}
	int loot = this->m_capturingValue();
	std::stringstream captureStream;
	captureStream<<this->m_Name()<<" captured by "<<diplomat.m_Nationality()<<"!\n "<<loot<<" gold pieces plundered!"<<std::endl;
	Miscellaneous::displayText(captureStream.str(), 200, 200, Nation::standardNationColor(diplomat.m_Nationality()), true, diplomat.m_Nationality() == ROMAN || diplomat.m_Nationality() == RUSSIAN ? blackColor : Graphics::Civ::resourcesWhiteColor());
	//now remove some buildings
	for(std::vector<std::shared_ptr<City>>::iterator it = this->m_OwningNation()->m_Cities().begin(); it!= this->m_OwningNation()->m_Cities().end();it++){
		if(it->get() == this){
			m_OwningNation()->m_Cities().erase(it);
			break;
		}
	}
	std::cout<<"size: "<<this->m_size()<<std::endl;
	diplomat.m_Nation()->m_Cities().push_back(shared_from_this());
	std::cout<<"captured!"<<std::endl;
	return true;
}

int City::m_everydaysPollution(){
	if(m_contains(MASS_TRANSIT))
		return 0;
	int coefficient = m_owningNation->m_pollutionCoefficient();
	if(coefficient==0)
		return 0;
	return (m_size()*coefficient)/4;
}

int City::m_pollutionProduction(){
	return std::max(m_everydaysPollution()+m_shieldProduction()/m_shieldsPerPollution()-20,0);
}

int City::m_shieldsPerPollution(){
	if(m_contains(RECYCLING_CENTER))return 3;
	if(m_contains(HYDRO_PLANT)) return 2;
	if(m_contains(NUCLEAR_PLANT)) return 2;
	if(theGame->m_isWonderOnContinent(*this, HOOVER_DAM))
		return 2;
	return 1;
}

bool City::m_handlePollution(){
	int probabilityPercent = std::min(100,m_pollutionProduction());
	if(probabilityPercent==0)
		return false;
	int randomNumber1 = theGame->m_getRandomNumberBetween(1, 100);
	if(randomNumber1>probabilityPercent){
		return false;
	}
	int fieldIndex;
	std::vector<std::shared_ptr<Field>> fields = m_whereItStands->m_cityFieldsAround();
	for(int i(20); i>=0;i--){
		Field* currentField = (fields)[i].get();
		if(currentField->m_CityContained()||!currentField->m_isVisible(m_owningNation->m_Nation())|| currentField->m_IsPolluted()){
			fields.erase(fields.begin()+i);
		}
	}
	int randomNumber2 = theGame->m_getRandomNumberBetween(0, fields.size());
	if(!fields.empty())
	return fields[randomNumber2]->m_pollute();
	return false;
}

