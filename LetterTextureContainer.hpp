/*
 * LetterTextureConatiner.hpp
 *
 *  Created on: 24.03.2022
 *      Author: uwe-w
 */

#ifndef LETTERTEXTURECONTAINER_HPP_
#define LETTERTEXTURECONTAINER_HPP_

#include "sdltypes.hpp"
#include <string>

class LetterTexture: public Texture{
private:
	char m_imageLetter;
	SDL_Color& m_color;
public:
	~LetterTexture(){std::cout<<"letter texture destroyed for image letter "<<m_imageLetter<<std::endl;}
	LetterTexture(char imageLetter, SDL_Color& color = whiteColor, int size = FIGURESTATE_TEXTURE_WIDTH);
	SDL_Color& m_Color();
	char m_ImageLetter();
};

class LetterTextureContainer{
private:
	std::vector<std::shared_ptr<LetterTexture>> m_letterTextures;
public:
	bool m_generateTexture(char imageLetter, SDL_Color& color = whiteColor, int size = FIGURESTATE_TEXTURE_WIDTH);
	LetterTextureContainer(std::string& whatToGenerate);
	std::shared_ptr<LetterTexture> m_getLetterTexture(char imageLetter);
};

extern LetterTextureContainer* theLetterTextureContainer;

#endif /* LETTERTEXTURECONATINER_HPP_ */
