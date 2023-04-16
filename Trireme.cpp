#include "Trireme.hpp"
#include "MovableThing.hpp"
#include "Nation.hpp"
#include "Field.hpp"
#include "City.hpp"

using namespace std;

DECONSTRUCTOR_SHIP(Trireme)
VISIBILITY_OF Trireme IS_NORMAL_VISIBILITY
STRENGTH_OF Trireme WHEN_ATTACKING(1)
STRENGTH_OF Trireme WHEN_DEFENDING(WEAK)
DEFAULT_MOVING_POINTS(Trireme, 3)
CARGO_COUNT_MAX(Trireme, 2)
SHIELD_COST(Trireme, 40)
CREATE_IMAGE(Trireme)
FIGURE_TYPE(Trireme,TRIREME)

Carrier::~Carrier(){std::cout<<m_FigureType()<<"-Destruktor, this = "<<this<<std::endl;}\
\
Carrier::Carrier(Field* whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)
:Ship(whereToStart, nationality, home, isVeteran)
{
	m_isCarrier = true;
	m_resetMovementPoints();
	if(!m_initImage()) cout<<"Fatal error: MovableThing for "<<m_FigureType()<<" not created"<<endl;
	cout<<m_FigureType()<<"konstruktor"<<this<<endl;
}
VISIBILITY(Carrier, 2)
STRENGTH_OF Carrier WHEN_ATTACKING(1)
STRENGTH_OF Carrier WHEN_DEFENDING(12)
DEFAULT_MOVING_POINTS(Carrier,5)
CARGO_COUNT_MAX(Carrier,8)
SHIELD_COST(Carrier,160)
CREATE_IMAGE(Carrier)
FIGURE_TYPE(Carrier,CARRIER)

Submarine::~Submarine(){std::cout<<m_FigureType()<<"-Destruktor, this = "<<this<<std::endl;}\
 Submarine::Submarine(Field* whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)\
:Ship(whereToStart, nationality, home, isVeteran)\
{
	m_mayBombardGroundTroops = false;
	m_resetMovementPoints();
	if(!m_initImage()) cout<<"Fatal error: MovableThing for "<<m_FigureType()<<" not created"<<endl;
	cout<<m_FigureType()<<"konstruktor"<<this<<endl;
}
VISIBILITY(Submarine, 2)
STRENGTH_OF Submarine WHEN_ATTACKING(8)
float Submarine ::m_defensiveStrength() {
	return 2;
}
DEFAULT_MOVING_POINTS(Submarine,3)
CARGO_COUNT_MAX(Submarine,0)
SHIELD_COST(Submarine,50)
CREATE_IMAGE(Submarine)
FIGURE_TYPE(Submarine,SUBMARINE)
bool Submarine::m_canBeSeenBy(Figure* figureLooking){
	return figureLooking->m_FigureCategory()==SEA && figureLooking->m_visibilityRange()>= 1 && figureLooking->m_WhereItStands().m_maxNormDistanceTo(m_whereItStands)<=1 && m_figureState!=MOVING;
}
DECONSTRUCTOR_SHIP(Transport)
VISIBILITY_OF Transport IS_NORMAL_VISIBILITY
STRENGTH_OF Transport WHEN_ATTACKING(WEAK)
STRENGTH_OF Transport WHEN_DEFENDING(3)
DEFAULT_MOVING_POINTS(Transport, 4)
CARGO_COUNT_MAX(Transport, 8)
SHIELD_COST(Transport, 50)
CREATE_IMAGE(Transport)
FIGURE_TYPE(Transport,TRANSPORT)

DECONSTRUCTOR_SHIP(Ironclad)
VISIBILITY(Ironclad,1)
STRENGTH_OF Ironclad WHEN_ATTACKING(4)
STRENGTH_OF Ironclad WHEN_DEFENDING(4)
DEFAULT_MOVING_POINTS(Ironclad, 4)
CARGO_COUNT_MAX(Ironclad, 0)
SHIELD_COST(Ironclad, 60)
CREATE_IMAGE(Ironclad)
FIGURE_TYPE(Ironclad,IRONCLAD)

DECONSTRUCTOR_SHIP(Cruiser)
VISIBILITY(Cruiser,2)
STRENGTH_OF Cruiser WHEN_ATTACKING(6)
STRENGTH_OF Cruiser WHEN_DEFENDING(6)
DEFAULT_MOVING_POINTS(Cruiser, 6)
CARGO_COUNT_MAX(Cruiser, 0)
SHIELD_COST(Cruiser, 80)
CREATE_IMAGE(Cruiser)
FIGURE_TYPE(Cruiser,CRUISER)

DECONSTRUCTOR_SHIP(Frigate)
VISIBILITY(Frigate,1)
STRENGTH_OF Frigate WHEN_ATTACKING(2)
STRENGTH_OF Frigate WHEN_DEFENDING(2)
DEFAULT_MOVING_POINTS(Frigate, 3)
CARGO_COUNT_MAX(Frigate, 4)
SHIELD_COST(Frigate, 40)
CREATE_IMAGE(Frigate)
FIGURE_TYPE(Frigate,FRIGATE)

DECONSTRUCTOR_SHIP(Sail)
VISIBILITY(Sail,1)
STRENGTH_OF Sail WHEN_ATTACKING(1)
STRENGTH_OF Sail WHEN_DEFENDING(1)
DEFAULT_MOVING_POINTS(Sail, 3)
CARGO_COUNT_MAX(Sail, 3)
SHIELD_COST(Sail, 40)
CREATE_IMAGE(Sail)
FIGURE_TYPE(Sail,SAIL)

DECONSTRUCTOR_SHIP(Battleship)
VISIBILITY(Battleship,2)
STRENGTH_OF Battleship WHEN_ATTACKING(18)
STRENGTH_OF Battleship WHEN_DEFENDING(12)
DEFAULT_MOVING_POINTS(Battleship, 4)
CARGO_COUNT_MAX(Battleship, 0)
SHIELD_COST(Battleship, 160)
CREATE_IMAGE(Battleship)
FIGURE_TYPE(Battleship,BATTLESHIP)

