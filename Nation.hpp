/*
 * Nation.hpp
 *
 *  Created on: 09.03.2022
 *      Author: uwe-w
 */

#ifndef NATION_HPP_
#define NATION_HPP_

#include "Settlers.hpp"
#include <queue>

class Nation: public std::enable_shared_from_this<Nation>{
private:
	GovernmentType m_government = DESPOTISM;
	Nationality m_nation;
	std::string m_leaderName;
	std::vector<std::shared_ptr<Figure>> m_figures;
	std::list<std::shared_ptr<Figure>> m_activeFigures;
	bool m_directlyMakingFiguresActive = true;
public:
	~Nation(){std::cout<<"Nationdestruktor"<<this<<std::endl;}
	static Coordinate getStandardCoordinateForNation(Nationality n);
	Nation(Nationality n = ROMAN, std::string leaderName = "");
	const std::string& m_LeaderName();
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

class MoveStartFail{
public:
	std::string m_figureType = "";
	MoveStartFail(std::string figureType):m_figureType(figureType){}
	MoveStartFail(int figureType){m_figureType+=figureType;}
	std::string what(){return "MoveStartFail for FigureType: " + m_figureType + "\n";}
};


#endif //NATION_HPP_
