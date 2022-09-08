/*
 * Settlers.cpp
 *
 *  Created on: 08.03.2022
 *      Author: uwe-w
 */
#include "Settlers.hpp"
#include "FieldContainer.hpp"
#include "movableThing.hpp"
#include "GameMain.hpp"
#include "Figure.hpp"
#include "City.hpp"

using namespace std;

Settlers::Settlers(std::shared_ptr<Field> whereToStart,  std::shared_ptr<Nation> nationality
, std::shared_ptr<City> home, bool isVeteran)
: Figure(whereToStart, nationality, home, isVeteran), m_workStepsCount(0)
{
	m_resetMovementPoints();
if(!m_initImage()) cout<<"Fatal error: MovableThing for Settlers not created"<<endl;
cout<<"Settlerskonstruktor"<<theRenderer<<endl;
}


short int Settlers::m_WorkStepsCount(){
	return m_workStepsCount;
}

SettlersWork Settlers::m_CurrentWork(){
	return m_currentWork;
}

void Settlers::m_finishWork(){
	m_workStepsCount = 0;
	m_figureState = DONE_WITH_TURN;
	m_currentWork = NONE;
#ifdef DEBUG
	cout<<"Some Settlers has finished his work"<<endl;
#endif
}

void Settlers::m_work(){
	m_work(m_currentWork);
}

void Settlers::m_work(SettlersWork work){
	m_figureState = SETTLERS_AT_WORK;
	m_currentWork = work;
	m_workStepsCount++;
	m_movementPoints = 0;
	m_nationality->m_removeFromQueue(shared_from_this());
	cout<<"Settlers working on work "<<m_currentWork<<", m_workStepsCount = "<<m_workStepsCount<<", howLongToTake = "<<m_whereItStands->m_howLongToTake(m_currentWork)<< endl;
}

Settlers::~Settlers(){
	std::cout<<"Settlersdestruktor"<<this<<std::endl;
	settlersCount--;
}

bool Settlers::m_takeOrder(char order){
	auto tempWorkCount = m_workStepsCount;
	MovementPoints tempMovementPoints = m_movementPoints;
	try{
	std::cout<<"order taken by settlers: this = "<<this<<", order = "<<(order == ' '? "space" : "char: ")<<order<<std::endl;
	switch(order){
	case 'i': return m_whereItStands->m_Irrigate(*this);
	case 's': return m_sentry();
	case 'r': return m_whereItStands->m_road(*this);
	case 'h': return m_homeCity();
	case 'm': return m_whereItStands->m_Mining(*this);
	case ' ': {m_finishMove(); return true;}
	case 'b': {return (!m_whereItStands->m_CityContained() ? m_startFoundingNewCity() : m_addToCity());}
	default: return false;
	}
}
	catch(SettlersworkUnavailable& theException){
		m_workStepsCount = tempWorkCount;
		m_movementPoints = tempMovementPoints;
		m_nationality->m_addToQueue(shared_from_this());
		return false;
}

}

void Settlers::m_loseOneWorkPoint(){
	if(m_workStepsCount <= 0){
		throw(32435);
	}
	else m_workStepsCount--;
}

FigureCategory Settlers::m_FigureCategory(){
	return GROUND;
}

FigureType Settlers::m_FigureType(){
	return SETTLERS;
}

std::string Settlers::m_orderOverview(){return "H: HomeCity, B: Add to/Found new City, I: Irrigate, M: Mining, S: Sentry, P: Pillage";}

std::shared_ptr<MovableThing> Settlers::m_createImage(){
	int x = m_whereItStands->m_X(), y = m_whereItStands->m_Y();
	//ändere das mit dem filename
	m_image = make_shared<MovableThing>(theRenderer, STANDARD_FIELD_SIZE, STANDARD_FIELD_SIZE, "bilder/Figures/Settlers/RomanRussianSettlers.png", x, y);
	return m_image;
}

int Settlers::m_drawSettlersWork(SDL_Rect& rectToDraw){
#ifdef DRAW_LETTER
#undef DRAW_LETTER
#endif
#define DRAW_LETTER(letter) { \
return SDL_RenderCopy(theRenderer, theLetterTextureContainer->m_getLetterTexture(letter)->theTexture(), nullptr, &rectToDraw);\
}

	switch(m_currentWork){
	case IRRIGATE: DRAW_LETTER('I')
	case MAKE_MINING: DRAW_LETTER('M')
	case BUILD_ROAD: DRAW_LETTER('R')
	default: return 0;
	}
#undef DRAW_LETTER
}

bool Settlers::m_canBuildRailroad(){
	return m_nationality ? m_nationality->m_canBuildRailroad() : false;
}

bool Settlers::m_canBuildBridges(){
	return m_nationality ? m_nationality->m_canBuildBridges() : false;
}

VISIBILITY_OF Settlers IS_NORMAL_VISIBILITY
STRENGTH_OF Settlers WHEN_ATTACKING(1)
STRENGTH_OF Settlers WHEN_DEFENDING(WEAK)
THE_END_OF_TURN_FOR Settlers GOES_UNEVENTFUL
NORMAL_MOVING(Settlers)
SHIELD_COST(Settlers, 40)

bool Settlers::m_startFoundingNewCity(){
	if(m_whereItStands->m_Landscape()==OCEAN){
		return false;
	}
	std::string cityNameSuggestion = "Some city";
	SDL_StartTextInput();
	SDL_SetRenderDrawColor(theRenderer, whiteColor.r, whiteColor.g, whiteColor.b, whiteColor.a);
	bool ready = false;
	SDL_Surface* mainTextSurface = TTF_RenderText_Shaded(theFont, "Enter city name:", blackColor, whiteColor);
	SDL_Texture* mainTextTexture = SDL_CreateTextureFromSurface(theRenderer, mainTextSurface);
	SDL_Rect fillingRect{FIGURE_INFO_WIDTH, (SCREEN_HEIGHT -mainTextSurface->h)/2,mainTextSurface->w,mainTextSurface->h, };
	SDL_RenderCopy(theRenderer, mainTextTexture,NULL,&fillingRect);
	SDL_RenderPresent(theRenderer);
	fillingRect.y+=fillingRect.h;
	SDL_Event currentEvent;
	while(!ready){
		bool renderText = false;
		while(SDL_PollEvent(&currentEvent)!=0){
			if(currentEvent.type == SDL_QUIT){
				throw(SDLQuitException());
			}
			else if(currentEvent.type == SDL_KEYDOWN){
				if(currentEvent.key.keysym.sym==SDLK_BACKSPACE && cityNameSuggestion.length()>0){
					cityNameSuggestion.pop_back();
					renderText = true;
				}
				if(currentEvent.key.keysym.sym==SDLK_ENTER_KEY){
					ready = true;
				}
			}
			else if(currentEvent.key.keysym.sym == SDLK_c && (SDL_GetModState() & KMOD_CTRL) )
				{
					SDL_SetClipboardText( cityNameSuggestion.c_str() );
				}
				//Handle paste
				else if(currentEvent.key.keysym.sym == SDLK_v && (SDL_GetModState() & KMOD_CTRL) )
				{
					cityNameSuggestion = SDL_GetClipboardText();
					renderText = true;
				}
				else if(currentEvent.type == SDL_TEXTINPUT && cityNameSuggestion.length()<MAX_CITY_NAME_LENGTH){
					//Not copy or pasting, taken from lazyfoo
					if( !(( SDL_GetModState() & KMOD_CTRL) && ( currentEvent.text.text[ 0 ] == 'c' || currentEvent.text.text[ 0 ] == 'C' || currentEvent.text.text[ 0 ] == 'v' || currentEvent.text.text[ 0 ] == 'V' ) ) ){
						cityNameSuggestion += currentEvent.text.text;
						renderText = true;
					}
				}
			if(renderText){
				SDL_RenderFillRect(theRenderer, &fillingRect);
				if(cityNameSuggestion != ""){
					SDL_Surface* cityNameSuggestionSurface = TTF_RenderText_Solid(theFont, cityNameSuggestion.c_str(), blackColor);
					SDL_Texture* cityNameSuggestionTexture = SDL_CreateTextureFromSurface(theRenderer, cityNameSuggestionSurface);
					fillingRect.w = cityNameSuggestionSurface->w;
					SDL_RenderFillRect(theRenderer, &fillingRect);
					SDL_RenderCopy(theRenderer,cityNameSuggestionTexture,nullptr,&fillingRect );
					SDL_RenderPresent(theRenderer);
					SDL_DestroyTexture(cityNameSuggestionTexture);
					SDL_FreeSurface(cityNameSuggestionSurface);}
			}
		}
	}
	SDL_DestroyTexture(mainTextTexture);
	SDL_FreeSurface(mainTextSurface);

	SDL_StopTextInput();
	return m_foundNewCity(cityNameSuggestion) != nullptr;
}

std::shared_ptr<City> Settlers::m_foundNewCity(std::string name){
	try{
	m_whereItStands->m_cityContained = std::make_shared<City>(m_whereItStands, m_nationality, name);
	m_nationality->m_destroyFigure(shared_from_this());
	theGame->m_AllCities().push_back(m_whereItStands->m_CityContained());
	m_nationality->m_Cities().push_back(m_whereItStands->m_CityContained());
	std::cout<<"nationCitiesSize: "<<m_nationality->m_Cities().size()<<std::endl;
	if(m_nationality->m_Cities().size()==1){
		m_whereItStands->m_CityContained()->m_improvements.push_back(CityImprovement(PALACE, m_whereItStands->m_CityContained().get()));
		m_nationality->m_setCapitalCity(m_whereItStands->m_CityContained());
	}
	if(m_whereItStands->m_citizenWorking){
		m_whereItStands->m_citizenWorking->m_whereItWorks = nullptr;
		m_whereItStands->m_citizenWorking->m_state = ENTERTAINER;
	}
	if(m_whereItStands->m_roadStatus==NOTHING){
		m_whereItStands->m_roadStatus = ROAD;
	}
	m_whereItStands->m_isIrrigated = false;
	m_whereItStands->m_isMined = false;
	return m_whereItStands->m_CityContained();
	}
	catch(TooManyCities& exception){
		return nullptr;
	}
}

bool Settlers::m_addToCity(){
	if(!m_whereItStands->m_CityContained()){
		throw NullPointerException(std::string("m_addToCity"));
	}
	m_whereItStands->m_CityContained()->m_grow();
	m_nationality->m_destroyFigure(shared_from_this());
	return true;
}
