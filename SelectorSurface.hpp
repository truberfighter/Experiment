/*
 * SelectorSurface.hpp
 *
 *  Created on: 21.06.2022
 *      Author: uwe-w
 */

#ifndef SELECTORSURFACE_HPP_
#define SELECTORSURFACE_HPP_
#include "sdltypes.hpp"
#include "functional"
#include "EventHandler.hpp"

struct SelectionReturn{
	std::string content;
	unsigned int index;
	Layer layer;
};

extern int selectionElementGlobalIndex;

class SelectionElement{
public:
	std::string content;
	int layer;
	std::function<void()> rightClickOrders;
	SelectionElement(std::string c, int l, std::function<void()>& f): content(c), layer(l), rightClickOrders(f){ /*layer = selectionElementGlobalIndex++;*/	}
	~SelectionElement(){std::cout<<"SelectionElementDestruktor: this = "<<this<<", content: "<<content<<", layer: "<<layer<<std::endl;}
};

#ifndef QUITSELECTION_
#define QUITSELECTION_
enum SelectionQuit{NO_ACTION, SELECTION, NEXT_PAGE};
struct InvalidSelectionElement{
	std::shared_ptr<SelectionElement> what;
};
class QuitSelection: public QuitSurface{
public:
	SelectionQuit m_returnSomething;
	QuitSelection(SelectionQuit m):m_returnSomething(m){}
};
#endif

class SelectorSurface: public EventHandler{
private:
	const int elementsPerUnit = (SCREEN_HEIGHT/NORMAL_FONT_HEIGHT) - 1;
	void m_drawSelection();
	bool m_inCenter;
	int m_x;
	int m_y;
	int maxHeightNormed = 0;
	int HowMuchToDraw;
	int maxWidth = 0;
	SDL_Color m_fontColor;
	SDL_Color m_markedColor;
	SDL_Color m_standardColor;
	std::vector<std::shared_ptr<SelectionElement>>& m_whatToAskFor;
	std::vector<int> endIndexes{0};
	int endIndexesIndex = 0;
	int currentRelativeIndex = 0;
	std::function<void()> m_backgroundOrders;
public:
	QuitSelection m_finalSelection();
	int m_effectiveIndex();
	void m_goUp();
	void m_goDown();
	bool m_select(int x, int y);
	SelectionReturn m_return();
	virtual ~SelectorSurface(){std::cout<<"SelectorSurface-Destruktor"<<std::endl;selectionElementGlobalIndex=0;}
	 bool m_handleLeftClick(const SDL_MouseButtonEvent& event);
	virtual bool m_handleKeyboardEvent(const SDL_Event& event);
	virtual bool m_handleEvent(const SDL_Event& event);
	SelectorSurface(int x, int y, std::vector<std::shared_ptr<SelectionElement>>& whatToAskFor, bool inCenter = false, SDL_Color standardColor = Graphics::Civ::shieldGreyColor(), SDL_Color fontColor = blackColor, SDL_Color markedColor = Graphics::Civ::brightCityBackgroundColor(), std::function<void()> backgroundOrders = [](){});
	SelectionReturn m_fetchSelection();
};



#endif /* SELECTORSURFACE_HPP_ */
