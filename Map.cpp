#include <cstdlib> 
#include <iostream>
#include "Map.h"
#include <fstream>
#include <string>
#include "Mario.h"

//constructor
map::map(int amountOfLevels, int grid){
    levels = amountOfLevels;
    dimensions = grid;
    m_map = new char**[levels];
    for (int i = 0; i < levels; ++i){
        m_map[i] = new char*[dimensions];
        for(int j = 0; j < dimensions; ++j){
            m_map[i][j] = new char[dimensions];
        }
    }
}

//destructor
map::~map(){
    for (int i = 0; i < levels; ++i) {
        for (int j = 0; j < dimensions; ++j) {
            delete[] m_map[i][j];
        }
        delete[] m_map[i];
    }
    delete[] m_map;
}

//generates the map with the given percentages
void map::generateMap(int coinPerc, int goombasPerc, int koopaPerc, int shroomsPerc, int nothingPerc){
    char interactables[] = {'x', 'c', 'm', 'g', 'k'};
    int mapSize = (dimensions * dimensions) * levels; //used for calculating percents
    int nothing = int((nothingPerc/100.0f) * mapSize);
    int coins = int((coinPerc/100.0f) * mapSize);
    int goombas = int((goombasPerc/100.0f) * mapSize); 
    int koopas = int((koopaPerc/100.0f) * mapSize);
    int shrooms = int((shroomsPerc/100.0f) * mapSize);
    while (nothing > 0){ //percents
        m_map[rand() % levels][rand() % dimensions][rand() % dimensions] = 'x';
        --nothing;
    }
    while (coins){ //percents
        m_map[rand() % levels][rand() % dimensions][rand() % dimensions] = 'c';
        --coins;
    }   
    while (goombas){ //percents
        m_map[rand() % levels][rand() % dimensions][rand() % dimensions] = 'g';
        --goombas;
    }
    while (koopas){ //percents
        m_map[rand() % levels][rand() % dimensions][rand() % dimensions] = 'k';
        --koopas;
    }
    while (shrooms){ //percents
        m_map[rand() % levels][rand() % dimensions][rand() % dimensions] = 'm';
        --shrooms;
    }
    for (int t = 0; t < levels; ++t){
        for (int q = 0; q < dimensions; ++q){
            for (int h = 0; h < dimensions; ++h){
                m_map[t][q][h] = interactables[rand() % 5];
                if (m_map[t][q][h] == NULL){
                    m_map[t][q][h] = interactables[rand() % 5];
                }
            }
        }
    }
    char boss = 'b';
    char warpPipe = 'w';
    int q = 0;
    while (q < levels){
        m_map[q][rand() % dimensions][rand() % dimensions] = boss;
        if (q == (levels - 1)){
            break;
        }
        else{
            m_map[q][rand() % dimensions][rand() % dimensions] = warpPipe;
        }
        ++q;
    }
}

//Like a toString for mario's location
std::string map::getMarioLocation(char** matrix){ 
    std::string s;
    for (int x = 0; x < dimensions; ++x){
        for (int t = 0; t < dimensions; ++t){
            if (matrix[x][t] == 'H'){
                s = "Mario is at the point " + std::to_string(x + 1) + "," + std::to_string(t + 1);
                return s;
            }
            
        }
    }
}

//returns mario's x coordinate
int map::getMarioRow(char** matrix){ //Used in play
    for (int x = 0; x < dimensions; ++x){
        for (int t = 0; t < dimensions; ++t){
            if (matrix[x][t] == 'H'){
                return x;
            }    
        }
    }
}

//returns mario's y coordinate
int map::getMarioColumn(char** matrix){
    for (int x = 0; x < dimensions; ++x){
        for (int t = 0; t < dimensions; ++t){
            if (matrix[x][t] == 'H'){
                return t;
            }    
        }
    }
}

//game simulation
void map::play(std::string file, int x){
    std::ofstream game(file); //writes game out to file
    mario player(x);
    char movements[] = {'W', 'a', 's', 'd'};
    bool marioAlive = true;
    bool levelComplete;
    int getLevel = 0;
    while (getLevel < levels){
        m_map[getLevel][rand() % dimensions][rand() % dimensions]= 'H';
         //this for loop generates a new level after
        char** levelArr = new char*[dimensions];
        for(int i = 0; i < dimensions; ++i) {
            levelArr[i] = new char[dimensions]; 
        }
        for(int t = 0; t < dimensions; ++t) {
            for(int s = 0; s < dimensions; ++s) {
                levelArr[t][s] = m_map[getLevel][t][s];
            }
        }
        levelComplete = false; //sets level complete to false after each new level is generated
        while (!(levelComplete) && marioAlive){
            char move = movements[rand() % 4]; //randomly generates a movement from the movements array
            game << "Level: " << std::to_string(getLevel + 1) << std::endl;
            for (int a = 0; a < dimensions; ++a){ //prints level contents out to file 
                for (int r = 0; r < dimensions; ++r){
                    game << levelArr[a][r] << " ";
                }
                game << std::endl;
            }
            game << getMarioLocation(levelArr) << std::endl; //prints mario's location to file
            game << "===============================" << std::endl; //for formatting purposes
            switch (move){
                //up movement
                case 'W':{
                    int oldRow = getMarioRow(levelArr); //storing variable
                    int oldColumn = getMarioColumn(levelArr); //storing variable
                    int newRow = getMarioRow(levelArr) - 1; //storing variable
                    int newCol = getMarioColumn(levelArr); //storing variable
                    if (newRow < dimensions && newRow >= 0){ //checks to see if mario is out of bounds
                        game << "Mario will move up." << std::endl;
                        char tempChar = levelArr[newRow][newCol];
                        levelArr[oldRow][oldColumn] = 'x'; //replaces marios old location with a nothing square
                        levelArr[newRow][newCol] = 'H'; //updates mario's current location on the map
                        switch (tempChar){
                            case 'x':{
                                game << "You landed on a nothing tile." << std::endl;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'c':{
                                game << "You landed on a coin tile." << std::endl;
                                if (player.getCoins() == 20){
                                    player.changeLives(player.getLives() + 1);
                                    game << "You got enough coins for a life." << std::endl;
                                    player.changeCoins(0);
                                    game << "Your coins have now reset." << std::endl;
                                }
                                else{
                                    player.changeCoins(player.getCoins() + 1);
                                    game << "Your coin count increased by 1." << std::endl;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'm':{
                                game << "You landed on a mushroom tile." << std::endl;
                                if (player.getPowerLevel() == 2){
                                    game << "Your power level did not change." << std::endl;
                                }
                                else{
                                    player.incrementPowerLevel();
                                    game << "You power level grew by one." << std::endl;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'g':{
                                game << "You are about to fight a goomba. Good luck." << std::endl;
                                int goo = rand() % 21;
                                int you = rand() % 81;
                                if (goo > you){
                                    game << "You lost to the goomba." << std::endl;
                                    player.decrementPowerLevel();
                                    if (player.getPowerLevel() ==  0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        if (player.getLives() == 0){
                                            marioAlive = false;
                                        }
                                    }
                                }
                                else{
                                    game << "You beat the goomba." << std::endl;
                                    break;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'k':{
                                game << "You are about to fight a koopa. Good luck." << std::endl;
                                int koo = rand() % 36;
                                int yo = rand() % 66;
                                if (koo > yo){
                                    game << "You lost to the koopa." << std::endl;
                                    player.decrementPowerLevel();
                                    if (player.getPowerLevel() ==  0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        if (player.getLives() == 0){
                                            marioAlive = false;
                                        }
                                    }
                                }
                                else{
                                    game << "You beat the koopa." << std::endl;
                                    break;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'b':{
                                game << "You are about to fight the boss. Good luck." << std::endl;
                                int bo = 0;
                                int yourNu = 0;
                                while (bo > yourNu){
                                    bo = rand() % 51;
                                    yourNu = rand() % 51;
                                    if (player.getPowerLevel() == 0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        player.changePowerLevel(0);
                                        if (player.getLives() == 0){
                                            game << "You lost to the boss." << std::endl;
                                            marioAlive = false;
                                        }
                                    }
                                }
                                game << "You beat the boss." << std::endl;
                                levelComplete = true;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'w':{
                                game << "You found the warp pipe. You move on to the next level." << std::endl;
                                levelComplete = true;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'H':{
                                break;
                            }
                        }
                    }
                    //prevents mario from moving out of bounds
                    else{
                        levelArr[oldRow][oldColumn] = 'H';
                        game << "Mario cannot move up." << std::endl;
                    }
                    break;
                }
                //left movement - same logic as W 
                case 'a': {
                    int oldRow = getMarioRow(levelArr);
                    int oldColumn = getMarioColumn(levelArr);
                    int newRow = getMarioRow(levelArr);
                    int newCol = getMarioColumn(levelArr) - 1;
                    if (newCol >= 0 && newCol < dimensions){
                        game << "Mario will move left." << std::endl;
                        char tempChar = levelArr[newRow][newCol];
                        levelArr[oldRow][oldColumn] = 'x';
                        levelArr[newRow][newCol] = 'H';
                        switch (tempChar){
                            case 'x':{
                                game << "You landed on a nothing tile." << std::endl;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'c':{
                                game << "You landed on a coin tile." << std::endl;
                                if (player.getCoins() == 20){
                                    player.changeLives(player.getLives() + 1);
                                    game << "You got enough coins for a life." << std::endl;
                                    player.changeCoins(0);
                                    game << "Your coins have now reset." << std::endl;
                                }
                                else{
                                    player.changeCoins(player.getCoins() + 1);
                                    game << "Your coin count increased by 1." << std::endl;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'm':{
                                game << "You landed on a mushroom tile." << std::endl;
                                if (player.getPowerLevel() == 2){
                                    game << "Your power level did not change." << std::endl;
                                }
                                else{
                                    player.incrementPowerLevel();
                                    game << "You power level grew by one." << std::endl;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'g':{
                                game << "You are about to fight a goomba. Good luck." << std::endl;
                                int goo = rand() % 21;
                                int you = rand() % 81;
                                if (goo > you){
                                    game << "You lost to the goomba." << std::endl;
                                    player.decrementPowerLevel();
                                    if (player.getPowerLevel() ==  0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        if (player.getLives() == 0){
                                            marioAlive = false;
                                        }
                                    }
                                }
                                else{
                                    game << "You beat the goomba." << std::endl;
                                    break;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'k':{
                                game << "You are about to fight a koopa. Good luck." << std::endl;
                                int koo = rand() % 36;
                                int yo = rand() % 66;
                                if (koo > yo){
                                    game << "You lost to the koopa." << std::endl;
                                    player.decrementPowerLevel();
                                    if (player.getPowerLevel() ==  0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        if (player.getLives() == 0){
                                            marioAlive = false;
                                        }
                                    }
                                }
                                else{
                                    game << "You beat the koopa." << std::endl;
                                    break;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'b':{
                                game << "You are about to fight the boss. Good luck." << std::endl;
                                int bo = 0;
                                int yourNu = 0;
                                while (bo > yourNu){
                                    bo = rand() % 51;
                                    yourNu = rand() % 51;
                                    if (player.getPowerLevel() == 0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        player.changePowerLevel(0);
                                        if (player.getLives() == 0){
                                            game << "You lost to the boss." << std::endl;
                                            marioAlive = false;
                                        }
                                    }
                                }
                                game << "You beat the boss." << std::endl;
                                levelComplete = true;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'w':{
                                game << "You found the warp pipe. You move on to the next level." << std::endl;
                                levelComplete = true;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'H':{
                                break;
                            }
                        }
                    }
                    else{
                        levelArr[oldRow][oldColumn] = 'H';
                        game << "Mario cannot move left." << std::endl;
                    }
                    break;
                }
                //down movement - same logic as W and a
                case 's': {
                    int oldRow = getMarioRow(levelArr);
                    int oldColumn = getMarioColumn(levelArr);
                    int newRow = getMarioRow(levelArr) + 1;
                    int newCol = getMarioColumn(levelArr);
                    if (newRow < dimensions){
                        game << "Mario will move down." << std::endl;
                        char tempChar = levelArr[newRow][newCol];
                        levelArr[oldRow][oldColumn] = 'x';
                        levelArr[newRow][newCol] = 'H';
                        switch (tempChar){
                            case 'x':{
                                game << "You landed on a nothing tile." << std::endl;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'c':{
                                game << "You landed on a coin tile." << std::endl;
                                if (player.getCoins() == 20){
                                    player.changeLives(player.getLives() + 1);
                                    game << "You got enough coins for a life." << std::endl;
                                    player.changeCoins(0);
                                    game << "Your coins have now reset." << std::endl;
                                }
                                else{
                                    player.changeCoins(player.getCoins() + 1);
                                    game << "Your coin count increased by 1." << std::endl;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'm':{
                                game << "You landed on a mushroom tile." << std::endl;
                                if (player.getPowerLevel() == 2){
                                    game << "Your power level did not change." << std::endl;
                                }
                                else{
                                    player.incrementPowerLevel();
                                    game << "You power level grew by one." << std::endl;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'g':{
                                game << "You are about to fight a goomba. Good luck." << std::endl;
                                int goo = rand() % 21;
                                int you = rand() % 81;
                                if (goo > you){
                                    game << "You lost to the goomba." << std::endl;
                                    player.decrementPowerLevel();
                                    if (player.getPowerLevel() ==  0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        if (player.getLives() == 0){
                                            marioAlive = false;
                                        }
                                    }
                                }
                                else{
                                    game << "You beat the goomba." << std::endl;
                                    break;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'k':{
                                game << "You are about to fight a koopa. Good luck." << std::endl;
                                int koo = rand() % 36;
                                int yo = rand() % 66;
                                if (koo > yo){
                                    game << "You lost to the koopa." << std::endl;
                                    player.decrementPowerLevel();
                                    if (player.getPowerLevel() ==  0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        if (player.getLives() == 0){
                                            marioAlive = false;
                                        }
                                    }
                                }
                                else{
                                    game << "You beat the koopa." << std::endl;
                                    break;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'b':{
                                game << "You are about to fight the boss. Good luck." << std::endl;
                                int bo = 0;
                                int yourNu = 0;
                                while (bo > yourNu){
                                    bo = rand() % 51;
                                    yourNu = rand() % 51;
                                    if (player.getPowerLevel() == 0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        player.changePowerLevel(0);
                                        if (player.getLives() == 0){
                                            game << "You lost to the boss." << std::endl;
                                            marioAlive = false;
                                        }
                                    }
                                }
                                game << "You beat the boss." << std::endl;
                                levelComplete = true;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'w':{
                                game << "You found the warp pipe. You move on to the next level." << std::endl;
                                levelComplete = true;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'H':{
                                break;
                            }
                        }
                    }
                    else{
                        levelArr[oldRow][oldColumn] = 'H';
                        game << "Mario cannot move down." << std::endl;
                    }
                    break;
                }
                //right movement - same logic as W, a, and s
                case 'd': {
                    int oldRow = getMarioRow(levelArr);
                    int oldColumn = getMarioColumn(levelArr);
                    int newRow = getMarioRow(levelArr);
                    int newCol = getMarioColumn(levelArr) + 1;
                    if (newCol < dimensions){
                        game << "Mario will move right." << std::endl;
                        char tempChar = levelArr[newRow][newCol];
                        levelArr[oldRow][oldColumn] = 'x';
                        levelArr[newRow][newCol] = 'H';
                        switch (tempChar){
                            case 'x':{
                                game << "You landed on a nothing tile." << std::endl;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'c':{
                                game << "You landed on a coin tile." << std::endl;
                                if (player.getCoins() == 20){
                                    player.changeLives(player.getLives() + 1);
                                    game << "You got enough coins for a life." << std::endl;
                                    player.changeCoins(0);
                                    game << "Your coins have now reset." << std::endl;
                                }
                                else{
                                    player.changeCoins(player.getCoins() + 1);
                                    game << "Your coin count increased by 1." << std::endl;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'm':{
                                game << "You landed on a mushroom tile." << std::endl;
                                if (player.getPowerLevel() == 2){
                                    game << "Your power level did not change." << std::endl;
                                }
                                else{
                                    player.incrementPowerLevel();
                                    game << "You power level grew by one." << std::endl;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'g':{
                                game << "You are about to fight a goomba. Good luck." << std::endl;
                                int goo = rand() % 21;
                                int you = rand() % 81;
                                if (goo > you){
                                    game << "You lost to the goomba." << std::endl;
                                    player.decrementPowerLevel();
                                    if (player.getPowerLevel() ==  0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        if (player.getLives() == 0){
                                            marioAlive = false;
                                        }
                                    }
                                }
                                else{
                                    game << "You beat the goomba." << std::endl;
                                    break;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'k':{
                                game << "You are about to fight a koopa. Good luck." << std::endl;
                                int koo = rand() % 36;
                                int yo = rand() % 66;
                                if (koo > yo){
                                    game << "You lost to the koopa." << std::endl;
                                    player.decrementPowerLevel();
                                    if (player.getPowerLevel() ==  0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        if (player.getLives() == 0){
                                            marioAlive = false;
                                        }
                                    }
                                }
                                else{
                                    game << "You beat the koopa." << std::endl;
                                    break;
                                }
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'b':{
                                game << "You are about to fight the boss. Good luck." << std::endl;
                                int bo = 0;
                                int yourNu = 0;
                                while (bo > yourNu){
                                    bo = rand() % 51;
                                    yourNu = rand() % 51;
                                    if (player.getPowerLevel() == 0){
                                        player.decrementLives();
                                        game << "You lost a life." << std::endl;
                                        player.changePowerLevel(0);
                                        if (player.getLives() == 0){
                                            game << "You lost to the boss." << std::endl;
                                            marioAlive = false;
                                        }
                                    }
                                }
                                game << "You beat the boss." << std::endl;
                                levelComplete = true;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'w':{
                                game << "You found the warp pipe. You move on to the next level." << std::endl;
                                levelComplete = true;
                                game << player.toString() << std::endl;
                                break;
                            }
                            case 'H':{
                                break;
                            }
                        }
                    }
                    else{
                        levelArr[oldRow][oldColumn] = 'H';
                        game << "Mario cannot move right." << std::endl;
                    }
                    
                    break;
                }

            }

        }
        //deletes the array after each level
        for(int i = 0; i < dimensions; ++i) {
            delete[] levelArr[i];
        }
        delete[] levelArr;
        //checks to see if mario is alive 
        if (!marioAlive){
            break;
        }
        //moves on to the next level if mario is alive
        ++getLevel;
    }

    if (player.getLives() == 0){
        game << "You have lost without capturing the princess.";
    }
    else{
        game << "You won and have rescued the princess!";
    }
    //closes file
    game.close();
}