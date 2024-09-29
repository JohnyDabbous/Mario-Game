#ifndef MARIO_H
#define MARIO_H
#include <string>

class mario {
    public:
        mario(int x);
        ~mario();
        int getLives();
        int getCoins();
        int getPowerLevel();
        void changeLives(int x);
        void changeCoins(int y);
        void changePowerLevel(int z);
        bool attack();
        int fight();
        std::string toString();
        void decrementLives();
        void decrementPowerLevel();
        void incrementPowerLevel();
    private:
        int m_lives;
        int m_coins;
        int m_powerLevel;


};
#endif