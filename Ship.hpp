#ifndef SHIP_HPP_
#define SHIP_HPP_

#include "sdltypes.hpp"
#include "Figure.hpp"

class Nation;
class City;

class Ship: public Figure{
protected:
	bool m_isCarrier = false;
	bool m_mayBombardGroundTroops = true;
public:
	virtual FigureType m_FigureType() override = 0;
	Ship(){}
	Ship(Field* whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home = nullptr, bool isVeteran = false);
	virtual ~Ship();
	bool m_takeOrder(char order);
	virtual std::string m_orderOverview();
	virtual float m_attackingStrength() = 0;
	virtual float m_defensiveStrength() = 0;
	virtual short unsigned int m_visibilityRange() = 0;
	void m_finishMove() override;
	virtual short unsigned int m_defaultMovementPoints() = 0;
	FigureCategory m_FigureCategory()override;
	std::shared_ptr<MovableThing> m_createImage() override = 0;
	virtual short unsigned int m_cargoCountMax() = 0;
	std::vector<std::shared_ptr<Figure>> m_getCargo();
	bool m_MayBombardGroundTroops(){return m_mayBombardGroundTroops;}
	void m_printData(std::ostream& os);
	friend class FigureFactory;
};



#endif /* SHIP_HPP_ */
