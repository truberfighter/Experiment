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

DrawingElement::~DrawingElement(){
for(auto current: m_whereToDraw){
		//std::cout<<"DrawingElement-Destruktor3, this = "<<this<<std::endl;
		current->m_delete(this);
		//std::cout<<"DrawingElement-Destruktor4, this = "<<this<<",listSize = "<<m_whereToDraw.size()<<std::endl;
		if(m_whereToDraw.size() == 0){
			goto endeDestruktor;
		}
	}
	endeDestruktor: std::cout<<"DrawingElement-Destruktor5, this = "<<this<<std::endl;
}
Drawing::Drawing(SDL_Renderer* renderer, int row, int column, Layer layer)
: DrawingElement(row, column, renderer, layer)
{
	cout<<"Drawingkonstruktor, this ="<<this;
	std::list<std::shared_ptr<DrawingElement>> m_drawingList;
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

void Drawing::m_delete(std::shared_ptr<DrawingElement> drElToDelete){
	m_drawingList.remove(drElToDelete);
}

void Drawing::m_delete(DrawingElement* drElToDelete){
	m_drawingList.remove_if([drElToDelete](std::shared_ptr<DrawingElement> drEl)->bool{return drEl.get()==drElToDelete;});
}


MovableDrawingElement::~MovableDrawingElement(){
	std::cout<<"MovableDrawingElement-Destruktor, this = "<<this<<std::endl;
}

Layer DrawingElement::m_getLayer(){
	return m_layer;
}

void DrawingElement::m_removeDrawing(Drawing* drawing){
	m_whereToDraw.remove(drawing);
	std::cout<<"m_removeDrawing: this = "<<this<<std::endl;
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
	m_rowWhereLastDrawn = xModulo(rowShift + c.x);
	m_columnWhereLastDrawn = columnShift + c.y;
	if(renderer!=nullptr){
		whatToReturn = m_content->m_drawNew(m_rowWhereLastDrawn, m_columnWhereLastDrawn, renderer) ? 1 : 0;
	}
	else{ //renderer==nullptr
		whatToReturn = m_content->m_drawNew(m_rowWhereLastDrawn, m_columnWhereLastDrawn) ? 1 : 0;	}
	if(m_Draw){
		//cout<<"additional instructions called"<<endl;
				if(m_Draw(m_rowWhereLastDrawn, m_columnWhereLastDrawn, renderer)==0)
					whatToReturn = 1;
			}
	//else cout<<"No additional instructions called for this = "<<this<<endl;
	if(whatToReturn != 1)
		cout<<"Probably an SDL_Error: \n"<<SDL_GetError()<<std::endl;
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
	for(Drawing* it: m_whereToDraw) {
		shared_ptr<DrawingElement> sharedFromThis = shared_from_this();
		it->m_putOver(sharedFromThis, layer);
	}
}

void DrawingElement:: m_climbToTop(){
	m_climbToTop(m_layer);
}

void Drawing::m_add(shared_ptr<DrawingElement> drawingElement){
		if(drawingElement->m_DrawingElement() == DRAWING || m_drawingList.empty()){
		}
	if(m_drawingList.empty()){
		m_drawingList.push_back(drawingElement);
}
	else
	m_putOver(drawingElement, drawingElement->m_getLayer());
	if(!drawingElement->m_addDrawing(this)){
		cout<<"Failed to synchronize Drawing and DrawingElement"<<endl;
	}
}

int Drawing::m_drawAtRenderer(SDL_Renderer* renderer, int rowShift, int columnShift){
	/*if(SDL_RenderClear(m_renderer) == 0)
		else
			std::cout<<"SDL_Error: %s\n"<<SDL_GetError()<<std::endl;
			*/
	SDL_RenderPresent(m_renderer);

	int whatToReturn = m_drawingList.size();
	//cout<<"whatToReturn: "<<whatToReturn<<endl;
	for(auto &it: m_drawingList){
		if(it->m_draw(rowShift, columnShift, renderer) == true){
			it->m_updatePosition();
			whatToReturn--;
		}
	}
	return whatToReturn + (
			m_Draw == nullptr ? 0
			: m_Draw(rowShift + m_row, columnShift + m_column, renderer));
}

int Drawing::m_draw(int rowShift, int columnShift, SDL_Renderer* renderer){
	//Handle the drawing status: You do not wanna let some asynchronous drawing happen.
	DrawState tempDrawState = Graphics::m_whatsUpDrawingwise;
	Graphics::m_whatsUpDrawingwise = NORMAL_DRAWING;
	SDL_Renderer* rendererToUse = renderer ? renderer : m_renderer;
	m_rowWhereLastDrawn = rowShift + m_row;
	m_columnWhereLastDrawn = columnShift + m_column;
	int whatToReturn = m_drawAtRenderer(rendererToUse, rowShift + m_row, columnShift + m_column);
	SDL_RenderPresent(rendererToUse);
	Graphics::m_whatsUpDrawingwise = tempDrawState;
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
	//cout<<"putOver"<<drElToUpdate<<std::endl;
	shared_ptr<DrawingElement> a = drElToUpdate;
	//m_drawingList.unique();
//list<DrawingElement*>::iterator whereToErase = m_drawingList.begin();
m_drawingList.remove_if([&drElToUpdate](shared_ptr<DrawingElement> drawEl){bool whatToReturn = drawEl.get() == drElToUpdate.get();  return whatToReturn;});
list<std::shared_ptr<DrawingElement>>::iterator whereToInsert;
for(whereToInsert= m_drawingList.begin(); whereToInsert != m_drawingList.end(); whereToInsert++){
		if((*whereToInsert)->m_getLayer() <= layer){
			continue;
		}
		else{
			break;
		}
	}
m_drawingList.insert(( whereToInsert == m_drawingList.end() ? whereToInsert : ++whereToInsert), drElToUpdate);
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
	fieldToDraw = m_field;
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
 		rect.x = xModulo(m_row+rowShift);
		rect.y = m_column + columnShift;
		rect.h = m_texture->m_Height();
		rect.w = m_texture->m_Width();

		SDL_Texture* textureToDraw = m_texture->theTexture();
		if(!textureToDraw){
			cout<<"No SDL_texture in the texture!"<<endl;
		}
		if(SDL_RenderCopy(renderer, m_texture->theTexture(), NULL, &rect)!=0){
			//cout<<"SDL_Error: %s\n"<<SDL_GetError()<<endl;
			return 0;
		}
		//SDL_RenderPresent(m_renderer);
		//Update data for your instance
		m_renderer = renderer;
		m_rowWhereLastDrawn = rect.x;
		m_columnWhereLastDrawn = rect.y;
		//SDL_Delay(STANDARD_DRAWING_TIME);
		if(m_Draw){
			if(m_Draw(m_rowWhereLastDrawn, m_columnWhereLastDrawn, renderer)==0)
				return 1;
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
	//cout<<"m_setAdditionalInstructions"<<m_Draw<<" for this = "<<this<<endl;
this->m_Draw = *Draw;
}

void DrawingElement::m_drawAsRemembered(SDL_Renderer* renderer){
	SDL_Renderer* rendererToUse = (renderer ? renderer : (m_renderer ? m_renderer : theRenderer));
	m_updatePosition();
	if(int a = m_draw(m_rowWhereLastDrawn - m_row, m_columnWhereLastDrawn - m_column, rendererToUse) != 1){
		std::cout<<"returned value of "<<a<<std::endl;
		throw(DrawingFail("Blinking goes wrong!"));
	}
}

void DrawingElement::m_drawAt(int x,int y,SDL_Renderer* renderer){
	int tempRow = this->m_row;
	int tempColumn = m_column;
	int tempRowWhereLastDrawn = m_rowWhereLastDrawn;
	int tempColumnWhereLastDrawn = m_columnWhereLastDrawn;
	m_row = 0;
	m_column = 0;
	m_rowWhereLastDrawn = 0;
	m_columnWhereLastDrawn = 0;
	m_draw(x,y,renderer);
	this->m_row = tempRow;
	m_column = tempColumn;
	m_rowWhereLastDrawn = tempRowWhereLastDrawn;
	m_columnWhereLastDrawn = tempColumnWhereLastDrawn;
}

