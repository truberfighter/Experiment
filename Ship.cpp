#include "Ship.hpp"
#include "Game.hpp"
#include "sdltypes.hpp"
#include "Field.hpp"
#include "EventHandler.hpp"
#include "City.hpp"

FigureCategory Ship::m_FigureCategory(){
	return SEA;
}

std::string Ship::m_orderOverview(){return "H: HomeCity, I: Irrigate, S: Sentry, F: Fortify (possibly)";}

Ship::Ship(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)
: Figure(whereToStart, nationality, home, isVeteran)
{
}

bool Ship::m_takeOrder(char order){
	switch(order){
	case 'u': {
		bool whatToReturn = false;
		for(std::shared_ptr<Figure> currentFigure: m_getCargo()){
			if(currentFigure->m_FigureState()==SENTRIED){
				m_nationality->m_addToQueue(currentFigure);
				currentFigure->m_startMove(true);
				whatToReturn = true;
			}
		}
		return whatToReturn;
	}
	case 's':{
		m_sentry();
		return true;
	}
	case 'f':{
		if(m_whereItStands->m_CityContained()){
			m_fortify();
			return true;
		}
		return false;
	}
	case ' ': {
		m_finishMove();
		return true;
	}
	case 'h': return m_homeCity();
	default: return false;
}
}

Ship::~Ship(){
	bool lol = true;
	std::cout<<"Ship-Destruktor Beginn"<<std::endl;
	while(true){
		whilebegin: short int cargoCapability = 0;
		for(std::shared_ptr<Figure> currentFigure:
				m_whereItStands->m_FiguresOnField()){
			if(currentFigure->m_FigureCategory() == SEA && (m_isCarrier ?
			currentFigure->m_FigureType()!=CARRIER : currentFigure->m_FigureType()==CARRIER)){
				cargoCapability+= (short int) reinterpret_cast<Ship*>(currentFigure.get())->m_cargoCountMax();
			}
			if(currentFigure->m_FigureCategory()== (m_isCarrier ? GROUND : FLIGHT)){
				continue;
			}
			if(lol)
			if(currentFigure->m_FigureCategory()== (m_isCarrier ? FLIGHT : GROUND)){
				cargoCapability--;
			}
			std::cout<<"cargoCapability: "<<cargoCapability<<std::endl;
		}
		if(cargoCapability >= 0){
			lol = false;
		std::cout<<"lol"<<std::endl;
			goto destructorend;
		}
		else{
			for(std::shared_ptr<Figure> currentFigure: m_whereItStands->m_FiguresOnField()){
				if(currentFigure->m_FigureCategory()==GROUND){
					lol = true;
					std::cout<<"deleting begun of "<<currentFigure->m_FigureType()<<std::endl;
					currentFigure->m_Nation()->m_destroyFigure(currentFigure);
					std::cout<<"deleting complete of "<<currentFigure->m_FigureType()<<std::endl;
					goto whilebegin;
				}
			}
		}
	}
		destructorend: std::cout<<"Ship-Destruktor Ende"<<std::endl;
}

void Ship::m_finishMove(){
	std::cout<<"m_finishMove(), x = "<<m_image->getPosition().x<<", y = "<<m_image->getPosition().y<<std::endl;
	m_figureState = m_figureState == SENTRYING ? SENTRIED : (m_figureState == FORTIFIED ? COMPLETELY_FORTIFIED : DONE_WITH_TURN);
	m_nationality->m_removeFromQueue(shared_from_this());
	if(m_FigureType()==TRIREME){
		Direction directions[] = {DOWN_LEFT, DOWN, DOWN_RIGHT, LEFT,RIGHT, UP_LEFT, UP, UP_RIGHT};
		for(int i(0); i<8;i++){
			if(m_whereItStands->m_getNeighbouringField(directions[i])->m_Landscape()!=OCEAN){
				return;
			}
		}
		bool lostAtSea = theGame->m_getRandomNumberBetween(0, 1) == 0;
		if(lostAtSea){
			m_nationality->m_destroyFigure(shared_from_this());

		}
	}
	return;
}

std::vector<std::shared_ptr<Figure>> Ship::m_getCargo(){
	std::vector<std::shared_ptr<Figure>> whatToReturn;
	int howMuch(0);
	for(std::shared_ptr<Figure> currentFigure: m_whereItStands->m_FiguresOnField()){
		if(currentFigure->m_FigureCategory()!=SEA){
			if(howMuch>=m_cargoCountMax()){
				break;
			}
			//Does the figurecategory fit?
			if(currentFigure->m_FigureCategory() == (m_FigureType() == CARRIER) ? FLIGHT : GROUND && currentFigure->m_FigureState() == SENTRIED)
				whatToReturn.push_back(currentFigure);
		}
	}
	return whatToReturn;

}

