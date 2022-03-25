#ifndef SDLTYPES_HPP
#define SDLTYPES_HPP

#include "SDL2\SDL_ttf.h"
#include <SDL2\SDL.h>
#include <iostream>
#include <SDL2\SDL_image.h>
#include <memory>
#include <vector>
#define MAIN_LOOP_BEGIN  bool quit = false; while(!quit){ while(SDL_PollEvent(&currentEvent)!=0){if(currentEvent.type == SDL_QUIT) quit = true;
#define MAIN_LOOP_END }}
#define KeyCode unsigned int
class Field;
class Figure;
#define Meridian std::vector<std::shared_ptr<Field>>
enum PoleHit{NORTH_POLE_HIT, SOUTHERN_POLE_HIT};
enum{SDLK_1_DOWN_LEFT = 1073741913, SDLK_2_DOWN = SDLK_1_DOWN_LEFT + 1, SDLK_3_DOWN_RIGHT = SDLK_2_DOWN+1,
	SDLK_4_LEFT = SDLK_3_DOWN_RIGHT + 1, SDLK_5_CENTER = SDLK_4_LEFT + 1, SDLK_6_RIGHT = SDLK_5_CENTER+1,
	SDLK_7_UP_LEFT = SDLK_6_RIGHT + 1, SDLK_8_UP = SDLK_7_UP_LEFT + 1, SDLK_9_UP_RIGHT = SDLK_8_UP+1,
};
enum{STANDARD_DRAWING_TIME = 200, STANDARD_FIELD_SIZE = 24};
enum Layer{STANDARD_LAYER = 100, STANDARD_FIELD_LAYER = -300, STANDARD_FIELD_MODIFICATOR_LAYER = 0};
enum FigureCategory{GROUND, SEA, FLIGHT};
enum Drawing_Element{MOVABLE_DRAWING_ELEMENT, DRAWING, IMMOVABLE_DRAWING_ELEMENT,LAMBDA_DRAWING_ELEMENT};
enum Direction{DOWN_LEFT = 1, DOWN = 2, DOWN_RIGHT = 3, LEFT = 4, STANDING_STILL = 5,
	RIGHT = 6, UP_LEFT = 7, UP = 8, UP_RIGHT = 9};
enum{WORLD_LENGTH = 40, WORLD_HEIGHT = 40, SCREEN_WIDTH_NORMED = 16, SCREEN_HEIGHT_NORMED = 12,
SCREEN_WIDTH = SCREEN_WIDTH_NORMED*STANDARD_FIELD_SIZE, SCREEN_HEIGHT = SCREEN_HEIGHT_NORMED*STANDARD_FIELD_SIZE,
MODULO_WIDTH_FOR_NEGATIVE = 2, MODULO_HEIGHT_FOR_NEGATIVE = MODULO_WIDTH_FOR_NEGATIVE,
FIGURESTATE_TEXTURE_HEIGHT = STANDARD_FIELD_SIZE/2, FIGURESTATE_TEXTURE_WIDTH = FIGURESTATE_TEXTURE_HEIGHT};
enum{MODULO_FAIL = -1000};
enum Landscape{GRASSLAND, PLAINS, OCEAN, RIVER, MOUNTAIN, HILLS, TUNDRA, ARCTIC, SWAMP, JUNGLE, FOREST, DESERT};
class MovableDrawingElement;
enum SettlersWork{NONE, IRRIGATE, MAKE_MINING, CHANGE_TO_FOREST, CHANGE_TO_PLAINS
, CHANGE_TO_GRASSLAND, BUILD_ROAD, BUILD_RAILROAD, BUILD_FORTRESS, BUILD_BRIDGE};
enum RoadStatus{NOTHING, ROAD, RAILROAD};
enum GovernmentType{ANARCHY, DESPOTISM, MONARCHY, COMMUNISM, REPUBLIC, DEMOCRACY};
enum SettlersWorkingTime{SETTLERSWORK_UNAVAILABLE = -1, STANDARD_ROAD_BUILDING_TIME = 2, STANDARD_RAILROAD_BUILDING_TIME = 4, STANDARD_FORTRESS_BUILDING_TIME = 6
, STANDARD_IRRIGATION_TIME = 6, STANDARD_FORESTING_TIME = 12};
enum{FIELD_TEXTURE_AMOUNT = 30};
enum Nationality {ROMAN, RUSSIAN, ZULU, GREEK, BABYLONIAN, ENGLISH, CHINESE, AMERICAN, GERMAN, FRENCH, AZTEC, EGYPTIAN, BARBARIAN, INDIAN, MONGOL};
enum FigureState{MOVING, SENTRIED, SENTRYING, FORTIFYING, FORTIFIED, FIGHT_IN_PROGRESS, PILLAGE_IN_PROGRESS, DONE_WITH_TURN, SETTLERS_AT_WORK};
enum{SMALL_DRAWING_FAIL = -1};

typedef short unsigned int ContinentID;
const ContinentID NO_CONTINENT_ID_GIVEN = WORLD_LENGTH*WORLD_HEIGHT;
typedef int MovementPoints;
extern MovementPoints FIGHT_IS_COMING;
extern MovementPoints MOVE_PROHIBITED;
int modulo (const int& i, const int& j);
int xModulo(const int&);
int yModulo(const int&);


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

extern std::shared_ptr<Texture>* fieldTextures;
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

int drawSquareLines(SDL_Renderer*, int, int, SDL_Color);

extern TTF_Font* theFont;
extern SDL_Color whiteColor;
extern Figure* figureToDraw;




#define PRINT_I_J 			cout<<"i = "<<i<<", j = "<<j<<endl;


#endif
