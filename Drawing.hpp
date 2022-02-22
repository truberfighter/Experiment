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





class DrawingElement{
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
	virtual bool m_draw(SDL_Renderer* renderer = nullptr) = 0;
	Layer m_getLayer();
	void m_setLayer(int layer);
	virtual bool m_equals(DrawingElement& comparedElToDr) = 0;
	bool m_add(Drawing* newDrawing);
	void m_climbToTop(Layer layer);
	void m_climbToTop();
};

class MovableDrawingElement: public DrawingElement{
protected:
	MovableThing* m_content;
public:
	MovableDrawingElement(SDL_Renderer* renderer, MovableThing* movableEntity);
	virtual ~MovableDrawingElement();
	virtual Drawing_Element m_DrawingElement();
	MovableThing* Content();
	virtual bool m_equals(DrawingElement& comparedDrEl);
	virtual bool m_draw(SDL_Renderer* renderer = nullptr);


};

class Drawing{
protected:
	SDL_Renderer* m_renderer;
	std::list<DrawingElement*> m_drawingList;
	void m_updateByLayers();
public:
	Drawing(SDL_Renderer* renderer);
	Drawing(SDL_Renderer* renderer, std::list<DrawingElement*> drawingList);
	int m_draw(bool deleteBefore = false);
	void m_add(DrawingElement* drawingElement);
	void m_delete(DrawingElement* drElToDelete);
	//zeichne eine Figur nach oben
	void m_putOver(DrawingElement* drElToUpdate, Layer layer);
	bool m_updateLayer(DrawingElement* drElToUpdate, int newLayer);
	void m_setRenderer(SDL_Renderer* renderer);
};

#endif /* DRAWING_HPP_ */
