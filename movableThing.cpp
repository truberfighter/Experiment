/*
 * movableThing.cpp
 *
 *  Created on: 15.02.2022
 *      Author: uwe-w
 */
#include <SDL2\SDL.h>
#include <iostream>
#include <SDL2\SDL_image.h>
#include "sdltypes.hpp"
#include "movableThing.hpp"
#include "drawing.hpp"

using namespace std;

unsigned int MovableThing::moveCount;


MovableThing::MovableThing(SDL_Renderer* renderer, std::shared_ptr<Texture> texture, int x, int y, bool renderInstantly)
: m_x(x), m_y(y), m_renderer(renderer), m_texture(texture)
{
	cout<<"MovableThing-Konstruktor (generell), this = "<<this<<endl;
	if(!renderer){
		cout<<"No valid renderer for MovableThing"<<endl;
		//return;
	}
	if(!texture){
		cout<<"No valid texture for MovableThing"<<endl;
		//return;
	}
	else{
		m_standardXVelocity = texture->m_Width();
		m_standardYVelocity = texture->m_Height();
	}
	if(renderInstantly){
		m_drawNew(m_x, m_y, m_renderer, m_texture);
	}
}

MovableThing::MovableThing(SDL_Renderer* renderer, int width, int height, const char* filename, int x, int y, bool renderInstantly)
: MovableThing(renderer, nullptr, x % (STANDARD_FIELD_SIZE*WORLD_LENGTH), y % (STANDARD_FIELD_SIZE * WORLD_HEIGHT))
{
	SDL_Texture* newTexture = IMG_LoadTexture(renderer, filename);
	if(!newTexture)cout<<"Fehler für "<<filename<<": \nSDL_Error: "<<SDL_GetError()<<endl;
	//SDL_Rect r{100,100,200,50};
	//SDL_RenderCopy(renderer, newTexture, NULL, &r);
	//SDL_RenderPresent(renderer);
	//SDL_Delay(3000);
	std::shared_ptr<Texture> s = std::make_shared<Texture>(newTexture, width, height);
	m_texture = s;
	if(!m_texture)cout<<"Fehler für "<<filename<<endl;
	m_standardXVelocity = m_texture->m_Width();
	m_standardYVelocity = m_texture->m_Height();
	cout<<"movableThing-Konstruktor beendet"<<endl;
}


bool MovableThing::m_drawNew(int x, int y, SDL_Renderer* renderer, std::shared_ptr<Texture> texture){

	if(!renderer){
		cout<<"No valid renderer for MovableThing"<<endl;
		return false;
	}
	if(!texture){
		cout<<"No valid texture for MovableThing"<<SDL_GetError()<<endl;
		return false;
	}
	SDL_Rect rect;
	rect.x = x% (STANDARD_FIELD_SIZE*WORLD_LENGTH);
	rect.y = y% (STANDARD_FIELD_SIZE * WORLD_HEIGHT);
	rect.h = texture->m_Height();
	rect.w = texture->m_Width();
	//cout<<"x: "<<x% (STANDARD_FIELD_SIZE * WORLD_LENGTH)<<", y: "<<y% (STANDARD_FIELD_SIZE * WORLD_HEIGHT)<<"Height: "<<rect.h<<"Width: "<<rect.w<<endl;
	SDL_Texture* textureToDraw = m_texture->theTexture();
	if(!textureToDraw){
		cout<<"No SDL_texture in the texture!\n"<<SDL_GetError()<<endl;
	}
	if(SDL_RenderCopy(renderer, texture->theTexture(), NULL, &rect)!=0){
		cout<<"SDL_Error: %s\n"<<SDL_GetError()<<endl;
		return false;
	}
	//SDL_RenderPresent(m_renderer);
	//Update data for your instance
	m_renderer = renderer;
	m_texture = texture;
	m_x = x% (STANDARD_FIELD_SIZE*WORLD_LENGTH);
	m_y = y% (STANDARD_FIELD_SIZE * WORLD_HEIGHT);
	//SDL_Delay(STANDARD_DRAWING_TIME);
	return true;
}

bool MovableThing::m_drawNew(SDL_Renderer* renderer, std::shared_ptr<Texture> texture){
	return 	m_drawNew(m_x, m_y ,renderer, texture);
}

bool MovableThing::m_drawNew(std::shared_ptr<Texture> texture){
	return 	m_drawNew(m_x, m_y ,m_renderer, texture);
}

bool MovableThing::m_drawNew(int x, int y, SDL_Renderer* renderer){
	return m_drawNew(x,y,renderer, m_texture);
}

bool MovableThing::m_drawNew(SDL_Renderer* renderer){
	return 	m_drawNew(m_x, m_y ,renderer, m_texture);
}

bool MovableThing::m_drawNew(){
	return 	m_drawNew(m_x, m_y ,m_renderer, m_texture);
}

bool MovableThing::m_drawNew(int x, int y){
	return 	m_drawNew(x, y ,m_renderer, m_texture);
}
bool MovableThing::m_drawAfterMotion(std::shared_ptr<Texture> texture, bool moveFurther){
	 if(m_drawNew(m_x + m_xVelocity, m_y + m_yVelocity, m_renderer, texture)){
		 m_move();
		 return true;
	 }
	 return false;
}

bool MovableThing::m_drawAfterMotion(bool moveFurther){
	return m_drawAfterMotion(m_texture, moveFurther);
}

bool MovableThing::m_drawRight(){
	m_xVelocity = m_standardXVelocity;
	return m_drawAfterMotion();
}

void MovableThing::m_move(bool moveFurther){
	m_x=xModulo(m_xVelocity+m_x);
	m_y=yModulo(m_y+ m_yVelocity);
	//cout<<"m_move(): x: "<<m_x<<"y: "<<m_y<<endl;
	if(!moveFurther){
		 m_xVelocity = NOT_MOVING;
		 m_yVelocity = NOT_MOVING;
	 }
	m_whenMoved = ++moveCount;
	for(MovableDrawingElement* it: m_howDrawn){
		//cout<<"Climbing to top!"<<endl;
		it->m_climbToTop();
	}

}

Velocity MovableThing::m_getVelocity(){
	return Velocity{m_xVelocity, m_yVelocity};
}

void MovableThing::m_setVelocity(int xVelocity, int yVelocity){
	m_xVelocity = xVelocity;
	m_yVelocity = yVelocity;
}

void MovableThing::m_setVelocity(Velocity v){
	m_setVelocity(v.s_xVelocity, v.s_yVelocity);
}

void MovableThing::m_setMoveToDirection(Direction whereToGo, float howFar){
	Velocity v = m_getStandardVelocity(whereToGo);
	//cout<<"xVelocity: "<<v.s_xVelocity<<", yVelocity: "<<v.s_yVelocity<<endl;
	float xVelocity = (float) v.s_xVelocity;
	float yVelocity = (float) v.s_yVelocity;
	xVelocity *= howFar;
	yVelocity *= howFar;
	m_setVelocity( (int) xVelocity, (int) yVelocity);
	//cout<<"xVelocity: "<<v.s_xVelocity<<", yVelocity: "<<v.s_yVelocity<<endl;
}

Velocity MovableThing::m_getStandardVelocity(Direction d){//fehlerhaft!
	cout<<"StandardVelocity:"<<m_standardXVelocity<<", "<<m_standardYVelocity<<endl;
	switch (d){
		case DOWN_LEFT:
			return Velocity{- m_standardXVelocity, m_standardYVelocity};
		case DOWN:
				return Velocity{0, m_standardYVelocity};

		case DOWN_RIGHT:
				return Velocity{m_standardXVelocity, m_standardYVelocity};

		case LEFT:
				return Velocity{- m_standardXVelocity, 0};

		case STANDING_STILL:
				return Velocity{0,0};

		case RIGHT:
				return Velocity{m_standardXVelocity,0};

		case UP_LEFT:
				return Velocity{- m_standardXVelocity, - m_standardYVelocity};

		case UP:
				return Velocity{0,- m_standardYVelocity};

		case UP_RIGHT:
				cout<<"UP_RIGHT"<<endl;
				return Velocity{m_standardXVelocity,- m_standardYVelocity};
		default:
			cout<<"Default"<<endl;
			return Velocity{0,0};
		}
}

Coordinate MovableThing::getPosition(){
	return Coordinate(m_x,m_y);
}

void MovableThing::m_add(MovableDrawingElement* newMoDrEl){
	m_howDrawn.push_back(newMoDrEl);
}

unsigned int MovableThing::WhenMoved(){
	return m_whenMoved;
}

/*void MovableThing::setWhenMoved(unsigned int whenMoved){
	if(m_whenMoved > whenMoved){
		cout<<"Wrong update for MovableThing::m_qhenMoved"<<endl;""
		return;
	}
	if(m_whenMoved == whenMoved){
		cout<< "Useless update for MovableThing::m_whenMoved"<<endl;
	}
	else
		m_whenMoved = whenMoved;}
		*/

void MovableThing::m_setDrawingInstructions(int (*foo) (int, int, SDL_Renderer*)){
	for(MovableDrawingElement* currentElement: m_howDrawn){
		cout<<"setDrawingInstructions in MovableThing"<<endl;
		currentElement->m_setAdditionalInstructions(foo);
	}
}

std::list<MovableDrawingElement*>& MovableThing::m_HowDrawn(){
	return m_howDrawn;
}

