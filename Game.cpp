/*
 * Game.cpp
 *
 *  Created on: 27.03.2022
 *      Author: uwe-w
 */

#include "Game.hpp"
#include "FieldContainer.hpp"
#include "Settlers.hpp"
#include "Plane.hpp"
#include "Trireme.hpp"
#include <stdlib.h>
#include <random>
#include <iomanip>
#include <functional>
#include "City.hpp"
Nationality Graphics::Civ::currentNationality = ROMAN; bool gameReady = false;

std::shared_ptr<Figure> Game::m_getCurrentFigure(Nation* nation){
	std::cout<<"nation = "<<(nation ? nation : m_NationAtCurrentTurn().get())<<", settlersCount = "<<settlersCount<<std::endl;
	auto whatToReturn = nation ? nation->m_getCurrentFigure()
		: m_NationAtCurrentTurn()->m_getCurrentFigure();
	return whatToReturn;
}

Game::Game(std::vector<Nationality>& nationsToPlay){
	for(int i(WONDER_MIN); i<=WONDER_MAX;i++){
		m_hasWonderBeenBuilt.push_back(WonderData{(ImprovementType)i,NO_NATIONALITY,false});
	}
	bool firstNation = true;
	for(std::vector<std::shared_ptr<Field>>& currentMeridian: *theContainer->m_getFieldsOfTheWorld()){
		for(std::shared_ptr<Field> currentField: currentMeridian){
			currentField->m_initNationFogInfo(nationsToPlay);
		}
	}
	for(Nationality currentNationality: nationsToPlay){
		Graphics::Civ::currentNationality = currentNationality;
		std::shared_ptr<Nation> nationPointer =  std::make_shared<Nation>(currentNationality, "", PLAYER_PRINCE, firstNation);
		int randomIndex = m_getRandomNumberBetween(0, Science::possibleStartingTechs().size()-1);
		nationPointer->m_setWhatToExplore (Science::possibleStartingTechs()[randomIndex]);
		nationPointer->m_maybeFinishExploration();
		firstNation = false;
		m_nationsPlaying.push_back(nationPointer);
		Nation& nation = *nationPointer;
		Coordinate fieldCoordinate = Nation::getStandardCoordinateForNation(currentNationality);
		std::vector<Meridian>& fieldsOfTheWorld = *theContainer->m_getFieldsOfTheWorld();
		std::shared_ptr<Field> fieldPointer = fieldsOfTheWorld[fieldCoordinate.x][fieldCoordinate.y];
		for(int i(0); i<1; i++){
			std::shared_ptr<Settlers> theSettlersPointer =
			std::make_shared<Settlers>(fieldPointer, nationPointer);
			nation.m_addFigure(theSettlersPointer);
			fieldPointer->m_takeFigure(theSettlersPointer);
			std::shared_ptr<Trireme> theTriremePointer =
			std::make_shared<Trireme>(fieldPointer->m_getNeighbouringField(LEFT), nationPointer);
			nation.m_addFigure(theTriremePointer);
			fieldPointer->m_getNeighbouringField(LEFT)->m_takeFigure(theTriremePointer);
			std::stringstream stream; stream<<"City of "<<nation.m_Nation();stream.flush();
			std::cout<<"stream: "<<stream.str();
		}
		nationPointer->m_setMakingActive(false);
	}
	Graphics::Civ::currentNationality = nationsToPlay[0];
	gameReady = true;
}

Game::Year::Year(unsigned int yearNumberRaw):m_yearNumberRaw(yearNumberRaw)
{

}

//New turn
void Game::m_startNewTurn(){
	int temp = m_nationAtCurrentTurnIndex;
	try{
	for(std::shared_ptr<Figure> currentFigureToCheck: m_NationAtCurrentTurn()->m_Figures()){
		if(currentFigureToCheck->m_FigureState()==MOVING){
			std::cout<<"turnendstooearly by figurestate, nation at fault: "<<currentFigureToCheck->m_Nationality()<<std::endl;
			throw(TurnEndsTooEarly());
		}
	}
	if(m_NationAtCurrentTurn()->m_activeFiguresSize()!=0){
		std::cout<<"turnendstooearly by activefigures"<<std::endl;
		throw(TurnEndsTooEarly());
	}
	for(std::shared_ptr<Nation> currentNation: m_nationsPlaying){
		for(std::shared_ptr<Figure> currentFigure: currentNation->m_Figures()){
			currentFigure->m_hideFrom(m_NationAtCurrentTurn()->m_Nation());
		}
	}
	(++m_nationAtCurrentTurnIndex)%= (m_nationsPlaying.size());
	m_NationAtCurrentTurn()->m_startNewTurn();
	if(m_nationAtCurrentTurnIndex==0){
	m_handleGlobalWarming();
		std::cout<<"wieder bei null im Nationenindex angekommen"<<std::endl;
	m_currentYear = Year(m_currentYear.m_yearNumberRaw+1);
	}
	std::cout<<"New turn started for "<<m_NationAtCurrentTurn()->m_Nation()<<"!"<<std::endl;
	if(m_NationAtCurrentTurn()->m_activeFiguresSize()<=0){
		std::cout<<"No figure is active!"<<std::endl;
		m_startNewTurn();
	}
	}
	catch(TurnEndsTooEarly& theException){
		std::cout<<theException.what()<<std::endl;
		m_nationAtCurrentTurnIndex = temp;
		return;
	}
}

std::shared_ptr<Nation> Game::m_NationAtCurrentTurn(){
	return m_nationsPlaying[m_nationAtCurrentTurnIndex];
}

void Game::m_makeEndOfTurn(){
	m_startNewTurn();
}

Nationality Game::m_calculateWinnerInFight(std::shared_ptr<Figure> attacker, std::shared_ptr<Figure> defender){
	try{
	float attackingStrength = attacker->m_attackingStrength()*std::min((float)1, (float) attacker->m_MovementPoints().m_movementPoints/ONE_MOVEMENT_POINT);
	float defensiveStrength = defender->m_defensiveStrength();
	defensiveStrength *= defender->m_WhereItStands().m_defenseBonus();
	if(attacker->m_IsVeteran()){
		attackingStrength *= 1.5;
	}
	if(defender->m_IsVeteran()){
		defensiveStrength*=1.5;
	}
	if(defender->m_FigureState()==FORTIFIED || defender->m_FigureState() == COMPLETELY_FORTIFIED){
		defensiveStrength*=1.5;
	}
	attackingStrength *= 2^8;
	defensiveStrength *= 2^8; //für mehr Präzision, indem Überträge/Rundiúngen entschärft werden
	int randomNumber = m_getRandomNumberBetween(1, ((int) attackingStrength) + ((int) defensiveStrength));

	return randomNumber <= attackingStrength ? attacker->m_Nationality() : defender->m_Nationality();
	}
	catch(RandomFail& randomFail){
	return defender->m_Nationality();
	}
}

int Game::m_getRandomNumberBetween(int lowerBound, int upperBound){
	if(lowerBound > upperBound){
		throw(RandomFail(lowerBound, upperBound));
	}
	 std::uniform_int_distribution<int> distribution(lowerBound, upperBound);
	 int randomNumber = distribution(m_theRandomDevice);
	 return randomNumber;
}

std::string Game::Year::m_yearString(){
	std::stringstream s;
	if(m_yearNumberRaw == 200){
		s<<"1 AD";
		goto Return;
	}
	if(m_yearNumberRaw < 200){
		s<<std::setw(4)<<4000-20*m_yearNumberRaw<<" BC";
		goto Return;
	}
	if(m_yearNumberRaw<=250){
		s<<std::setw(4)<<-4000+20*m_yearNumberRaw<<" AD";
		goto Return;
	}
	if(m_yearNumberRaw<=300){
		s<<std::setw(4)<<1000+(m_yearNumberRaw-250)*10<<" AD";
		goto Return;
	}
	if(m_yearNumberRaw<=350){
		s<<1500+(m_yearNumberRaw-300)*5<<" AD";
		goto Return;
	}
	if(m_yearNumberRaw<=400){
		s<<1750+(m_yearNumberRaw-350)*2<<" AD";
		goto Return;
	}
	s<<1850+(m_yearNumberRaw-400)<<" AD";
	Return: s.flush();
	return s.str();
}

const WonderData& Game::m_wonderData(ImprovementType imptype){
	return m_hasWonderBeenBuilt[imptype-WONDER_MIN];
}

bool Game::m_isObsolete(ImprovementType imptype){
	TechnologyDependency info = Science::techInfo(imptype);
	for(std::shared_ptr<Nation> nation: m_nationsPlaying){
		for(Technology currentTech: nation->m_ExploredTechnologies()){
			if(currentTech == info.techMakingObsolete1 || currentTech == info.techMakingObsolete2){
				return true;
			}
		}
	}
	return false;
}

std::shared_ptr<Nation> Game::m_getNation(Nationality nationality){
	std::shared_ptr<Nation> whatToReturn = nullptr;
	for(std::shared_ptr<Nation> nation: m_nationsPlaying){
		if(nation->m_Nation()==nationality){
			whatToReturn = nation;
			break;
		}
	}
	return whatToReturn;
}

void Game::m_acknowledgeExploration(Technology tech, Nationality nationality){

	std::vector<ImprovementType> imptypesMadeObsolete;
	for(ImprovementType comparedImptype = BUILDING_MIN;(int)comparedImptype<=BUILDING_MAX; comparedImptype = (ImprovementType) ((int) comparedImptype + 1)){
		if(tech == Science::techInfo(comparedImptype).techMakingObsolete1 || tech == Science::techInfo(comparedImptype).techMakingObsolete2){
			imptypesMadeObsolete.push_back(comparedImptype);
		}
	}
	std::vector<ImprovementType> wonderTypesMadeObsolete;
	for(ImprovementType comparedImptype = WONDER_MIN;(int)comparedImptype<=WONDER_MAX; comparedImptype = (ImprovementType) ((int) comparedImptype + 1)){
		if(tech == Science::techInfo(comparedImptype).techMakingObsolete1 || tech == Science::techInfo(comparedImptype).techMakingObsolete2){
			wonderTypesMadeObsolete.push_back(comparedImptype);
		}
	}
	m_handleObsoletion(imptypesMadeObsolete, wonderTypesMadeObsolete);

	if(m_isObsolete(GREAT_LIBRARY)){
		return;
	}
	Nationality greatLibraryOwner = NO_NATIONALITY;
	if(m_wonderData(GREAT_LIBRARY).who != NO_NATIONALITY && m_wonderData(GREAT_LIBRARY).isDestroyed == false){
		greatLibraryOwner = m_wonderData(GREAT_LIBRARY).who;
		Nation& currentNation = *m_getNation(greatLibraryOwner);
		if(currentNation.m_hasExplored(tech)){
			return;
		}
		for(std::shared_ptr<Nation> comparedNation: m_nationsPlaying){
			if(comparedNation->m_Nation()!=greatLibraryOwner && comparedNation->m_Nation()!=nationality){
				if(comparedNation->m_hasExplored(tech)){
					currentNation.m_addTechnology(tech);
				}
			}
		}
	}
}

void Game::m_handleObsoletion(std::vector<ImprovementType>& imptypes, std::vector<ImprovementType>& wonderTypes){
	for(ImprovementType currentImptype: imptypes){
		for(std::shared_ptr<Nation> currentNation: m_nationsPlaying){
			for(std::shared_ptr<City> currentCity: currentNation->m_Cities()){
				for(const CityImprovement& currentImprovement: currentCity->m_Improvements()){
					if(currentImprovement.m_what == currentImptype){
						currentImprovement.whenDestroyed(currentCity.get());
					}
				}
		}
	}
	}
	for(ImprovementType currentImptype: wonderTypes){
		for(std::shared_ptr<Nation> currentNation: m_nationsPlaying){
			for(std::shared_ptr<City> currentCity: currentNation->m_Cities()){
				for(const CityImprovement& currentImprovement: currentCity->m_Improvements()){
					if(currentImprovement.m_what == currentImptype){
						currentImprovement.whenDestroyed(currentCity.get());
					}
				}
			}
		}
	}
}

bool Game::m_hasActiveWonder(Nationality nationality, ImprovementType imptype){
	if(!City::isWonderType(imptype))
		throw imptype;
	const WonderData& data = m_wonderData(imptype);
	if(data.who == nationality && !data.isDestroyed && m_isObsolete(imptype)){
		return true;
	}
	return false;
}

bool Game::m_isWonderOnContinent(City& city, ImprovementType imptype){
	if(!m_hasActiveWonder(city.m_OwningNation()->m_Nation(),imptype)){
		return false;
	}
	return m_wonderData(imptype).city->m_WhereItStands()->m_ContinentID()==city.m_WhereItStands()->m_ContinentID();
}

void Game::m_handleGlobalWarming(){
	if(m_pollutionCount==0)
		return;
	m_climateState = (ClimateState) (m_climateState + 1);
	if(m_climateState<=RED_CLIMATE_ALARM){
		return;
	}
	m_climateState = NO_CLIMATE_ALARM;
	int notDesertificated = GlobalWarming::DESERTS_TO_MAKE;
	GlobalWarming::DESERTS_TO_MAKE+=GlobalWarming::WARMING_EXACERBATION;
	int notSwamped = GlobalWarming::SWAMPS_TO_MAKE;
	GlobalWarming::SWAMPS_TO_MAKE+=GlobalWarming::WARMING_EXACERBATION;
	auto& fields = *theContainer->m_getFieldsOfTheWorld();
	int triesLeftForDesertification = 50;
while1:	while(notSwamped>0||notDesertificated>0){
		int randomNumberY = m_getRandomNumberBetween(-GlobalWarming::DESERTIFICATION_BOUND,GlobalWarming::DESERTIFICATION_BOUND);
		int randomNumberX = m_getRandomNumberBetween(0,WORLD_LENGTH-1);
		while(triesLeftForDesertification>0){
			Field* field = ((fields[randomNumberX])[randomNumberY+WORLD_HEIGHT/2]).get();
			switch(field->m_Landscape()){
			case PLAINS: {
				if(notDesertificated<=0)
				goto normalcase;
				field->m_changeLandscapeTo(DESERT);
				notDesertificated--;
				field->m_TurnsUntilSwamped()=10;
				/*std::stringstream s;
				SDL_RenderClear(theRenderer);
				s<<"Now it happened on field "<<randomNumberX<<", "<<randomNumberY<<std::endl;
				Miscellaneous::printMultipleLines(s.str(), 0, 0, Graphics::redColor());
				SDL_RenderPresent(theRenderer);
				SDL_Delay(900);*/
				goto while1;
			}
			//universelle Hashfunktion auf Basis der Rundenzahl wäre eine bessere Idee
			//generell, um die problematischen Zufallszahlen zu lösen
			case GRASSLAND:
			{
				if(notDesertificated<=0)
					goto normalcase;
				field->m_changeLandscapeTo(PLAINS);
				notDesertificated--;
				field->m_TurnsUntilSwamped()=10;
				/*std::stringstream s;
				SDL_RenderClear(theRenderer);
				s<<"Now it happened on field "<<randomNumberX<<", "<<randomNumberY<<std::endl;
				Miscellaneous::printMultipleLines(s.str(), 0, 0, Graphics::redColor());
				SDL_RenderPresent(theRenderer);
				SDL_Delay(900);*/
				goto while1;
			}
			case MOUNTAIN: break;
			case HILLS: break;
			case OCEAN: break;
			case ARCTIC: break;
			case TUNDRA: break;
			case SWAMP: break;
			default:
			{normalcase:
				if(field->m_closeToLandscape(OCEAN)&&field->m_TurnsUntilSwamped()==0){
					field->m_changeLandscapeTo(SWAMP);
					notSwamped--;
					/*std::stringstream s;
									SDL_RenderClear(theRenderer);
									s<<"Now it happened on field "<<randomNumberX<<", "<<randomNumberY<<std::endl;
									Miscellaneous::printMultipleLines(s.str(), 0, 0, Graphics::redColor());
									SDL_RenderPresent(theRenderer);
									SDL_Delay(9000);*/
					goto while1;
				}
				break;
			}
			}
		triesLeftForDesertification--;
			randomNumberX = (randomNumberX+GlobalWarming::HORIZONTAL_HOPPING_DISTANCE)%WORLD_LENGTH;
		}
	}
	GlobalWarming::DESERTIFICATION_BOUND = std::min(GlobalWarming::DESERTIFICATION_BOUND + GlobalWarming::DESERTIFICATION_SPREADER, WORLD_HEIGHT/2 - 5);
}
