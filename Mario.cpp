#include "Mario.h"
#include <string>

//constructor
mario::mario(int x){ 
    m_lives = x;
    m_coins = 0;
    m_powerLevel = 0;
}

//destructor
mario::~mario(){

}

//getter
int mario::getLives(){
    return m_lives;
}

//getter
int mario::getCoins(){
    return m_coins;
}

//getter
int mario::getPowerLevel(){
    return m_powerLevel;
}

//setter
void mario::changeCoins(int x){
    this->m_coins = x;
}

//setter
void mario::changeLives(int z){
    this->m_lives = z;
}

//setter
void mario::changePowerLevel(int x){
    this->m_powerLevel = x;
}

//decreases marios lives by one ensuring it never gets negative so the game can make sense
void mario::decrementLives(){
    if (m_lives == 0){
        this->m_lives = 0;
    }
    else{
        this->m_lives = m_lives - 1;
    }
}

//decreases marios power level by one ensuring it never gets negative to make the game make sense - logic for losing lives is handled in play method in map
void mario::decrementPowerLevel(){
    if (m_powerLevel == 0){
        this->m_powerLevel = 0;
    }
    else{
        this->m_powerLevel = m_powerLevel - 1;
    }
}

//increments power level by one ensuring that mario never exceeds 2 keeping consistent with the game's rules
void mario::incrementPowerLevel(){
    if (m_powerLevel == 2){
        this->m_powerLevel = 2;
    }
    else{
        this->m_powerLevel = m_powerLevel + 1;
    }
}

//returns a nicely formatted string of mario's stats 
std::string mario::toString(){
    std::string s = "";
    s += "Mario's Stats";
    s += "\n";
    s += "Lives: ";
    s += std::to_string(this->getLives());
    s += "\n";
    s += "Coin count: ";
    s += std::to_string(this->getCoins());
    s += "\n";
    s += "Power Level(PL): ";
    s += std::to_string(this->getPowerLevel());
    return s;
}