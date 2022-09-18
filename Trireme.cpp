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
Carrier::Carrier(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)
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

