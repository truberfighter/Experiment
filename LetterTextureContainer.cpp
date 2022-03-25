/*
 * LetterTextureContainer.cpp
 *
 *  Created on: 24.03.2022
 *      Author: uwe-w
 */

#include "LetterTextureContainer.hpp"
#include "Drawing.hpp"
#include <string>
#include <algorithm>

LetterTextureContainer* theLetterTextureContainer = nullptr;

LetterTexture::LetterTexture(char imageLetter, SDL_Color& color, int size)
:  Texture(nullptr, size, size), m_imageLetter(imageLetter), m_color(color)
{
	char* tempChar = (char*) malloc(sizeof (char) * 1);
	*tempChar = imageLetter;
	SDL_Surface* temp = TTF_RenderText_Solid(theFont, tempChar, m_color);
	m_theTexture = SDL_CreateTextureFromSurface(theRenderer, temp);
	free(tempChar);
}

SDL_Color& LetterTexture::m_Color(){
	return m_color;
}

char LetterTexture::m_ImageLetter(){
	return m_imageLetter;
}

bool LetterTextureContainer::m_generateTexture(char imageLetter, SDL_Color& color, int size){
	if(m_getLetterTexture(imageLetter)!=nullptr)
		return false;
	else
		m_letterTextures.push_back(std::make_shared<LetterTexture>(imageLetter, color, size));
	return *m_letterTextures.end()!=nullptr;
}

std::shared_ptr<LetterTexture> LetterTextureContainer::m_getLetterTexture(char imageLetter){
	for(std::shared_ptr<LetterTexture>& currentTexture: m_letterTextures){
		if(currentTexture->m_ImageLetter()==imageLetter)
			return currentTexture;
	}
	return nullptr;
}

LetterTextureContainer::LetterTextureContainer(std::string& whatToGenerate){
	for(char currentChar: whatToGenerate){
		m_generateTexture(currentChar);
	}
}
