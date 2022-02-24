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
	std::list<Drawing*> m_whereToDraw;
public:
	virtual Drawing_Element m_DrawingElement() = 0;
	DrawingElement(const DrawingElement&);
	DrawingElement(int row, int column, SDL_Renderer* renderer, Layer layer = STANDARD_LAYER);
	virtual ~DrawingElement(){}
	virtual int m_draw(SDL_Renderer* renderer = nullptr) = 0;
	Layer m_getLayer();
	void m_setLayer(int layer);
	virtual bool m_equals(DrawingElement& comparedDrEl) = 0;
	bool m_add(Drawing* newDrawing);
	void m_climbToTop(Layer layer);
	void m_climbToTop();
	void m_removeDrawing(Drawing* drawing);
};

class MovableDrawingElement: public DrawingElement{
protected:
	MovableThing* m_content;
public:
	MovableDrawingElement(SDL_Renderer* renderer, MovableThing* movableEntity);
	virtual ~MovableDrawingElement();
	virtual Drawing_Element m_DrawingElement();
	MovableThing* Content();
	virtual bool m_equals(DrawingElement& comparedDrEl) override;
	virtual int m_draw(SDL_Renderer* renderer = nullptr) override;


};

class Drawing: public DrawingElement{
protected:
	std::list<std::shared_ptr<DrawingElement>> m_drawingList;
	void m_updateByLayers();
	virtual int m_drawAtRenderer(SDL_Renderer* renderer);
public:
	Drawing(SDL_Renderer* renderer, int row = 0, int column = 0, Layer layer = STANDARD_LAYER);
	Drawing(SDL_Renderer* renderer, std::list<std::shared_ptr<DrawingElement>> drawingList, int row = 0, int column = 0, Layer layer = STANDARD_LAYER);
	virtual ~Drawing();
	virtual int m_draw(SDL_Renderer* renderer = nullptr) override;
	void m_add(std::shared_ptr<DrawingElement> drawingElement);
	void m_delete(std::shared_ptr<DrawingElement> drElToDelete);
	//zeichne eine Figur nach oben
	void m_putOver(std::shared_ptr<DrawingElement> drElToUpdate, Layer layer);
	bool m_updateLayer(std::shared_ptr<DrawingElement> drElToUpdate, int newLayer);
	void m_setRenderer(SDL_Renderer* renderer);
	virtual bool m_equals(DrawingElement& comparedDrEl);
	virtual Drawing_Element m_DrawingElement();
	void m_add(MovableThing* movableEntity);


};

#endif /* DRAWING_HPP_ */