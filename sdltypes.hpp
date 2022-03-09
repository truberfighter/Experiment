#ifndef SDLTYPES_HPP
#define SDLTYPES_HPP

#include "SDL2\SDL_ttf.h"
#include <SDL2\SDL.h>
#include <iostream>
#include <SDL2\SDL_image.h>
#include <memory>
#include <vector>
#define KeyCode unsigned int
class Field;
#define Meridian std::vector<std::shared_ptr<Field>>
enum{SDLK_1_DOWN_LEFT = 1073741913, SDLK_2_DOWN = SDLK_1_DOWN_LEFT + 1, SDLK_3_DOWN_RIGHT = SDLK_2_DOWN+1,
	SDLK_4_LEFT = SDLK_3_DOWN_RIGHT + 1, SDLK_5_CENTER = SDLK_4_LEFT + 1, SDLK_6_RIGHT = SDLK_5_CENTER+1,
	SDLK_7_UP_LEFT = SDLK_6_RIGHT + 1, SDLK_8_UP = SDLK_7_UP_LEFT + 1, SDLK_9_UP_RIGHT = SDLK_8_UP+1,
};
enum{STANDARD_DRAWING_TIME = 200, STANDARD_FIELD_SIZE = 24};
enum Layer{STANDARD_LAYER = 100, STANDARD_FIELD_LAYER = -300};
enum FigureCategory{GROUND, SEA, FLIGHT};
enum Drawing_Element{MOVABLE_DRAWING_ELEMENT, DRAWING, IMMOVABLE_DRAWING_ELEMENT};
enum Direction{DOWN_LEFT = 1, DOWN = 2, DOWN_RIGHT = 3, LEFT = 4, STANDING_STILL = 5,
	RIGHT = 6, UP_LEFT = 7, UP = 8, UP_RIGHT = 9};
enum{WORLD_LENGTH = 40, WORLD_HEIGHT = 40, SCREEN_WIDTH_NORMED = 16, SCREEN_HEIGHT_NORMED = 12,
SCREEN_WIDTH = SCREEN_WIDTH_NORMED*STANDARD_FIELD_SIZE, SCREEN_HEIGHT = SCREEN_HEIGHT_NORMED*STANDARD_FIELD_SIZE};
enum{MODULO_FAIL = -1};
enum Landscape{GRASSLAND, PLAINS, OCEAN, RIVER, MOUNTAIN, HILLS, TUNDRA, ARCTIC, SWAMP, JUNGLE, FOREST, DESERT};
class MovableDrawingElement;
enum SettlersWork{NONE, IRRIGATE, MAKE_MINING, CHANGE_TO_FOREST, CHANGE_TO_PLAINS
, CHANGE_TO_GRASSLAND, BUILD_ROAD, BUILD_RAILROAD, BUILD_FORTRESS, BUILD_BRIDGE};
enum RoadStatus{NOTHING, ROAD, RAILROAD};
enum GovernmentType{ANARCHY, DESPOTISM, MONARCHY, COMMUNISM, REPUBLIC, DEMOCRACY};
enum SettlersWorkingTime{SETTLERSWORK_UNAVAILABLE = -1, STANDARD_ROAD_BUILDING_TIME = 2, STANDARD_RAILROAD_BUILDING_TIME = 4, STANDARD_FORTRESS_BUILDING_TIME = 6
, STANDARD_IRRIGATION_TIME = 6, STANDARD_FORESTING_TIME = 12};
enum{FIELD_TEXTURE_AMOUNT = 30};

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
public:
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

#endif
