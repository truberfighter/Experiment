/*
 * Drawing.cpp
 *
 *  Created on: 18.02.2022
 *      Author: uwe-w
 */

#include <SDL2\SDL.h>
#include <iostream>
#include <SDL2\SDL_image.h>
#include "sdltypes.hpp"
#include <functional>
#include <list>
#include "Drawing.hpp"
#include "movableThing.hpp"
#include <memory>

using namespace std;

Drawing::Drawing(SDL_Renderer* renderer, int row, int column, Layer layer)
: DrawingElement(row, column, renderer, layer)
{
	cout<<"Drawingkonstruktor, this ="<<this;
	std::list<DrawingElement*> m_drawingList;
}

Drawing::Drawing(SDL_Renderer* renderer,  std::list<shared_ptr<DrawingElement>>& drawingList, int row, int column, Layer layer)
: DrawingElement(row, column, renderer, layer), m_drawingList(drawingList){}

DrawingElement::DrawingElement(int row, int column, SDL_Renderer* renderer, Layer layer)
:  m_row(row% (STANDARD_FIELD_SIZE * WORLD_LENGTH)), m_column(column% (STANDARD_FIELD_SIZE * WORLD_HEIGHT)), m_renderer(renderer), m_layer (layer), enable_shared_from_this(){}

SDL_Renderer* DrawingElement::m_Renderer(){
	return m_renderer;
}

bool DrawingElement::m_addDrawing(Drawing* newDrawing){
	//Noch keine Fehlermeldungen, doch zur Sicherheit mal bool!
	//list<Drawing*> newList = m_whereToDraw;
	m_whereToDraw.push_back(newDrawing);
	m_whereToDraw.unique();
	return true;
}


MovableDrawingElement::~MovableDrawingElement(){
	delete m_content;
}

Layer DrawingElement::m_getLayer(){
	return m_layer;
}

bool MovableDrawingElement::m_equals(DrawingElement& comparedDrEl){
	if(m_DrawingElement()!=comparedDrEl.m_DrawingElement()){
		return false;
	}
	return m_content == reinterpret_cast<MovableDrawingElement*>(&comparedDrEl)->m_content;
}

int MovableDrawingElement::m_draw(int rowShift, int columnShift, SDL_Renderer* renderer){
	figureToDraw = m_figure;
	Coordinate c = m_content->getPosition();
	int whatToReturn = 0;
	if(renderer!=nullptr){
		whatToReturn = m_content->m_drawNew(rowShift + c.x, columnShift + c.y, renderer) ? 1 : 0;
	}
	else{ //renderer==nullptr
		whatToReturn = m_content->m_drawNew(rowShift + c.x,columnShift + c.y) ? 1 : 0;	}
	if(m_Draw){
		cout<<"additional instructions called"<<endl;
				if(m_Draw(rowShift + c.x, columnShift + c.y, renderer)==0)
					whatToReturn = 1;
			}
	else cout<<"No additional instructions called for this = "<<this<<endl;
	return whatToReturn;
}

Drawing_Element MovableDrawingElement::m_DrawingElement(){
	return MOVABLE_DRAWING_ELEMENT;
}

MovableDrawingElement::MovableDrawingElement(SDL_Renderer* renderer, MovableThing* movableEntity)
: DrawingElement(movableEntity->getPosition().x, movableEntity->getPosition().y, renderer, STANDARD_LAYER), m_content(movableEntity)
{
	movableEntity->m_add(this);
}

bool MovableDrawingElement::m_updatePosition(){
	Coordinate c = m_content->getPosition();
	m_row = c.x;
	m_column = c.y;
	return true;
}

void DrawingElement::m_climbToTop(Layer layer){
	cout<<"Dieses DrawingElement ist in so vielen Drawings enthalten: "<<m_whereToDraw.size()<<endl;
	for(Drawing* it: m_whereToDraw) {
		shared_ptr<DrawingElement> sharedFromThis = shared_from_this();
		it->m_putOver(sharedFromThis, layer);
	}
}

void DrawingElement:: m_climbToTop(){
	m_climbToTop(m_layer);
}

void Drawing::m_add(shared_ptr<DrawingElement>& drawingElement){
//cout<<"Z.97"<<endl;
	m_putOver(drawingElement, drawingElement->m_getLayer());
	if(!drawingElement->m_addDrawing(this)){
		cout<<"Failed to synchronize Drawing and DrawingElement"<<endl;
	}
}

int Drawing::m_drawAtRenderer(SDL_Renderer* renderer, int rowShift, int columnShift){
	/*if(SDL_RenderClear(m_renderer) == 0)
			cout<<"Renderer is cleared"<<endl;
		else
			std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
			*/
	SDL_RenderPresent(m_renderer);

	int whatToReturn = m_drawingList.size();
	//cout<<"whatToReturn: "<<whatToReturn<<endl;
	for(auto &it: m_drawingList){
		if(it->m_draw(rowShift, columnShift, renderer) == true){
			//cout<<"Erfolgreiche Zeichnung!"<<endl;
			whatToReturn--;
		}
	}
	return whatToReturn + (
			m_Draw == nullptr ? 0
			: m_Draw(rowShift + m_row, columnShift + m_column, renderer));
}

int Drawing::m_draw(int rowShift, int columnShift, SDL_Renderer* renderer){
	SDL_Renderer* rendererToUse = renderer ? renderer : m_renderer;
	int whatToReturn = m_drawAtRenderer(rendererToUse, rowShift, columnShift);
	SDL_RenderPresent(rendererToUse);
	return whatToReturn;
}


void Drawing::m_setRenderer(SDL_Renderer* renderer){
	if(renderer == nullptr){
		cout<<"Do not set Drawing::m_renderer to a nullptr"<<endl;
		return;
	}
	m_renderer = renderer;
}
//Fall 1: alles ok
//Fall 2: nicht drin
//Fall 3: muss nach ganz hinten
void Drawing::m_putOver(shared_ptr<DrawingElement>& drElToUpdate, Layer layer){
	shared_ptr<DrawingElement> a = drElToUpdate;
	//m_drawingList.unique();
//list<DrawingElement*>::iterator whereToErase = m_drawingList.begin();
m_drawingList.remove_if([&drElToUpdate](shared_ptr<DrawingElement> drawEl){bool whatToReturn = &(*drawEl) == &(*drElToUpdate); return whatToReturn;});
list<std::shared_ptr<DrawingElement>>::iterator whereToInsert;
for(whereToInsert= m_drawingList.begin(); whereToInsert != m_drawingList.end(); whereToInsert++){
		if((*whereToInsert)->m_getLayer() <= drElToUpdate->m_getLayer()){

			continue;
		}
		else{
			break;
		}
	}
m_drawingList.insert(whereToInsert, drElToUpdate);
}

Drawing::~Drawing(){
	cout<<"Drawing-Destruktor, this = "<<this<<endl;
}

Drawing_Element Drawing::m_DrawingElement(){
	return DRAWING;
}

void Drawing::m_add(MovableThing* movableEntity){
	shared_ptr<DrawingElement> temp = make_shared<MovableDrawingElement>(m_renderer, movableEntity);
	m_add(temp);
}

bool Drawing::m_equals(DrawingElement& comparedDrEl){
	if(m_DrawingElement() != DRAWING){
		return false;
	}
	Drawing* pointerToWorkWith = dynamic_cast<Drawing*> (&comparedDrEl);
	//Zur Sicherheit.
	if(!pointerToWorkWith)
		return false;
	if((pointerToWorkWith->m_drawingList).size() != m_drawingList.size())
		return false;
	std::list<shared_ptr<DrawingElement>>::iterator it1 = pointerToWorkWith->m_drawingList.end();
	for(std::list<shared_ptr<DrawingElement>>::iterator it2 = m_drawingList.begin(); it2 != m_drawingList.end(); it2++){
		if(*it2 != *it1)
			return false;
	}
	//Wenn alle Komponenten übereinstimmen
	return true;
}

bool Drawing::m_updatePosition(){
	return false;
}

int ImmovableDrawingElement::m_draw(int rowShift, int columnShift, SDL_Renderer* renderer ){
	m_updatePosition();
	if(!renderer){
			cout<<"No valid renderer for ImmovableDrawingElement"<<endl;
			return 0;
		}
		SDL_Rect rect;
		if(!m_texture){
					//cout<<"No valid texture in ImmovableDrawingElement to draw"<<endl;
					return 0;
				}
		//m_texture->m_Height();
		//cout<<"m_texture ist schonmal vorhanden in ImmovableDrawingElement to draw"<<endl;
 		rect.x = xModulo(m_row+rowShift);
		rect.y = yModulo(m_column + columnShift);
		//cout<<"Guenther"<<m_texture->m_Height()<<endl;
		rect.h = m_texture->m_Height();
		rect.w = m_texture->m_Width();
		//cout<<"ImmovableDrawingElement.\n x: "<<rect.x<<", y: "<<rect.y<<"Height: "<<rect.h<<"Width: "<<rect.w<<endl;

		SDL_Texture* textureToDraw = m_texture->theTexture();
		if(!textureToDraw){
			cout<<"No SDL_texture in the texture!"<<endl;
		}
		if(SDL_RenderCopy(renderer, m_texture->theTexture(), NULL, &rect)!=0){
			cout<<"SDL_Error: %s\n"<<SDL_GetError()<<endl;
			return 0;
		}
		//SDL_RenderPresent(m_renderer);
		//Update data for your instance
		m_renderer = renderer;
		//SDL_Delay(STANDARD_DRAWING_TIME);
		if(m_Draw){
			if(m_Draw(rowShift + m_row, columnShift + m_column, renderer)==0)
				return 0;
		}
		return 1;

}

	/*else{
		m_drawingList.pop_front();
	}*/
	//m_drawingList.insert(whereToInsert, drElToUpdate);


ImmovableDrawingElement::ImmovableDrawingElement(SDL_Renderer* renderer, std::shared_ptr<Texture> texture, int row, int column, Layer layer)
: DrawingElement(row, column, renderer, layer), m_texture(texture)
{
}

LambdaDrawingElement::LambdaDrawingElement(SDL_Renderer* renderer, int draw(int, int, SDL_Renderer* renderer), int row, int column, Layer layer)
: DrawingElement(row, column, renderer, layer)
{
	m_Draw = draw;
}

LambdaDrawingElement::~LambdaDrawingElement(){
}

void DrawingElement::m_setAdditionalInstructions(int (*Draw)(int, int, SDL_Renderer*)){
	cout<<"m_setAdditionalInstructions"<<m_Draw<<" for this = "<<this<<endl;
this->m_Draw = *Draw;
	m_draw(0,0,theRenderer);
	SDL_Delay(2000);
}


