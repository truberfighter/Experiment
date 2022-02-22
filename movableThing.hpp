#ifndef MOVABLETHING_H
#define MOVABLETHING_H

#include <SDL2\SDL.h>
#include <iostream>
#include <SDL2\SDL_image.h>
#include "sdltypes.hpp"
#include <list>

struct Velocity{
	int s_xVelocity;
	int s_yVelocity;
};

enum{STANDARD_x_VELOCITY = 100, STANDARD_y_VELOCITY = STANDARD_x_VELOCITY, NOT_MOVING = 0};
enum{STANDARD_DRAWING_TIME = 200};
class MovableThing{//Redactoring coming: Rect-Farb-Version muss kommen
protected:
	std::list<MovableDrawingElement*> m_howDrawn;
	unsigned int m_whenMoved = 0;
	int m_x;
	int m_y;
	int m_xVelocity = 0;
	int m_yVelocity = 0;
	int m_standardXVelocity;
	int m_standardYVelocity;
	SDL_Renderer* m_renderer;
	Texture* m_texture;
public:
	static unsigned int moveCount;
	void m_add(MovableDrawingElement* newMoDrEl);
	MovableThing(SDL_Renderer* renderer, Texture* texture, int x, int y, bool renderInstantly = false);
	MovableThing(SDL_Renderer* renderer, int width, int height, const char* filename, int x, int y, bool renderInstantly = false);
	bool m_drawNew(int x, int y, SDL_Renderer* renderer, Texture* texture);
	bool m_drawNew(int x, int y);
	bool m_drawNew(SDL_Renderer* renderer, Texture* texture);
	bool m_drawNew(Texture* texture);
	bool m_drawNew(SDL_Renderer* renderer);
	bool m_drawNew();
	void m_setMoveToDirection(Direction whereToGo, float howFar = 1);
	Velocity m_getVelocity();
	void m_setVelocity(Velocity v);
	void m_setVelocity(int xVelocity, int yVelocity);
	bool m_drawAfterMotion(Texture* texture, bool moveFurther = false);
	bool m_drawAfterMotion(bool moveFurther = false);
	void m_move(bool moveFurther = false);
	//erst mal zum Testen
	bool m_drawRight();
	Velocity m_getStandardVelocity(Direction d = DOWN_RIGHT);
	Coordinate getPosition();
	unsigned int WhenMoved();
	//void setWhenMoved(unsigned int whenMoved);
};

#endif
