#include <iostream>
#include <fstream>
#include "Map.h"

int main(int argc, char** argv){
    int i = 0;
    std::string s;
    std::ifstream readFile(argv[1]);
    std::string words[8]; //file will always read 8 elements - at least in this project - constant array size
    if (readFile.is_open()){
        while (getline(readFile, s)){
            words[i] = s;
            ++i;
        }
        map mymap(stoi(words[0]),stoi(words[1]));
        mymap.generateMap(stoi(words[3]), stoi(words[4]), stoi(words[5]), stoi(words[6]), stoi(words[7]));
        mymap.play("game.txt", stoi(words[2]));
        readFile.close();
        return 0;
    }
    else{
        std::cout << "File did not open." << std::endl;
        return -1;
    }
}