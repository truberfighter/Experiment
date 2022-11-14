/*
 * Nation.hpp
 *
 *  Created on: 09.03.2022
 *      Author: uwe-w
 */

#ifndef NATION_HPP_
#define NATION_HPP_

#include "Settlers.hpp"
#include "Technologytypes.hpp"
#include <queue>

class City;
class Embassy;
class NegativeTreasury{
public:
	int previousTreasury, treasuryNow;
	NegativeTreasury(int a, int b):previousTreasury(a), treasuryNow(b){}
};

class Nation: public std::enable_shared_from_this<Nation>{
private:
	std::shared_ptr<City> m_capitalCity;
	std::vector<std::shared_ptr<City>> m_cities;
	std::vector<std::shared_ptr<City>> m_foundedCities;
	GovernmentType m_government = DESPOTISM;
	Nationality m_nation;
	std::string m_leaderName;
	std::vector<std::shared_ptr<Figure>> m_figures;
	std::list<std::shared_ptr<Figure>> m_activeFigures;
	bool m_directlyMakingFiguresActive = false;
	short unsigned int m_taxRate = 5;
	short unsigned int m_luxuriesRate = 0;
	int m_treasury = 5000;
	std::vector<Technology> m_exploredTechnologies;
	Technology m_whatToExplore = NO_TECHNOLOGY;
	int m_explorationProgress = 0;
	Difficulty m_difficulty;
	std::vector<Embassy> m_embassies;
public:
	std::vector<std::shared_ptr<City>> m_FoundedCities(){return m_foundedCities;}
	void m_giveCity(std::shared_ptr<City> cityToGive,std::shared_ptr<Nation> whoToGiveTo);
	void m_destroyCity(std::shared_ptr<City> cityToDestroy);
	void m_addProgress(int progress);
	void m_setWhatToExplore(Technology tech){m_whatToExplore = tech;}
	bool m_hasExplored(Technology tech);
	void m_maybeFinishExploration();
	int m_howMuchNeededForExploration();
	Technology m_askForNewExploration();
	std::vector<Technology> m_technologiesAvailable();
	void m_receiveMoney(int amount);
	short unsigned int m_TaxRate();
	void m_setTaxRate(short unsigned int tax);
	short unsigned int m_LuxuriesRate();
	short unsigned int m_ScienceRate(){return TAX_RATE_STEP_COUNT - m_taxRate - m_luxuriesRate;}
	void m_addTechnology(Technology tech);
	bool m_alterTaxRate();
	bool m_alterLuxuriesRate();
	void m_setLuxuriesRate(short unsigned int luxuries);
	void m_setMakingActive(bool setting){m_directlyMakingFiguresActive = setting;}
	std::shared_ptr<City> m_CapitalCity(){return m_capitalCity;}
	void m_setCapitalCity(std::shared_ptr<City> city);
	std::vector<std::shared_ptr<City>>& m_Cities(){return m_cities;}
	static Coordinate getStandardCoordinateForNation(Nationality n);
	Nation(Nationality n = ROMAN, std::string leaderName = "", Difficulty difficulty = PLAYER_PRINCE, bool directlyMakingFiguresActive = false);
	const std::string& m_LeaderName();
	~Nation();
	GovernmentType m_Government();
	Nationality m_Nation() const;
	bool m_canBuildRailroad();
	bool m_canBuildBridges();
	std::vector<std::shared_ptr<Figure>>& m_Figures();
	void m_addFigure(std::shared_ptr<Figure> newFigure);
	bool m_destroyFigure(std::shared_ptr<Figure>);
	std::shared_ptr<Figure> m_getCurrentFigure();
	void m_makeFigureWait();
	bool m_removeFromQueue(std::shared_ptr<Figure> figureToRemove);
	bool m_addToQueue(std::shared_ptr<Figure> figureToAdd);
	void m_startNewTurn();
	int m_activeFiguresSize(){return m_activeFigures.size();}
	int m_queueSize(){return m_activeFigures.size();}
	std::string m_colorString();
	int m_Treasury(){return m_treasury;}
	friend class Figurebutton;
	static SDL_Color standardNationColor(Nationality nationalityToDraw);
	void m_captureCity(std::shared_ptr<City> cityToCapture);
	void m_establishEmbassy(std::shared_ptr<Nation> nationToTrack);
	void m_removeFigure(std::shared_ptr<Figure> figureToRemove);
	friend class Embassy;
};

inline GovernmentType Nation::m_Government(){
	return m_government;
}

inline 	Nationality Nation::m_Nation() const{
	return m_nation;
}

inline std::vector<std::shared_ptr<Figure>>& Nation::m_Figures(){
	return m_figures;
}

class TechnologyOvershot{
public:
	int whatNeeded;
	int whatsThere;
	TechnologyOvershot(int wn, int wt):whatNeeded(wn), whatsThere(wt){}
};

class MoveStartFail{
public:
	std::string m_figureType = "";
	MoveStartFail(std::string figureType):m_figureType(figureType){}
	MoveStartFail(int figureType){m_figureType+=figureType;}
	std::string what(){return "MoveStartFail for FigureType: " + m_figureType + "\n";}
};


#endif //NATION_HPP_
