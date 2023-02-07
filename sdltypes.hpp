#ifndef SDLTYPES_HPP
#define SDLTYPES_HPP

#include "SDL2\SDL_ttf.h"
#include <SDL2\SDL.h>
#include <iostream>
#include <SDL2\SDL_image.h>
#include <memory>
#include <vector>
#include <string>
#include <sstream>
#include <functional>
#define KeyCode unsigned int
class Field;
class MovableThing;
class Figure;
#define Meridian std::vector<std::shared_ptr<Field>>
enum PoleHit{NORTH_POLE_HIT, SOUTHERN_POLE_HIT};
enum{SDLK_1_DOWN_LEFT = 1073741913, SDLK_2_DOWN = SDLK_1_DOWN_LEFT + 1, SDLK_3_DOWN_RIGHT = SDLK_2_DOWN+1,
	SDLK_4_LEFT = SDLK_3_DOWN_RIGHT + 1, SDLK_5_CENTER = SDLK_4_LEFT + 1, SDLK_6_RIGHT = SDLK_5_CENTER+1,
	SDLK_7_UP_LEFT = SDLK_6_RIGHT + 1, SDLK_8_UP = SDLK_7_UP_LEFT + 1, SDLK_9_UP_RIGHT = SDLK_8_UP+1,
	SDLK_ENTER_KEY = 13,
	SDLK_TAXRATE_KEY = 1073741911, SDLK_LUXURIESRATE_KEY = 1073741910
};
enum Difficulty{PLAYER_CHIEFTAIN,PLAYER_WARLORD,PLAYER_PRINCE,PLAYER_KING,PLAYER_EMPEROR,KI_CHIEFTAIN,KI_WARLORD,KI_PRINCE,KI_KING,KI_EMPEROR};
enum{NORMAL_FONT_HEIGHT = 22};
enum{STANDARD_DRAWING_TIME = 200, STANDARD_FIELD_SIZE = 60};
enum CitizenState{HAPPY=13,CONTENT=26,UNHAPPY=39,TAX_COLLECTOR=65,ENTERTAINER=52,SCIENTIST=78};
typedef int Layer; extern Layer CITY_LAYER, STANDARD_LAYER, STANDARD_FIELD_LAYER, STANDARD_FIELD_MODIFICATOR_LAYER, SIDETEXT_LAYER;
enum FigureCategory{GROUND, SEA, FLIGHT};
enum FigureType{SETTLERS, MILITIA, TRANSPORT, SAIL, CARRIER, TRIREME, DIPLOMAT, CARAVAN, ARTILLERY, FIGHTER, CAVALRY, BOMBER,LEGION,IRONCLAD,FRIGATE,NUCLEAR,CRUISER,BATTLESHIP,SUBMARINE,PHALANX,RIFLEMEN,MUSKETEERS,KNIGHTS,CATAPULT,MECH_INF,ARMOR,CANNON,CHARIOT};
enum FightResult{ATTACKER_LOSES, DEFENDER_LOSES, KAMIKAZE};
enum Drawing_Element{BUTTON_ELEMENT, MOVABLE_DRAWING_ELEMENT, DRAWING, IMMOVABLE_DRAWING_ELEMENT,LAMBDA_DRAWING_ELEMENT};
enum Direction{DOWN_LEFT = 1, DOWN = 2, DOWN_RIGHT = 3, LEFT = 4, STANDING_STILL = 5,
	RIGHT = 6, UP_LEFT = 7, UP = 8, UP_RIGHT = 9};
enum{WORLD_LENGTH = 80, WORLD_HEIGHT = 80, SCREEN_WIDTH_NORMED = 20, SCREEN_HEIGHT_NORMED = 12,
SCREEN_WIDTH = SCREEN_WIDTH_NORMED*STANDARD_FIELD_SIZE, SCREEN_HEIGHT = SCREEN_HEIGHT_NORMED*STANDARD_FIELD_SIZE,
MODULO_WIDTH_FOR_NEGATIVE = 2, MODULO_HEIGHT_FOR_NEGATIVE = MODULO_WIDTH_FOR_NEGATIVE,
FIGURESTATE_TEXTURE_HEIGHT = STANDARD_FIELD_SIZE/2, FIGURESTATE_TEXTURE_WIDTH = FIGURESTATE_TEXTURE_HEIGHT,
ANNOUNCEMENT_X = (2*SCREEN_WIDTH)/5,ANNOUNCEMENT_Y = (2*SCREEN_HEIGHT)/5,
DIPLOMAT_DESERT_X = SCREEN_WIDTH/2 - 50, DIPLOMAT_DESERT_Y = SCREEN_HEIGHT/2 - 50};
enum{FIGURE_BUTTON_HEIGHT = STANDARD_FIELD_SIZE, FIGURE_BUTTON_WIDTH = STANDARD_FIELD_SIZE * 10};
enum{MODULO_FAIL = -1000};
enum Landscape{GRASSLAND, PLAINS, OCEAN, RIVER, MOUNTAIN, HILLS, TUNDRA, ARCTIC, SWAMP, JUNGLE, FOREST, DESERT};
class MovableDrawingElement;
enum SettlersWork{NONE, IRRIGATE, MAKE_MINING , BUILD_ROAD, BUILD_RAILROAD, BUILD_FORTRESS,CLEAN_UP_POLLUTION};
enum RoadStatus{NOTHING, ROAD, RAILROAD};
enum GovernmentType{ANARCHY, DESPOTISM, MONARCHY, COMMUNISM, REPUBLIC, DEMOCRACY};
enum SettlersWorkingTime{SETTLERSWORK_UNAVAILABLE = -1, STANDARD_ROAD_BUILDING_TIME = 2, STANDARD_RAILROAD_BUILDING_TIME = 4, STANDARD_FORTRESS_BUILDING_TIME = 6, STANDARD_POLLUTION_CLEANING_TIME = 6
, STANDARD_IRRIGATION_TIME = 6, STANDARD_FORESTING_TIME = 12, STANDARD_GRASSLANDING_TIME = 12};
enum{FIELD_TEXTURE_AMOUNT = 30};
enum Nationality {ROMAN, RUSSIAN, ZULU, GREEK, BABYLONIAN, ENGLISH, CHINESE, AMERICAN, GERMAN, FRENCH, AZTEC, EGYPTIAN, BARBARIAN, INDIAN, MONGOL,NO_NATIONALITY};
enum FigureState{MOVING, SENTRIED, SENTRYING, FORTIFYING, FORTIFIED,COMPLETELY_FORTIFIED, FIGHT_IN_PROGRESS, PILLAGE_IN_PROGRESS, DONE_WITH_TURN, SETTLERS_AT_WORK};
enum{SMALL_DRAWING_FAIL = -1};
enum{STANDARD_LINE_THICKNESS = STANDARD_FIELD_SIZE / 8, INFO_TEXT_X = 200, INFO_TEXT_Y = 200};
enum{FIGURE_INFO_WIDTH = SCREEN_WIDTH/5, FIGURE_INFO_HEIGHT = 160,  FIGURE_INFO_FONT_SIZE = SCREEN_WIDTH/30, FIGURE_INFO_Y = SCREEN_HEIGHT*4/10, NATION_FIGURE_INFO_BORDER_THICKNESS = 5, NATION_INFO_HEIGHT = 69};
enum DrawState {NOT_IN_ANY_DRAWING, BLINKING, NORMAL_DRAWING, SOMETHING_OTHER};
enum {CITIZENS_OVERVIEW_HEIGHT = STANDARD_FIELD_SIZE, IMPROVEMENTS_OVERVIEW_WIDTH = SCREEN_WIDTH / 3,
 CITIZEN_SCALEFACTOR = STANDARD_FIELD_SIZE / 20, CITIZEN_HEIGHT = 13, CITIZEN_MAX_WIDTH = 8,FIRST_CITIZEN_X = CITIZEN_MAX_WIDTH, RESOURCES_TEXT_HEIGHT = STANDARD_FIELD_SIZE,
RESOURCES_SCALEFACTOR = CITIZEN_SCALEFACTOR, RESOURCES_SCALEFACTOR_CITY_OVERVIEW = STANDARD_FIELD_SIZE/15,
CITYSURFACE_BUTTON_HEIGHT = 30, PRODUCTION_OVERVIEW_WIDTH = SCREEN_WIDTH - IMPROVEMENTS_OVERVIEW_WIDTH - 5*STANDARD_FIELD_SIZE};
enum CitySurfaceSize{FOOD_STORAGE_HEIGHT = 138*8*RESOURCES_SCALEFACTOR/12, FOOD_STORAGE_WIDTH = 13* RESOURCES_SCALEFACTOR*7,FOOD_STORAGE_Y = SCREEN_HEIGHT - FOOD_STORAGE_HEIGHT,
RESOURCE_TEXT_SHIELD_DIFFERENCE = RESOURCES_SCALEFACTOR*8, FIGURE_OVERVIEW_HEIGHT = 2*STANDARD_FIELD_SIZE,
SHIELD_OVERVIEW_WIDTH = 11*8*RESOURCES_SCALEFACTOR, CITIZENS_OVERVIEW_WIDTH = SCREEN_WIDTH - IMPROVEMENTS_OVERVIEW_WIDTH,
BUYBUTTON_WIDTH = 45, CHANGEBUTTON_WIDTH = 135,
SHIELD_OVERVIEW_HEIGHT = FOOD_STORAGE_HEIGHT + 10*RESOURCES_SCALEFACTOR,
IMPROVEMENT_OVERVIEW_HEIGHT_NORMED = 12, IMPROVEMENT_OVERVIEW_WIDTH = SCREEN_WIDTH - PRODUCTION_OVERVIEW_WIDTH - 5*STANDARD_FIELD_SIZE,
STANDARD_TEXT_HEIGHT = RESOURCES_TEXT_HEIGHT/2,
BUY_INFO_X = SCREEN_WIDTH/2, BUY_INFO_Y = SCREEN_HEIGHT/2};
enum SubSurfaceSize{
	SUBSURFACE_BUTTON_HEIGHT = CITYSURFACE_BUTTON_HEIGHT, SUBSURFACE_HEIGHT = FOOD_STORAGE_HEIGHT, SUBSURFACE_WIDTH = SCREEN_WIDTH - FOOD_STORAGE_WIDTH - SHIELD_OVERVIEW_WIDTH
};
enum CitizenDrawingContext{CITIZENCONTEXT_SURFACE,CITIZENCONTEXT_SUBSURFACE};
enum HappyVectorType{HAPPY_ALL = 0, HAPPY_1 = 1, HAPPY_2 = 2, HAPPY_3 = 3, HAPPY_4 = 4};
enum SubSurfaceState{
	SUBSURFACE_INFO = 0, SUBSURFACE_HAPPY = 1, SUBSURFACE_MAP = 2, SUBSURFACE_VIEW = 3
};
enum{TAX_RATE_STEP_COUNT = 10};
enum TradeType{TRADE_GOLD, TRADE_SCIENCE, TRADE_LUXURIES};
enum BlinkingTime{STANDARD_BLINKING_INTERVAL_TIME = 1500};
enum{MAX_CITY_NAME_LENGTH = 12, CITIES_PER_NATION = 15, ADDITIONAL_CITIES = 20};

enum ImprovementType{IMPROVEMENT_SETTLERS = SETTLERS, IMPROVEMENT_TRIREME = TRIREME, IMPROVEMENT_CARRIER = CARRIER, IMPROVEMENT_ARTILLERY = ARTILLERY, IMPROVEMENT_BOMBER = BOMBER, IMPROVEMENT_FIGHTER = FIGHTER, IMPROVEMENT_CAVALRY = CAVALRY, IMPROVEMENT_DIPLOMAT = DIPLOMAT,
IMPROVEMENT_LEGION = LEGION,IMPROVEMENT_CHARIOT = CHARIOT,IMPROVEMENT_IRONCLAD = IRONCLAD,IMPROVEMENT_CRUISER = CRUISER,IMPROVEMENT_SUBMARINE = SUBMARINE,IMPROVEMENT_MECH_INF = MECH_INF,IMPROVEMENT_ARMOR = ARMOR,IMPROVEMENT_KNIGHTS=KNIGHTS,IMPROVEMENT_MUSKETEERS=MUSKETEERS,IMPROVEMENT_RIFLEMEN=RIFLEMEN,IMPROVEMENT_BATTLESHIP=BATTLESHIP,IMPROVEMENT_NUCLEAR=NUCLEAR,IMPROVEMENT_SAIL=SAIL,IMPROVEMENT_CARAVAN=CARAVAN,IMPROVEMENT_CANNON=CANNON,IMPROVEMENT_CATAPULT=CATAPULT,IMPROVEMENT_TRANSPORT=TRANSPORT,IMPROVEMENT_FRIGATE=FRIGATE,IMPROVEMENT_MILITIA=MILITIA,IMPROVEMENT_PHALANX=PHALANX,
TEMPLE = 5667, GRANARY, PALACE, COURTHOUSE,
AQUEDUCT,BANK,LIBRARY,FACTORY,CATHEDRAL,BARRACKS,MFG_PLANT,NUCLEAR_PLANT,
COLOSSEUM,POWER_PLANT,MARKETPLACE, MASS_TRANSIT,RECYCLING_CENTER,SDI_DEFENSE,CITY_WALLS, UNIVERSITY,
HYDRO_PLANT, SS_STRUCTURAL,SS_COMPONENT,SS_MODULE,SPACESHIP_MIN = SS_STRUCTURAL,SPACESHIP_MAX = SS_MODULE,
APOLLO_PROGRAM = 70000, GREAT_WALL, DARWINS_VOYAGE,MAGELLANS_EXPEDITION,
WOMENS_SUFFRAGE,GREAT_LIBRARY,HOOVER_DAM,HANGING_GARDENS,ISAAC_NEWTONS_COLLEGE,
BACH_CATHEDRAL,MICHELANGELOS_CHAPEL,COLOSSUS,COPERNICUS_OBSERVATORY,CURE_FOR_CANCER,
LIGHTHOUSE,MANHATTAN_PROJECT,ORACLE,PYRAMIDS,SETI_PROGRAM,
SHAKESPEARES_THEATRE,UNITED_NATIONS,
BUILDING_MIN = TEMPLE,BUILDING_MAX = HYDRO_PLANT,FIGURE_MIN = IMPROVEMENT_SETTLERS,WONDER_MIN=APOLLO_PROGRAM,WONDER_MAX = UNITED_NATIONS
};
enum{MAX_SAVESLOT_LENGTH = 70};
enum ClimateState{NO_CLIMATE_ALARM,GREEN_CLIMATE_ALARM,YELLOW_CLIMATE_ALARM,ORANGE_CLIMATE_ALARM,RED_CLIMATE_ALARM};
enum DiplomatChoice{MEET_WITH_KING,INVESTIGATE_CITY,STEAL_TECHNOLOGY,INCITE_REVOLT,ESTABLISH_EMBASSY,INDUSTRIAL_SABOTAGE};
enum StartingMode{LOAD_GAME,START_A_NEW_GAME};
extern int POSSIBLE_SAVE_FILES;
typedef bool BlinkingState;typedef std::function<int(int,int,SDL_Renderer*)> drawingFunction;
constexpr BlinkingState VISIBLE(){return true;}
constexpr BlinkingState INVISIBLE(){return false;}
#define IMPROVEMENT_UNIT_LAYER 0
#define IMPROVEMENT_LAYER 1
#define IMPROVEMENT_WONDER_LAYER 2
#ifndef PUSH
#define PUSH(a,b) theJson[a]=b;
#define AT(a,c) a = j.at(c);
#define GET(a,c) a = j[c].get<std::vector<int>>();
#endif

typedef int ContinentID;
const ContinentID NO_CONTINENT_ID_GIVEN = WORLD_LENGTH*WORLD_HEIGHT;
class MovementPoints{
public:
	int m_movementPoints = 0;
	MovementPoints(){}
	MovementPoints(int x):m_movementPoints(x){}
	bool operator==(const int whatToCompare){return m_movementPoints == whatToCompare;}
	bool operator==(const MovementPoints& whatToCompare){return whatToCompare.m_movementPoints == m_movementPoints;}
	MovementPoints& operator=(MovementPoints x){m_movementPoints += x.m_movementPoints; return *this;}
	MovementPoints& operator=(int movementPoints){m_movementPoints += movementPoints; return *this;}
	MovementPoints operator% (int x){return MovementPoints(m_movementPoints % x);}
	bool operator<(int x){return m_movementPoints<x;}
	bool operator-=(int y){return m_movementPoints+=(-y);}
	MovementPoints& operator-=(MovementPoints x){m_movementPoints-=x.m_movementPoints; return *this;}
};
extern MovementPoints FIGHT_IS_COMING;
extern MovementPoints MOVE_PROHIBITED;
int modulo (int i, int j);
int xModulo(int);
int yModulo(int);


class Coordinate{
public:
    int x;
    int y;
    Coordinate(const Coordinate& f): x(f.x),y(f.y){}
    Coordinate(int a, int b);
    void print(std::ostream& os = std::cout)
    {
        os<<x<<" "<<y<<std::endl;
    }
    bool operator==(const Coordinate& other);
    bool operator!=(const Coordinate& other);
};

class Texture{
protected:
	SDL_Texture* m_theTexture;
	int m_width;
	int m_height;
public:
	Texture(SDL_Texture* texture, int width, int height);
	~Texture();
	int m_Width();
	int m_Height();
	SDL_Texture* theTexture();
};

extern std::vector<std::shared_ptr<Texture>> fieldTextures, resourceTextures,shieldTextures;
extern SDL_Renderer* theRenderer;

class PoleHitException: public std::exception{
public:
	PoleHit m_poleHit;
	PoleHitException(PoleHit ph):m_poleHit(ph){}
	char* what(){
		switch(m_poleHit){
		case NORTH_POLE_HIT: return "North Pole was hit!\n";
		case SOUTHERN_POLE_HIT: return "Southern Pole was hit!\n";
		default: return "Weird Pole Hit!\n";
		}
	}
};

class NullPointerException{
public:
	std::string m_class;
	std::string m_functionData = "";
	NullPointerException(std::string s, std::string ss = ""): m_class(s), m_functionData(ss){}
	std::string what(){return "NullPointerException: "+m_class+"\n"+m_functionData;}
};

class SettlersworkUnavailable{
	std::string what(){return "Settlerswork unavailable!";}
};

class RandomFail{
public:
	int m_upperBound;
	int m_lowerBound;
	RandomFail(int i, int j):m_upperBound(i), m_lowerBound(j){}
	std::string what(){
		std::stringstream s;
		s<< "Bad random request: upperBound = "<<(m_upperBound)<<(", lowerBound = ")<<m_lowerBound<<"!";
		return s.str();
	}
};

class DrawingFail{
public:
	std::string m_whatsUp;
	DrawingFail();
	DrawingFail(std::string whatsUp){m_whatsUp = std::string("DrawingFail: ")+whatsUp;}
	std::string what(){return m_whatsUp;}
};

class Fight{
public:
	FightResult m_result;
	Fight(FightResult result):m_result(result){}
	std::string what(){return "Fight";}
};

class CityFounded{

};

class TooManyCities{

};

class SDLQuitException{

};

int SDL_SetRenderDrawColor(SDL_Renderer* renderer, SDL_Color color);

namespace Graphics{
bool ratherShieldThanResource(Landscape ls);
int drawSquareLines(SDL_Renderer*, int, int, SDL_Color);
int drawSquareStarLines(SDL_Renderer*, int, int, SDL_Color);
int drawThickerDiagonalLineDown(SDL_Renderer* renderer, int x, int y,  int thickness = STANDARD_LINE_THICKNESS);
int drawThickerHorizontalLine(SDL_Renderer* renderer, int x, int y, int thickness = STANDARD_LINE_THICKNESS);
int drawThickerVerticalLine(SDL_Renderer* renderer, int x, int y, int thickness = STANDARD_LINE_THICKNESS);
int drawThickerDiagonalLineUp(SDL_Renderer* renderer, int x, int y, int thickness = STANDARD_LINE_THICKNESS);
SDL_Color redColor();
namespace Civ{
extern std::shared_ptr<Texture> miningTexture;
SDL_Color yellowColor();
SDL_Color shieldGreyColor();
SDL_Color resourcesWhiteColor();
SDL_Color cityBackgroundColor();
SDL_Color cityNameColor();
SDL_Color cityOccupiedColor();
SDL_Color brightCityBackgroundColor();
SDL_Color turquoiseColor();
SDL_Color darkgreyColor();
SDL_Color irrigationBlueColor();
int drawPollution(SDL_Renderer* renderer, int x, int y);
int drawIrrigation(SDL_Renderer* renderer, int x, int y);
int drawFood(SDL_Renderer* renderer, int x, int y, int scaleFactor, bool minus = false);
int drawShield(SDL_Renderer* renderer, int x, int y, int scaleFactor, bool minus = false);
int drawTrade(SDL_Renderer* renderer, int x, int y, int scaleFactor, bool corruption = false);
int drawUnhappyFace(SDL_Renderer* renderer, int x, int y, int scaleFactor);
int drawLuxury(SDL_Renderer* renderer, int x, int y, int scaleFactor);
int drawGold(SDL_Renderer* renderer, int x, int y, int scaleFactor);
int drawScience(SDL_Renderer* renderer, int x, int y, int scaleFactor);
SDL_Color alarmColor(ClimateState cs);
extern Nationality currentNationality;
}
extern DrawState m_whatsUpDrawingwise;
}

char getSettlersOrder(SettlersWork work);

std::ostream& operator<<(std::ostream& os, Nationality nationality);
std::ostream& operator<<(std::ostream& os, DiplomatChoice dc);

extern int CONTENT_BASE;
extern TTF_Font* theFont, *citySizeFont;
extern SDL_Color whiteColor, blackColor, brownColor, infoTextColor, infoTextBackgroundColor;
extern Figure* figureToDraw;
extern Field* fieldToDraw;
extern unsigned int settlersCount;

std::ostream& operator<<(std::ostream& os, FigureType figureType);
std::ostream& operator<<(std::ostream& os, MovementPoints mp);
std::ostream& operator<<(std::ostream& os, Coordinate &co);
std::ostream& operator<<(std::ostream& os, ImprovementType imptype);

#define PRINT_I_J 			std::cout<<"i = "<<i<<", j = "<<j<<std::endl;

namespace Coordinates{
constexpr int leftCornerX(){return FIGURE_INFO_WIDTH;}
constexpr int leftCornerY(){return 0;}
Coordinate leftCornerCoordinate();
}
class InertCitizenState{
public:
	CitizenState m_state;
	InertCitizenState(CitizenState State): m_state(State){}
};

#define NATION_INFO_Y FIGURE_INFO_Y - NATION_FIGURE_INFO_BORDER_THICKNESS - NATION_INFO_HEIGHT

class QuitSurface{

};

CitizenState operator++(CitizenState& state);
CitizenState operator--(CitizenState& state);

std::vector<Coordinate> coordinatesAroundField(int visibilityRange = 1);

#ifndef IS_IN_VECTOR_
#define IS_IN_VECTOR_
template<typename T>
bool isInVector(std::vector<T>& theVector, const T& whatToFind, bool (*equals) (const T& t1, const T& t2)){
	for(const T& t: theVector){
		if(equals(whatToFind, t)){
			return true;
		}
	}
	return false;
}

class DoNothing: public std::function<void()>{
public:
	void operator()();
	DoNothing();
};
class SelectionReturn;
namespace Miscellaneous{
SDL_Rect printMultipleLines(const std::string& str, int x, int y, SDL_Color color, bool shaded = false, SDL_Color backgroundColor = whiteColor);
void displayText(const std::string& str, int x, int y, SDL_Color color, bool shaded = false, SDL_Color backgroundColor = whiteColor);
bool yesOrNo(int x = FIGURE_INFO_WIDTH, int y = SCREEN_HEIGHT/3, std::string yes = "yes", std::string no = "no");
void holdImage();
SelectionReturn selectSavingSlot();
}

extern unsigned int figureCountGlobal, cityCountGlobal;
namespace GlobalWarming{
extern int SWAMPS_TO_MAKE,DESERTS_TO_MAKE,WARMING_EXACERBATION, DESERTIFICATION_BOUND, HORIZONTAL_HOPPING_DISTANCE, DESERTIFICATION_SPREADER, TURNS_UNTIL_SWAMPED;
}
extern int slotNumber;

#endif

#endif
