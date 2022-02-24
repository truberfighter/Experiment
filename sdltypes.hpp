#ifndef SDLTYPES_HPP
#define SDLTYPES_HPP

#include <SDL2\SDL.h>
#include <iostream>
#include <SDL2\SDL_image.h>
#include<memory>
#define KeyCode unsigned int
enum{SDLK_1_DOWN_LEFT = 1073741913, SDLK_2_DOWN = SDLK_1_DOWN_LEFT + 1, SDLK_3_DOWN_RIGHT = SDLK_2_DOWN+1,
	SDLK_4_LEFT = SDLK_3_DOWN_RIGHT + 1, SDLK_5_CENTER = SDLK_4_LEFT + 1, SDLK_6_RIGHT = SDLK_5_CENTER+1,
	SDLK_7_UP_LEFT = SDLK_6_RIGHT + 1, SDLK_8_UP = SDLK_7_UP_LEFT + 1, SDLK_9_UP_RIGHT = SDLK_8_UP+1,
};
enum{STANDARD_DRAWING_TIME = 200};
enum layerenum{STANDARD_LAYER = 100};
typedef layerenum Layer;
enum Drawing_Element{MOVABLE_DRAWING_ELEMENT, DRAWING};
enum Direction{DOWN_LEFT = 1, DOWN = 2, DOWN_RIGHT = 3, LEFT = 4, STANDING_STILL = 5,
	RIGHT = 6, UP_LEFT = 7, UP = 8, UP_RIGHT = 9};
class MovableDrawingElement;

class Coordinate{
public:
    int x;
    int y;
    Coordinate(const Coordinate& f): x(f.x),y(f.y){}
    Coordinate(int a, int b);
    void ausgeben()
    {
        std::cout<<x<<" "<<y<<std::endl;
    }
    bool operator==(const Coordinate& other);
    bool operator!=(const Coordinate& other);
};

class Texture{
private:
	SDL_Texture* m_theTexture;
	int m_width;
	int m_height;
public:
	Texture(SDL_Texture* texture, int width, int height);
	~Texture();
	int Width();
	int Height();
	SDL_Texture* theTexture();
};

#endif
