#include "Trireme.hpp"
#include "MovableThing.hpp"
#include "Nation.hpp"
#include "Field.hpp"

using namespace std;

VISIBILITY_OF Trireme IS_NORMAL_VISIBILITY
STRENGTH_OF Trireme WHEN_ATTACKING(1)
STRENGTH_OF Trireme WHEN_DEFENDING(WEAK)
DEFAULT_MOVING_POINTS(Trireme, 3)
CARGO_COUNT_MAX(Trireme, 2)

std::shared_ptr<MovableThing> Trireme::m_createImage(){
	int x = m_whereItStands->m_X(), y  = m_whereItStands->m_Y();
	std::string imageString = "bilder/Figures/Trireme/Trireme";
	imageString += m_nationality->m_colorString();
	//ändere das mit dem filename
	m_image = std::make_shared<MovableThing>(theRenderer, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE, imageString.c_str(), x, y);
	return m_image;
}

FigureType Trireme::m_FigureType(){return TRIREME;}

Trireme::~Trireme(){std::cout<<"Trireme-Destruktor, this = "<<this<<std::endl;}

Trireme::Trireme(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality, std::shared_ptr<City> home, bool isVeteran)
:Ship(whereToStart, nationality, home, isVeteran)
{
	m_resetMovementPoints();
	if(!m_initImage()) cout<<"Fatal error: MovableThing for Trireme not created"<<endl;
	cout<<"Triremekonstruktor"<<theRenderer<<endl;
}


