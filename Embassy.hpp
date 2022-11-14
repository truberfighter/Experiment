/*
 * Embassy.hpp
 *
 *  Created on: 13.11.2022
 *      Author: uwe-w
 */

#ifndef EMBASSY_HPP_
#define EMBASSY_HPP_

#include "Nation.hpp"

//The Embassy class is also designed to be used for F3, if this is ever going to become a thing
class Embassy{
private: std::shared_ptr<Nation> m_nation;
public: Embassy(std::shared_ptr<Nation> whichNation);
std::string m_giveEmbassyInfo();
const std::shared_ptr<Nation> m_Nation();
};



#endif /* EMBASSY_HPP_ */
