/*
 * Nation.cpp
 *
 *  Created on: 19.03.2022
 *      Author: uwe-w
 */

#include "Nation.hpp"
#include "sdltypes.hpp"
#include <string>
#include "FieldContainer.hpp"
#include "City.hpp"
#include "Game.hpp"
#include "SelectorSurface.hpp"
#include <functional>
#include <iomanip>
#include "GameMain.hpp"
#include <algorithm>
#include "Embassy.hpp"

using namespace std;

Nation::~Nation(){std::cout<<"Nationdestruktor"<<this<<std::endl;}


Nation::Nation(Nationality nationality, std::string leaderName, Difficulty difficulty, bool directlyMakingFiguresActive): m_nation(nationality), m_directlyMakingFiguresActive(directlyMakingFiguresActive), m_leaderName(leaderName), m_difficulty(difficulty),enable_shared_from_this()
{
	cout<<"Nation-Konstruktor mit Nationalität "<< nationality<<", this = "<<this<<endl;
}

bool Nation::m_canBuildRailroad(){
	return m_hasExplored(TECHNOLOGY_RAILROAD);
}

bool Nation::m_canBuildBridges(){
	return m_hasExplored(TECHNOLOGY_BRIDGE_BUILDING);
}

const std::string& Nation::m_LeaderName(){
	if(!m_leaderName.compare(""))
		return m_leaderName;
	switch(m_nation){
	case ROMAN:
		return "Iulius Caesar.png";
	case MONGOL:
		return "Dschinghis Khan.png";
	case GERMAN:
		return "Otto von Bismarck.png";
	case CHINESE:
		return "Xi Jinping.png";
	default:
		return "ANONYMOUS.png";
	}
}

void Nation::m_makeFigureWait(){
	std::shared_ptr<Figure> whatToWait = m_getCurrentFigure();
	if(!whatToWait){
		return;
	}
	if(!m_removeFromQueue(whatToWait)){
		throw("Waiting fail");
	}
	m_addToQueue(whatToWait);
}

bool Nation::m_removeFromQueue(std::shared_ptr<Figure> figureToRemove){
	short int sizeToCompare = m_activeFigures.size();
	m_activeFigures.remove(figureToRemove);
	if(m_activeFigures.size()==sizeToCompare){
		return false;
	}
	return true;
}

Coordinate Nation::getStandardCoordinateForNation(Nationality n){
	switch(n){
	case GERMAN:
		return Coordinate(30,30);
	case ROMAN:
		return Coordinate(15,15);
	default:{
		return Coordinate((int) n , (int) n);
		cout<<"Make better standardcoordinates"<<std::endl;
	}
}
}

std::shared_ptr<Figure> Nation::m_getCurrentFigure(){
	//auto& f = m_activeFigures;
	//cout<<"m_getCurrentFigure(), &m_activeFigures = " <<&m_activeFigures<<endl;

	//cout<< m_activeFigures.front()<<std::endl;
	if(m_activeFigures.empty()){
		cout<<"empty list"<<std::endl;
		return nullptr;
	}
	return m_activeFigures.front();
}

void Nation::m_addFigure(std::shared_ptr<Figure> newFigure){
	if(!newFigure){
		std::string myString("m_addFigure");
		myString+=(int)newFigure.get();
		myString+=").png";
		throw(NullPointerException("Nation",myString));

	}
	m_figures.push_back(newFigure);
	if(m_directlyMakingFiguresActive){
		if(!m_addToQueue(newFigure))
			cout<<"m_addToQueueFail"<<m_activeFigures.size()<<endl;	}
}

bool Nation::m_addToQueue(std::shared_ptr<Figure> newFigure){
	if(!newFigure){
		std::string myString("m_addToQueue");
		myString+=(int)newFigure.get();
		myString+=").png";
		throw(NullPointerException("Nation",myString));
	}
	m_activeFigures.push_back(newFigure);
	cout<<"size: "<<m_activeFigures.size();
	unsigned int previousSize = m_activeFigures.size();
	m_activeFigures.unique();
	cout<<", "<<m_activeFigures.size()<<endl;
	newFigure->m_setFigureState(MOVING);
	return previousSize == m_activeFigures.size();
}

void Nation::m_setTaxRate(short unsigned int tax){
	//Automatic adjustion, if needed
	m_taxRate = tax;
	m_luxuriesRate = std::min(10-(int) m_taxRate, (int) m_luxuriesRate);
}

void Nation::m_setLuxuriesRate(short unsigned int luxuries){
	m_luxuriesRate = luxuries;
}

bool Nation::m_alterLuxuriesRate(){
	try{
	std::cout<<"m_alterLuxuriesRrate: taxRate = "<<m_taxRate<<", luxuriesRate = "<<m_luxuriesRate<<std::endl;
	if(m_taxRate == 10)
		return false;
	std::vector<std::shared_ptr<SelectionElement>> whatToSelectFrom;
	int whatIsPossible = 10 - m_taxRate;
	for(int i(0);i<=whatIsPossible;i++){
		std::stringstream stream;
		stream<<"Luxuries Rate:"<<std::setw(2)<<i<<"0%, Science Rate:"<<std::setw(2)<<whatIsPossible-i<<"0%";
		stream.flush();
		std::function<void()> dn = [](){};
		std::shared_ptr<SelectionElement> selectionElement = std::make_shared<SelectionElement>(stream.str(),0,dn);
		whatToSelectFrom.push_back(selectionElement);
	}
	std::stringstream stringStream;
	stringStream<<"Tax Rate:"<<std::setw(2)<<m_taxRate<<"0%, Luxuries Rate:"<<std::setw(2)<<m_luxuriesRate<<"0%, Science Rate:"<<std::setw(2)<<10-m_luxuriesRate - m_taxRate<<"0%";
	stringStream.flush();
	SDL_Surface* textSurface = TTF_RenderText_Shaded(theFont,stringStream.str().c_str(),blackColor,Graphics::Civ::resourcesWhiteColor());
	SDL_Texture* textTexture = SDL_CreateTextureFromSurface(theRenderer, textSurface);
	SDL_Rect rect{0,(whatIsPossible+2)*textSurface->h,textSurface->w,textSurface->h};
	SDL_RenderCopy(theRenderer, textTexture, nullptr, &rect);
	SDL_RenderPresent(theRenderer);
	SDL_FreeSurface(textSurface);
	SDL_DestroyTexture(textTexture);

	SelectorSurface selectorSurface(0,0,whatToSelectFrom);
	m_setLuxuriesRate(selectorSurface.m_fetchSelection().index);
	return true;
}
	catch(QuitSelection& qs){
		if(qs.m_returnSomething == NO_ACTION)
			return false;
		throw qs;
	}
}

bool Nation::m_alterTaxRate(){
	try{
	std::cout<<"m_alterTaxRate: taxRate = "<<m_taxRate<<", luxuriesRate = "<<m_luxuriesRate<<std::endl;
	std::vector<std::shared_ptr<SelectionElement>> whatToSelectFrom;
	for(int i(0);i<=10;i++){
		std::stringstream stream;
		stream<<"Tax Rate:"<<std::setw(2)<<i<<"0%, Luxuries Rate:"<<std::setw(2)<<std::min(10-(int)i, (int)m_luxuriesRate)<<"0%, Science Rate:"<<std::setw(2)<<std::max(10-i-(int)m_luxuriesRate,0)<<"0%";
		stream.flush();
		std::function<void()> dn = [](){};
		std::shared_ptr<SelectionElement> selectionElement = std::make_shared<SelectionElement>(stream.str(),0,dn);
		whatToSelectFrom.push_back(selectionElement);
	}
	std::stringstream stringStream;
		stringStream<<"Tax Rate:"<<std::setw(2)<<m_taxRate<<"0%, Luxuries Rate:"<<std::setw(2)<<m_luxuriesRate<<"0%, Science Rate:"<<std::setw(2)<<10-m_luxuriesRate - m_taxRate<<"0%";
		stringStream.flush();
		SDL_Surface* textSurface = TTF_RenderText_Shaded(theFont,stringStream.str().c_str(),blackColor,Graphics::Civ::resourcesWhiteColor());
		SDL_Texture* textTexture = SDL_CreateTextureFromSurface(theRenderer, textSurface);
		SDL_Rect rect{0,12*textSurface->h,textSurface->w,textSurface->h};
		SDL_RenderCopy(theRenderer, textTexture, nullptr, &rect);
		SDL_RenderPresent(theRenderer);
		SDL_FreeSurface(textSurface);
		SDL_DestroyTexture(textTexture);SelectorSurface selectorSurface(0,0,whatToSelectFrom);
	m_setTaxRate(selectorSurface.m_fetchSelection().index);
	return true;
	}
	catch(QuitSelection qs){
		if(qs.m_returnSomething == NO_ACTION)
			return false;
		throw qs;
	}

}

short unsigned int Nation::m_LuxuriesRate(){return m_luxuriesRate;}
short unsigned int Nation::m_TaxRate(){return m_taxRate;}


void Nation::m_startNewTurn(){
	try{
	Graphics::Civ::currentNationality = m_Nation();
		for(std::shared_ptr<Figure>& currentFigureToCheck : m_figures){
			if(currentFigureToCheck->m_FigureState()==MOVING){
				throw(TurnEndsTooEarly());
			}
		}
	m_activeFigures = std::list<std::shared_ptr<Figure>>();
	cout<<"Nation::m_startNewTurn(), m_figures.size() = "<<m_figures.size()<<", m_activeFiguresSize = "<<m_activeFigures.size()<<std::endl;
	for(std::shared_ptr<City> currentCityStarting: m_cities){
		currentCityStarting->m_startNewTurn();
	}
	for(std::shared_ptr<Figure>& currentFigure : m_figures){
		if(!currentFigure->m_startMove()){
			cout<<"Not what I wanted!"<<std::endl;
			throw(MoveStartFail("not yet defined"));
		}
		if(currentFigure->m_FigureState()==MOVING){
		cout<<"m_startNewTurn"<<endl<<"m_activeFiguresSize: "<<m_activeFigures.size();
			if(m_addToQueue(currentFigure))
				cout<< m_activeFigures.front()<<std::endl;
//			cout<<", m_activeFigures.size() = "<<m_activeFigures.size()<<", &m_activeFigures = "<<&m_activeFigures<<", nation = "<<this<<endl;
		}
	}
	}
	catch(TurnEndsTooEarly& theException){
		throw(theException);
	}
}

GovernmentType Nation::m_Government(){
	return m_government;
}

Nationality Nation::m_Nation() const{
	return this->m_nation;
}

std::vector<std::shared_ptr<Figure>>& Nation::m_Figures(){
	return m_figures;
}

bool Nation::m_destroyFigure(std::shared_ptr<Figure> figureToRemove){
	std::cout<<"m_destroyFigure: figure.this = "<<figureToRemove<<std::endl;
	figureToRemove->m_WhereItStands().m_releaseFigure(figureToRemove);
	if(figureToRemove->m_Home()){
		if(!figureToRemove->m_Home()->m_releaseFigure(figureToRemove))
			throw 1111;
	}
	m_removeFigure(figureToRemove);
	return true;
}

std::string Nation::m_colorString(){
	switch(m_nation){
	case ROMAN:
		return "RomanRussian.png";
	case RUSSIAN:
		return "RomanRussian.png";
	case BABYLONIAN:
		return "BabylonianZulu.png";
	case ZULU:
		return "BabylonianZulu.png";
	case EGYPTIAN:
		return "AztecianEgyptian.png";
	case AZTEC:
		return "AztecEgyptian.png";
	case MONGOL:
		return "IndianMongol.png";
	case INDIAN:
		return "IndianMongol.png";
	case AMERICAN:
		return "AmericanChinese.png";
	case CHINESE:
		return "AmericanChinese.png";
	case ENGLISH:
		return "EnglishGreekpng";
	case GREEK:
		return "EnglishGreek.png";
	case FRENCH:
		return "FrenchGerman.png";
	case GERMAN:
		return "FrenchGerman.png";
	}
	return "Some mistake!";
}

SDL_Color Nation::standardNationColor(Nationality nationalityToDraw){
	switch(nationalityToDraw){
	case ROMAN:
		return SDL_Color{220,220,220};
	case RUSSIAN:
		return SDL_Color{220,220,220};
	case BABYLONIAN:
		return SDL_Color{0,255,0,0};
	case ZULU:
		return SDL_Color{0,255,0,0};
	case EGYPTIAN:
		return SDL_Color{120,255,30,0};
	case AZTEC:
		return SDL_Color{120,255,30,0};
	case MONGOL:
		return SDL_Color{50,40,50,0};
	case INDIAN:
		return SDL_Color{50,40,50,0};
	case AMERICAN:
		return SDL_Color{0,30,255,0};
	case CHINESE:
		return SDL_Color{0,30,255,0};
	case ENGLISH:
		return SDL_Color{180,20,80,0};
	case GREEK:
		return SDL_Color{180,20,80,0};
	case FRENCH:
		return SDL_Color{0,0,255,0};
	case GERMAN:
		return SDL_Color{0,0,255,0};
	case BARBARIAN:
		return SDL_Color{255,0,0,0};
	}
	//This color should NEVER appear
	return SDL_Color{100,0,100,0};
}

void Nation::m_receiveMoney(int amount){
	m_treasury+=amount;
	if(m_treasury<0){
		throw NegativeTreasury(m_treasury-amount,m_treasury);
	}
}

bool Nation::m_hasExplored(Technology tech){
	if(tech==NO_TECHNOLOGY){
		return true;
	}
	return std::find(m_exploredTechnologies.begin(),m_exploredTechnologies.end(),tech)!=m_exploredTechnologies.end();
}

std::vector<Technology> Nation::m_technologiesAvailable(){
	if((int)m_exploredTechnologies.back()>TECHNOLOGY_MAX){
		return std::vector<Technology>{(Technology)(m_exploredTechnologies.back()+1)};
	}
	std::vector<Technology> whatToReturn;
	for(int technologyIndex(TECHNOLOGY_MIN);technologyIndex<=TECHNOLOGY_MAX;technologyIndex++){
		if(m_hasExplored((Technology)technologyIndex)){
			std::cout<<"continued!"<<std::endl;
			continue;
		}
		TechnologyDependency td = Science::techInfo((Technology)technologyIndex);
		if(m_hasExplored(td.neededTech1)&&m_hasExplored(td.neededTech2)){
			std::cout<<"pushed!"<<std::endl;
			whatToReturn.push_back((Technology) technologyIndex);
		}
	}
	if(m_exploredTechnologies.size()==TECHNOLOGY_MAX - TECHNOLOGY_MIN + 1){
		return std::vector<Technology>{(Technology)(TECHNOLOGY_MAX+1)};
	}
	return whatToReturn;
}

Technology Nation::m_askForNewExploration(){
	try{
	std::vector<Technology> whatToChooseFrom = m_technologiesAvailable();
	std::stringstream stream1;
	stream1<<"What do you wanna explore, most trustworthy leader "<<m_leaderName<<" of the "<<m_nation<<" Civilization?"<<std::endl;
	SDL_Rect theRect;
	theRect = Miscellaneous::printMultipleLines(stream1.str(), 0, 0., whiteColor, true, Graphics::Civ::irrigationBlueColor());
	std::vector<std::shared_ptr<SelectionElement>> selectionBase;
	for(Technology techToAskFor: whatToChooseFrom){
		std::cout<<"Technology available: "<<techToAskFor<<std::endl;
		std::function<void()> rightClick = [techToAskFor](){technologyRightClick(techToAskFor);};
		std::stringstream stream2;
		stream2<<techToAskFor;stream2.flush();
		std::shared_ptr<SelectionElement> element = std::make_shared<SelectionElement>(stream2.str(),(int)techToAskFor,rightClick);
		selectionBase.push_back(element);
	}
	std::cout<<"selectionbasesize: "<<selectionBase.size()<<std::endl;
	SelectorSurface surface(theRect.x,theRect.h+theRect.y, selectionBase, false, Graphics::Civ::shieldGreyColor(), blackColor, Graphics::Civ::brightCityBackgroundColor(),
			[](){
		SDL_RenderClear(theRenderer);
		someDrawing->m_draw();});
	SelectionReturn result;
	result = surface.m_fetchSelection();
	return whatToChooseFrom[result.index];
	}
	catch(SDLQuitException& exception){
		throw exception;
	}
	catch(QuitSelection& qs){
		if(qs.m_returnSomething==NO_ACTION){
			return m_askForNewExploration();
		}
		else{
			throw qs;
		}
	}
}

bool Nation::m_maybeFinishExploration(){
	if(m_explorationProgress>m_howMuchNeededForExploration()){
		std::cout<<"techovershot thrown"<<std::endl;
		throw TechnologyOvershoot(m_howMuchNeededForExploration(),m_explorationProgress);
	}
	if(m_explorationProgress==m_howMuchNeededForExploration()){
		m_addTechnology(m_whatToExplore);
		m_whatToExplore = NO_TECHNOLOGY;
		std::cout<<"Kevin"<<std::endl;
		return true;
	}
	return false;
}

int Nation::m_howMuchNeededForExploration(){
	return (6+2*(int)m_difficulty)*(m_exploredTechnologies.size());
}

void Nation::m_addTechnology(Technology tech){
	if(gameReady)
	theGame->m_acknowledgeExploration(tech,m_nation);
	m_exploredTechnologies.push_back(tech);
	if(tech==m_whatToExplore){
		m_explorationProgress = 0;
	}
}

void Nation::m_addProgress(int progress){
	if(progress<0){
		//Not clear what I wanna do here.
		m_explorationProgress+=progress;
	}
	bool finishInstantly = false;
	bool notTheFirst = false;
	while(progress>0){
		if(m_whatToExplore==NO_TECHNOLOGY){
			if(notTheFirst){
				SDL_RenderClear(theRenderer);
			}
			m_whatToExplore = m_askForNewExploration();
		}
		try{
			if(!finishInstantly){
				m_explorationProgress += progress;
				progress = 0;
			}
			if(m_maybeFinishExploration()){
				notTheFirst = true;
			}
			finishInstantly = false;
		}
		catch(TechnologyOvershoot& ts){
			progress = ts.whatsThere - ts.whatNeeded;
			m_explorationProgress = ts.whatNeeded;
			finishInstantly = true;
		}
	}
}

void Nation::m_setCapitalCity(std::shared_ptr<City> city){
	if(m_capitalCity){
		for(int improvementIndex(0); improvementIndex<m_capitalCity->m_Improvements().size(); improvementIndex++){
			if(m_capitalCity->m_Improvements()[improvementIndex].m_what==PALACE){
				m_capitalCity->m_sell(improvementIndex);
				break;
			}
		}
	}
	m_capitalCity = city;
}

void Nation::m_destroyCity(std::shared_ptr<City> cityToDestroy){
	for(std::vector<std::shared_ptr<City>>::iterator it = theGame->m_CitiesAlive().begin();it!=theGame->m_CitiesAlive().end();it++){
		if(cityToDestroy.get()==it->get()){
			theGame->m_CitiesAlive().erase(it);
			break;
		}
	}
	//clear the contents of the city

	while(!cityToDestroy->m_tradeRoutes.empty()){
		cityToDestroy->m_tradeRoutes.front()->m_destroyItself();
	}
	while(!cityToDestroy->m_figuresOwned.empty()){
		cityToDestroy->m_figuresOwned.front()->m_WhereItStands().m_releaseFigure(cityToDestroy->m_figuresOwned.front());
		m_destroyFigure(cityToDestroy->m_figuresOwned.front());
	}
	for(const CityImprovement& currentImprovement: cityToDestroy->m_Improvements()){
		currentImprovement.whenDestroyed(cityToDestroy.get());
		if(City::isWonderType(currentImprovement.m_what)){
			theGame->m_HasWonderBeenBuilt()[currentImprovement.m_what - WONDER_MIN].isDestroyed = true;
		}
	}
	for(std::vector<std::shared_ptr<City>>::iterator it = m_cities.begin();it!=m_cities.end();it++){
		if(cityToDestroy.get()==it->get()){
			m_cities.erase(it);
			break;
		}
	}
	for(std::vector<std::shared_ptr<City>>::iterator it = theGame->m_CitiesAlive().begin();it!=theGame->m_CitiesAlive().end();it++){
		if(cityToDestroy.get()==it->get()){
			theGame->m_CitiesAlive().erase(it);
			break;
		}
	}
	cityToDestroy->m_WhereItStands()->m_setCityContained(nullptr);
	cityToDestroy->m_WhereItStands()->m_DrawingElement()->m_setAdditionalInstructions([](int x,int y,SDL_Renderer* renderer)->int{return 0;});
}

void TradeRoute::m_destroyItself(){
	std::vector<std::shared_ptr<TradeRoute>>& theVector = m_city1->m_tradeRoutes;
	for(auto it = theVector.begin(); it!= theVector.end(); it++){
		if(this==it->get()){
			theVector.erase(it);
			break;
		}
	}
	theVector = m_city2->m_tradeRoutes;
	for(auto it = theVector.begin(); it!= theVector.end(); it++){
		if(this==it->get()){
			theVector.erase(it);
			break;
		}
	}
}

void Nation::m_captureCity(std::shared_ptr<City> cityToCapture){
	int loot = cityToCapture->m_capturingValue();
	std::stringstream captureStream;
	captureStream<<cityToCapture->m_Name()<<" captured by "<<m_nation<<"!\n "<<loot<<" gold pieces plundered!"<<std::endl;
	Miscellaneous::displayText(captureStream.str(), 200, 200, standardNationColor(m_nation), true, m_nation == ROMAN || m_nation == RUSSIAN ? blackColor : Graphics::Civ::resourcesWhiteColor());
	//now remove some buildings
	for(std::vector<std::shared_ptr<City>>::iterator it = cityToCapture->m_OwningNation()->m_cities.begin(); it!= cityToCapture->m_OwningNation()->m_cities.end();it++){
		if(it->get() == cityToCapture.get()){
			cityToCapture->m_OwningNation()->m_cities.erase(it);
			break;
		}
	}
	cityToCapture->m_shrink();
	std::cout<<"shrunk!"<<std::endl;
	bool destroy = false;
	std::vector<ImprovementType> eraseData;
	for(int index(0); index<cityToCapture->m_improvements.size();index++){
		if(cityToCapture->m_Improvements()[index].m_what==PALACE){
			destroy = false;
			eraseData.push_back(PALACE);
			cityToCapture->m_OwningNation()->m_capitalCity = nullptr;
		}
		if(destroy){
			eraseData.push_back(cityToCapture->m_improvements[index].m_what);
		}
		destroy = !destroy;
	}
	std::cout<<"citytocapture: "<<cityToCapture.get()<<std::endl;
	for(ImprovementType imptype: eraseData){
		forbegin:
		for(std::vector<CityImprovement>::iterator it = cityToCapture->m_improvements.begin(); it!= cityToCapture->m_improvements.end();it++){
			if(it->m_what == imptype){
				cityToCapture->m_improvements.erase(it);
				goto forbegin;
			}
		}
	}
	while(!cityToCapture->m_figuresOwned.empty()){
		std::shared_ptr<Figure> figureToRelease = cityToCapture->m_figuresOwned.front();
		std::cout<<"releasing "<<figureToRelease->m_FigureType()<<std::endl;
		figureToRelease->m_WhereItStands().m_releaseFigure(figureToRelease);
		cityToCapture->m_OwningNation()->m_destroyFigure(figureToRelease);
	}
	std::cout<<"size: "<<cityToCapture->m_size()<<std::endl;
	if(cityToCapture->m_size()!=0){
		m_cities.push_back(cityToCapture);
		cityToCapture->m_owningNation = shared_from_this();
	}
	for(std::vector<std::shared_ptr<City>>::iterator it = cityToCapture->m_OwningNation()->m_cities.begin(); it!= cityToCapture->m_OwningNation()->m_cities.end();it++){
		if(it->get()==cityToCapture.get()){
			cityToCapture->m_OwningNation()->m_cities.erase(it);
			break;
		}
	}
	std::cout<<"captured!"<<std::endl;
}

void Nation::m_establishEmbassy(std::shared_ptr<Nation> nationToTrack){
	m_embassies.push_back(Embassy(nationToTrack));
}

bool Nation::m_hasActiveWonder(ImprovementType imptype){
	return theGame->m_hasActiveWonder(m_nation, imptype);
}

void Nation::m_removeFigure(std::shared_ptr<Figure> figureToRemove){
	int previousListSize = m_figures.size();
	m_activeFigures.remove(figureToRemove);
	for(int i(0); i<previousListSize; i++){
		if(m_figures[i].get()==figureToRemove.get()){
			for(int j=i; j<previousListSize - 1; j++){
				m_figures[j] = m_figures[j+1];
			}
			break;
		}
	}
	m_figures.pop_back();
	std::cout<<"previousListSize (destroyFigure): "<<previousListSize<<", listSize: "<<m_figures.size()<<std::endl;
}

float Nation::m_scienceCoefficient(){
	float whatToReturn = 1;
	if(m_hasActiveWonder(SETI_PROGRAM)){
		whatToReturn+=0.5;
	}
	return whatToReturn;
}

float Nation::m_libraryCoefficient(){
	float whatToReturn = 0.5;
	if(m_hasActiveWonder(ISAAC_NEWTONS_COLLEGE)){
		whatToReturn+=(1/3);
	}
	return whatToReturn;
}

int Nation::m_pollutionCoefficient(){
	if(m_hasExplored(TECHNOLOGY_PLASTICS))
		return 4;
	if(m_hasExplored(TECHNOLOGY_MASS_PRODUCTION))
		return 3;
	if(m_hasExplored(TECHNOLOGY_AUTOMOBILE))
		return 2;
	if(m_hasExplored(Technology::TECHNOLOGY_INDUSTRIALIZATION))
		return 1;
	return 0;
}
