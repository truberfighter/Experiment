/*
 * Field.hpp
 *
 *  Created on: 04.03.2022
 *      Author: uwe-w
 */

#ifndef FIELD_HPP_
#define FIELD_HPP_

#include "sdltypes.hpp"
#include "Nation.hpp"
#include "LetterTextureContainer.hpp"
#include "Drawing.hpp"
#include <String>

struct NationKnows{
	bool value = false;
	Nationality nationality;
};
class City;
class Settlers;
class Citizen;
class Figure;
class FieldContainer;

class FieldElement: public ImmovableDrawingElement{
public:
	FieldElement (SDL_Renderer* renderer, std::shared_ptr<Texture> texture, int row = 0, int column = 0, Layer layer = STANDARD_FIELD_LAYER);
	virtual int m_draw(int rowShift = 0, int columnShift = 0, SDL_Renderer* renderer = nullptr) override;
};

class Field: public std::enable_shared_from_this<Field>{
protected:
	void m_makeVisibleAround(int visibilityRange = 1);
	std::vector<NationKnows> m_nationFogInfo;
	Citizen* m_citizenWorking = nullptr;
	std::list<std::shared_ptr<Figure>> m_figuresOnField;
	bool m_createRoadImage(SDL_Color& color);
	ContinentID m_continentID = NO_CONTINENT_ID_GIVEN;
	int m_roadTradeResult();
	bool m_hasFortress = false;
	bool m_IrrigateTemplate(SettlersWork whatWorkWillCome, Settlers& settlers);
	bool m_MiningTemplate(SettlersWork whatWorkWillCome, Settlers& settlers);
	Layer m_layer;
	bool m_hasSpecialResource = false;
	std::shared_ptr<FieldElement> m_drawingElement;
	int m_x;
	int m_y;
	bool m_isIrrigated = false;
	std::shared_ptr<City> m_cityContained;
	bool m_isMined = false;
	bool m_maybeFinishWork(Settlers& settlers, SettlersWork work);
	virtual short int m_howLongToTake(SettlersWork work) = 0;
	RoadStatus m_roadStatus = NOTHING;
	bool m_road(Settlers& settlers);
	void m_railRoadProductionEffect(int& count);
public:
	int m_distanceTo(std::shared_ptr<City>);
	bool m_isVisible(Nationality nationality);
	void m_makeVisible(Nationality nationality);
	void m_initNationFogInfo(std::vector<Nationality>& nationalities);
	bool m_irrigationBonus();
	static std::vector<Coordinate> coordinatesAroundCity();
	Citizen* m_CitizenWorking(){return m_citizenWorking;}
	bool m_setCitizenWorking(Citizen* citizen){if(m_cityContained!=nullptr) return false; m_citizenWorking = citizen; return true;}
	short int m_getCargoCapability(Figure& figureToEnter);
	bool m_HasFortress(){return m_hasFortress;}
	const std::list<std::shared_ptr<Figure>>& m_FiguresOnField(){return m_figuresOnField;}
	virtual ~Field();
	Field(int x, int y, Layer layer, bool hasSpecialResource = false);
	int m_X() const;
	int m_Y() const;
	std::shared_ptr<FieldElement> m_DrawingElement();
	void m_takeFigure(std::shared_ptr<Figure> movingFigure);
	void m_releaseFigure(std::shared_ptr<Figure> movingFigure);
	virtual MovementPoints m_movementPoints() =0;
	virtual float m_defenseBonus()=0;
	bool m_IsMined() const;
	bool m_IsIrrigated() const;
	bool m_Road(Settlers& settlers);
	virtual bool m_Mining(Settlers& settlers)=0;
	virtual bool m_Irrigate(Settlers& settlers)=0;
	int m_shields(Nation& nation);
	virtual int m_shields() = 0;
	int m_food(Nation& nation);
	virtual int m_food()=0;
	int m_trade(Nation& nation);
	virtual int m_trade()=0;
	bool m_militaryProblem(std::shared_ptr<Figure> movingFigure);
	virtual std::string m_resourceOverview()=0;
	virtual Landscape m_Landscape() const = 0;
	RoadStatus m_RoadStatus();
	bool m_HasSpecialResource();
	bool m_Pillage();
	std::shared_ptr<City> m_CityContained();
	std::shared_ptr<Field> m_getNeighbouringField(Direction whereToLook);
	std::shared_ptr<Field> m_getNeighbouringField(Coordinate differenceCoordinate);
	void m_drawField();
	bool m_closeToLandscape(Landscape ls);
	std::vector<std::shared_ptr<Field>> m_cityFieldsAround();
	friend std::ostream& operator<<(std::ostream&, Field&);
	friend class FieldContainer;
	friend class City;
	friend class Game;
	friend class Settlers;
};
std::ostream& operator<<(std::ostream& os, Landscape ls);
std::ostream& operator<<(std::ostream&, Field&);
#define ACKNOWLEDGE_RAILROAD m_railRoadProductionEffect(count);
#define ACKNOWLEDGE_DEMOCRACY if(count>0 && (nation.m_Government()==REPUBLIC || nation.m_Government()==DEMOCRACY))count++;
#define ACKNOWLEDGE_DESPOTISM if(count>2 && (nation.m_Government()==ANARCHY || nation.m_Government()==DESPOTISM))count--;
#define IS_CLASSICALLY_ROAD_BASED 	{return m_roadTradeResult();}
#define LANDMASS_SEPARATOR OCEAN
extern FieldContainer* theContainer;

#endif /* FIELD_HPP_ */
