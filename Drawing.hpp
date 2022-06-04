/*
 * Drawing.hpp
 *
 *  Created on: 15.02.2022
 *      Author: uwe-w
 */

#ifndef DRAWING_HPP_
#define DRAWING_HPP_
#include <SDL2\SDL.h>
#include <iostream>
#include <SDL2\SDL_image.h>
#include "sdltypes.hpp"
#include <functional>
#include <list>
#include "movableThing.hpp"
#include <memory>

class Drawing;
#define NO_INSTRUCTIONS 0
/*class DrawElement{
protected:
	SDL_Renderer* m_renderer;
	Layer m_layer;
	int m_row;
	int m_column;
public:
	DrawElement(){}
	DrawElement(const DrawElement&);
	DrawElement(int row, int column, SDL_Renderer* renderer, Layer layer = STANDARD_LAYER);
	virtual ~DrawElement();
	virtual bool draw(SDL_Renderer* renderer = nullptr) = 0;
	int Layer() const;
	void setLayer(int layer);
	virtual bool equals(DrawElement& comparedDrEl) = 0;
};*/




class DrawingElement: public std::enable_shared_from_this<DrawingElement>{
protected:
	int m_row;
	int m_column;
	SDL_Renderer* m_renderer;
	Layer m_layer;
	int (*m_Draw)(int, int, SDL_Renderer*)=0;
	std::list<Drawing*> m_whereToDraw;
	int m_rowWhereLastDrawn;
	int m_columnWhereLastDrawn;
public:
	friend class MovableThing;
	void m_drawAt(int x, int y, SDL_Renderer* renderer = nullptr);
	virtual Drawing_Element m_DrawingElement() = 0;
	DrawingElement(const DrawingElement&);
	DrawingElement(int row, int column, SDL_Renderer* renderer, Layer layer = STANDARD_LAYER);
	virtual ~DrawingElement();
	virtual int m_draw(int rowShift, int columnShift, SDL_Renderer* renderer = nullptr) = 0;
	Layer m_getLayer();
	void m_setLayer(int layer);
	virtual bool m_equals(DrawingElement& comparedDrEl) = 0;
	bool m_addDrawing(Drawing* newDrawing);
	void m_setAdditionalInstructions(int (*Draw)(int, int, SDL_Renderer*)=NO_INSTRUCTIONS);
	virtual void m_drawAsRemembered(SDL_Renderer* renderer = nullptr);
	void m_climbToTop(Layer layer);
	void m_climbToTop();
	void m_removeDrawing(Drawing* drawing);
	Coordinate m_getPosition(){
		return Coordinate(m_row, m_column);
	}
	virtual bool m_updatePosition()=0;
	SDL_Renderer* m_Renderer();
};

class MovableDrawingElement: public DrawingElement{
protected:
	MovableThing* m_content;
	Figure* m_figure;
public:
	MovableDrawingElement(SDL_Renderer* renderer, MovableThing* movableEntity);
	virtual ~MovableDrawingElement();
	virtual Drawing_Element m_DrawingElement();
	MovableThing* Content();
	virtual bool m_equals(DrawingElement& comparedDrEl) override;
	virtual int m_draw(int rowShift, int columnShift, SDL_Renderer* renderer = nullptr) override;
//	virtual Coordinate m_getPosition() override;
	virtual bool m_updatePosition();
	void m_setFigure(Figure* figure){m_figure = figure;}
	Figure* m_getFigure(){return m_figure;}
	//void m_drawAsRemembered(SDL_Renderer* renderer = nullptr) override;
};

class Drawing: public DrawingElement{
protected:
	std::list<std::shared_ptr<DrawingElement>> m_drawingList;
	void m_updateByLayers();
	virtual int m_drawAtRenderer(SDL_Renderer* renderer, int rowShift = 0, int columnShift = 0);
public:
	Drawing(SDL_Renderer* renderer, int row = 0, int column = 0, Layer layer = STANDARD_LAYER);
	Drawing(SDL_Renderer* renderer, std::list<std::shared_ptr<DrawingElement>>& drawingList, int row = 0, int column = 0, Layer layer = STANDARD_LAYER);
	virtual ~Drawing();
	virtual int m_draw(int rowShift = 0, int columnShift = 0, SDL_Renderer* renderer = nullptr) override;
	void m_add(std::shared_ptr<DrawingElement> drawingElement);
	void m_delete(std::shared_ptr<DrawingElement> drElToDelete);
	void m_delete(DrawingElement* drElToDelete);
	//zeichne eine Figur nach oben
	void m_putOver(std::shared_ptr<DrawingElement>& drElToUpdate, Layer layer);
	bool m_updateLayer(std::shared_ptr<DrawingElement>& drElToUpdate, int newLayer);
	void m_setRenderer(SDL_Renderer* renderer);
	virtual bool m_equals(DrawingElement& comparedDrEl);
	virtual Drawing_Element m_DrawingElement();
	void m_add(MovableThing* movableEntity);
	virtual bool m_updatePosition();
	//virtual Coordinate m_getPosition() override;
};

class ImmovableDrawingElement: public DrawingElement{
protected:
	Field* m_field;
	std::shared_ptr<Texture> m_texture;
public:
	ImmovableDrawingElement (SDL_Renderer* renderer, std::shared_ptr<Texture> texture, int row = 0, int column = 0, Layer layer = STANDARD_FIELD_LAYER);
	virtual ~ImmovableDrawingElement(){}
	virtual bool m_updatePosition(){return false;}
	virtual Drawing_Element m_DrawingElement(){return IMMOVABLE_DRAWING_ELEMENT;}
	virtual bool m_equals(DrawingElement& comparedDrEl){return &comparedDrEl == this;}
	virtual int m_draw(int rowShift = 0, int columnShift = 0, SDL_Renderer* renderer = nullptr) override;
	std::shared_ptr<Texture>& getTexture(){return m_texture;}
	void setTexture(std::shared_ptr<Texture> texture){m_texture = texture;}
	void m_setField(Field* field){m_field = field;}
	Field* m_getField(){return m_field;}
};

class LambdaDrawingElement: public DrawingElement{
protected:
public:
	LambdaDrawingElement(SDL_Renderer* renderer, int(*)(int, int, SDL_Renderer*), int row = 0, int column = 0, Layer layer = STANDARD_FIELD_MODIFICATOR_LAYER);
	virtual ~LambdaDrawingElement();
	virtual bool m_updatePosition(){return false;}
	virtual Drawing_Element m_DrawingElement(){return LAMBDA_DRAWING_ELEMENT;}
	virtual bool m_equals(DrawingElement& comparedDrEl){return this == &comparedDrEl;}
	virtual int m_draw(int rowShift = 0, int columnShift = 0, SDL_Renderer* renderer = nullptr) override{return m_Draw(m_row + rowShift, m_column + columnShift, renderer? renderer : m_renderer);}
};



#endif /* DRAWING_HPP_ */
