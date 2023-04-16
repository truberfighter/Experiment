/*
 * SoundEffectContainer.hpp
 *
 *  Created on: 02.03.2023
 *      Author: uwe-w
 */

#ifndef SOUNDEFFECTCONTAINER_HPP_
#define SOUNDEFFECTCONTAINER_HPP_

#include "sdltypes.hpp"
#include <map>

class SoundEffect{
public: Mix_Chunk* m_sound;
SoundEffect(Mix_Chunk* chunk):m_sound(chunk){}
~SoundEffect(){
	Mix_FreeChunk(m_sound);
}
};

class SoundEffectContainer{
public:
	std::map<std::string, std::unique_ptr<SoundEffect>> m_content;
	SoundEffectContainer(std::map<std::string, SoundEffect*>& content)
	{for(auto& it: content){
		m_content[it.first]=std::unique_ptr<SoundEffect>(it.second);}}
	SoundEffectContainer(){}
	void m_requestSound(std::string filename);
};


#endif /* SOUNDEFFECTCONTAINER_HPP_ */
