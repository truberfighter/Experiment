/*
 * SoundEffectContainer.cpp
 *
 *  Created on: 30.03.2023
 *      Author: uwe-w
 */

#include "SoundEffectContainer.hpp"
#include "FightSoundManager.hpp"
#include <map>
#include <algorithm>
#include <fstream>
#include "includeJson/single_include/nlohmann/json.hpp"
using Json = nlohmann::json;

FightSoundManager::~FightSoundManager(){
	std::cout<<"Fightsoundmanagerdestruktor"<<std::endl;
}

FightSoundManager::FightSoundManager(Nationality pointOfView):m_pointOfView(pointOfView)
{
	//later, include loading the map
	m_container = std::make_unique<SoundEffectContainer>();
}

void SoundEffectContainer::m_requestSound(std::string filename){
	std::map<std::string, std::unique_ptr<SoundEffect>>::iterator it = m_content.find(filename);
	if(it!=m_content.end()){
		Audio::playSound(it->second.get()->m_sound);
	}
	else{
		m_content[filename]=std::make_unique<SoundEffect>(Audio::loadAudio(filename));
		Audio::playSound((m_content[filename]->m_sound));
	}
}

void FightSoundManager::m_makeFightSound(Fight& fight){
	FigureType winner = (fight.m_result == ATTACKER_LOSES ? fight.defender->m_FigureType() : fight.attacker->m_FigureType());
	std::pair<FigureType, FightResult> key{winner,fight.m_result};
	m_container->m_requestSound(m_fightResultConfigurations[m_identify(fight.m_result == ATTACKER_LOSES ? fight.defender->m_Nationality() : fight.attacker->m_Nationality())][key]);
}

IsMyNation FightSoundManager::m_identify(Nationality who){
	if(m_pointOfView == NO_NATIONALITY){
		return NEUTRAL;
	}
	return who == m_pointOfView ? MY_NATION : FOREIGN_NATION;
}

#define SET(A)  	m_fightResultConfigurations[]

void FightSoundManager::m_createMap(){
	// no point of view
	std::map<std::pair<FigureType,FightResult>,std::string> neutralConfigurations;
	std::map<std::string,std::string> translation = {
			{"ex","Audio/FightSounds/explosion-6055.wav"},
			{"gun","Audio/FightSounds/gunner-sound-43794.wav"},
			{"kar","Audio/FightSounds/karate-chop-6357.wav"},
			{"met","Audio/FightSounds/metal-sound-fighting-game-87507.wav"},
			{"sw","Audio/FightSounds/sword-clash-1-6917.wav"}
	};
	std::vector<FigureType> gunners = {MUSKETEERS,RIFLEMEN,MECH_INF,CRUISER,FRIGATE,IRONCLAD,FIGHTER},
			explosive = {CANNON,CATAPULT,ARMOR,BATTLESHIP,SUBMARINE,ARTILLERY,NUCLEAR,BOMBER};
	for(FigureType currentType = SETTLERS;currentType<=CHARIOT;currentType=(FigureType)(currentType+1)){
		if(std::find(gunners.begin(), gunners.end(), currentType)!=gunners.end()){
			neutralConfigurations[std::pair<FigureType,FightResult>{currentType,ATTACKER_LOSES}]=translation["gun"];
			neutralConfigurations[std::pair<FigureType,FightResult>{currentType,DEFENDER_LOSES}]=translation["gun"];
			neutralConfigurations[std::pair<FigureType,FightResult>{currentType,KAMIKAZE}]=translation["gun"];
			continue;
		}
		if(std::find(explosive.begin(), explosive.end(), currentType)!=explosive.end()){
			neutralConfigurations[std::pair<FigureType,FightResult>{currentType,ATTACKER_LOSES}]=translation["ex"];
			neutralConfigurations[std::pair<FigureType,FightResult>{currentType,DEFENDER_LOSES}]=translation["ex"];
			neutralConfigurations[std::pair<FigureType,FightResult>{currentType,KAMIKAZE}]=translation["ex"];
			continue;
		}
		neutralConfigurations[std::pair<FigureType,FightResult>{currentType,ATTACKER_LOSES}]=translation["kar"];
		neutralConfigurations[std::pair<FigureType,FightResult>{currentType,DEFENDER_LOSES}]=translation["sw"];
		neutralConfigurations[std::pair<FigureType,FightResult>{currentType,KAMIKAZE}]=translation["met"];
	}
	neutralConfigurations[std::pair<FigureType,FightResult>{SUBMARINE,ATTACKER_LOSES}]=translation["gun"];
	neutralConfigurations[std::pair<FigureType,FightResult>{BOMBER,ATTACKER_LOSES}]=translation["gun"];
	m_fightResultConfigurations[NEUTRAL]=neutralConfigurations;
	std::map<std::pair<FigureType,FightResult>,std::string> loserConfigurations;
	for(FigureType currentType = SETTLERS;currentType<=CHARIOT;currentType=(FigureType)(currentType+1)){
		if(std::find(gunners.begin(), gunners.end(), currentType)!=gunners.end()){
			loserConfigurations[std::pair<FigureType,FightResult>{currentType,ATTACKER_LOSES}]=translation["gun"];
			loserConfigurations[std::pair<FigureType,FightResult>{currentType,DEFENDER_LOSES}]=translation["gun"];
			loserConfigurations[std::pair<FigureType,FightResult>{currentType,KAMIKAZE}]=translation["gun"];
			continue;
		}
		if(std::find(explosive.begin(), explosive.end(), currentType)!=explosive.end()){
			loserConfigurations[std::pair<FigureType,FightResult>{currentType,ATTACKER_LOSES}]=translation["ex"];
			loserConfigurations[std::pair<FigureType,FightResult>{currentType,DEFENDER_LOSES}]=translation["ex"];
			loserConfigurations[std::pair<FigureType,FightResult>{currentType,KAMIKAZE}]=translation["ex"];
			continue;
		}
		loserConfigurations[std::pair<FigureType,FightResult>{currentType,ATTACKER_LOSES}]=translation["met"];
		loserConfigurations[std::pair<FigureType,FightResult>{currentType,DEFENDER_LOSES}]=translation["met"];
		loserConfigurations[std::pair<FigureType,FightResult>{currentType,KAMIKAZE}]=translation["met"];
	}
	loserConfigurations[std::pair<FigureType,FightResult>{SUBMARINE,ATTACKER_LOSES}]=translation["gun"];
	loserConfigurations[std::pair<FigureType,FightResult>{BOMBER,ATTACKER_LOSES}]=translation["gun"];
	m_fightResultConfigurations[FOREIGN_NATION]=loserConfigurations;
	std::map<std::pair<FigureType,FightResult>,std::string> winnerConfigurations;
	for(FigureType currentType = SETTLERS;currentType<=CHARIOT;currentType=(FigureType)(currentType+1)){
		if(std::find(gunners.begin(), gunners.end(), currentType)!=gunners.end()){
			winnerConfigurations[std::pair<FigureType,FightResult>{currentType,ATTACKER_LOSES}]=translation["gun"];
			winnerConfigurations[std::pair<FigureType,FightResult>{currentType,DEFENDER_LOSES}]=translation["gun"];
			winnerConfigurations[std::pair<FigureType,FightResult>{currentType,KAMIKAZE}]=translation["gun"];
			continue;
		}
		if(std::find(explosive.begin(), explosive.end(), currentType)!=explosive.end()){
			winnerConfigurations[std::pair<FigureType,FightResult>{currentType,ATTACKER_LOSES}]=translation["ex"];
			winnerConfigurations[std::pair<FigureType,FightResult>{currentType,DEFENDER_LOSES}]=translation["ex"];
			winnerConfigurations[std::pair<FigureType,FightResult>{currentType,KAMIKAZE}]=translation["ex"];
			continue;
		}
		winnerConfigurations[std::pair<FigureType,FightResult>{currentType,ATTACKER_LOSES}]=translation["sw"];
		winnerConfigurations[std::pair<FigureType,FightResult>{currentType,DEFENDER_LOSES}]=translation["sw"];
		winnerConfigurations[std::pair<FigureType,FightResult>{currentType,KAMIKAZE}]=translation["met"];
	}
	winnerConfigurations[std::pair<FigureType,FightResult>{SUBMARINE,ATTACKER_LOSES}]=translation["gun"];
	winnerConfigurations[std::pair<FigureType,FightResult>{BOMBER,ATTACKER_LOSES}]=translation["gun"];
	m_fightResultConfigurations[MY_NATION]=winnerConfigurations;
}

void FightSoundManager::m_saveMap(){
	const std::string filename = "Config/Audio-config/FightSounds.config";
	std::fstream truncator(filename, std::ofstream::out|std::ofstream::trunc);
	truncator.close();
	std::fstream saver(filename);
	Json outputData;
	outputData["hashmapping"]=m_fightResultConfigurations;
	saver<<outputData<<std::endl;
}
