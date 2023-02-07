/*
 * AppInitter.hpp
 *
 *  Created on: 03.02.2023
 *      Author: uwe-w
 */

#ifndef APPINITTER_HPP_
#define APPINITTER_HPP_

#include "Technologytypes.hpp"
#include <SDL2\SDL_image.h>
#include "drawing.hpp"
#include "EventHandler.hpp"
#include "Window.hpp"
#include "Field.hpp"
#include "GameMain.hpp"
#include "SelectorSurface.hpp"
#include "FieldContainer.hpp"
#include "GameLoader.hpp"
#include <functional>

class AppInitter{
public:
	StartingMode m_requestStartingMode();
	AppInitter();
	void m_init();
	bool m_isAnythingLoadable();
};




#endif /* APPINITTER_HPP_ */
