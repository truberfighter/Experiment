/*
 * FightSoundManager.hpp
 *
 *  Created on: 07.03.2023
 *      Author: uwe-w
 */

#ifndef FIGHTSOUNDMANAGER_HPP_
#define FIGHTSOUNDMANAGER_HPP_
#include "SoundEffectContainer.hpp"
#include "Nation.hpp"
enum IsMyNation{NEUTRAL,MY_NATION,FOREIGN_NATION};
class FightSoundManager{
private:
	Nationality m_pointOfView = ROMAN;
	std::unique_ptr<SoundEffectContainer> m_container;
	std::map<IsMyNation, std::map<std::pair<FigureType, FightResult>,std::string>> m_fightResultConfigurations;
public:
	FightSoundManager(Nationality pointOfView);
	void m_makeFightSound(Fight& fight);
	void m_createMap();
	void m_loadMap();
	void m_saveMap();
	virtual ~FightSoundManager();
	IsMyNation m_identify(Nationality who);
};



#ifndef I_DONT_CARE
#define I_DONT_CARE -1
#define NEVER_REPEAT 0
#endif

#endif /* FIGHTSOUNDMANAGER_HPP_ */
