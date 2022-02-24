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
	std::list<DrawingElement*> m_drawingList;
}

Drawing::Drawing(SDL_Renderer* renderer,  std::list<shared_ptr<DrawingElement>> drawingList, int row, int column, Layer layer)
: DrawingElement(row, column, renderer, layer), m_drawingList(drawingList){}

DrawingElement::DrawingElement(int row, int column, SDL_Renderer* renderer, Layer layer)
:  m_row(row), m_column(column), m_renderer(renderer), m_layer (layer){}

bool DrawingElement::m_add(Drawing* newDrawing){
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

int MovableDrawingElement::m_draw(SDL_Renderer* renderer){
	if(renderer!=nullptr){
		return m_content->m_drawNew(renderer) ? 1 : 0;
	}
	else{ //renderer==nullptr
		return m_content->m_drawNew() ? 1 : 0;	}
}

Drawing_Element MovableDrawingElement::m_DrawingElement(){
	return MOVABLE_DRAWING_ELEMENT;
}

MovableDrawingElement::MovableDrawingElement(SDL_Renderer* renderer, MovableThing* movableEntity)
: DrawingElement(movableEntity->getPosition().x, movableEntity->getPosition().y, renderer, STANDARD_LAYER), m_content(movableEntity)
{
	movableEntity->m_add(this);
}

void DrawingElement::m_climbToTop(Layer layer){
	cout<<"Dieses DrawingElement ist in so vielen Drawings enthalten: "<<m_whereToDraw.size()<<endl;
	for(Drawing* it: m_whereToDraw) {
		it->m_putOver(shared_from_this(), layer);
	}
}

void DrawingElement:: m_climbToTop(){
	m_climbToTop(m_layer);
}

void Drawing::m_add(shared_ptr<DrawingElement> drawingElement){
	m_putOver(drawingElement, drawingElement->m_getLayer());
	if(!drawingElement->m_add(this)){
		cout<<"Failed to synchronize Drawing and DrawingElement"<<endl;
	}
}

int Drawing::m_drawAtRenderer(SDL_Renderer* renderer){
	if(SDL_RenderClear(m_renderer) == 0)
			cout<<"Renderer is cleared"<<endl;
		else
			std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
	SDL_RenderPresent(m_renderer);

	int whatToReturn = m_drawingList.size();
	cout<<"whatToReturn: "<<whatToReturn<<endl;
	for(auto &it: m_drawingList){
		if(it->m_draw(m_renderer) == true){
			cout<<"Erfolgreiche Zeichnung!"<<endl;
			whatToReturn--;
		}
	}
	return whatToReturn;
}

int Drawing::m_draw(SDL_Renderer* renderer){
	SDL_Renderer* rendererToUse = renderer ? renderer : m_renderer;
	return m_drawAtRenderer(rendererToUse);
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
void Drawing::m_putOver(shared_ptr<DrawingElement> drElToUpdate, Layer layer){
list<std::shared_ptr<DrawingElement>>::iterator whereToInsert = m_drawingList.begin();
//list<DrawingElement*>::iterator whereToErase = m_drawingList.begin();
m_drawingList.remove_if([drElToUpdate](shared_ptr<DrawingElement> drawEl){return &(*drawEl) == &(*drElToUpdate);});
for(; whereToInsert != m_drawingList.end(); whereToInsert++){
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

	/*else{
		m_drawingList.pop_front();
	}*/
	//m_drawingList.insert(whereToInsert, drElToUpdate);

