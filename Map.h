#ifndef MAP_H
#define MAP_H
#include <string>

class map {
    public:
        map(int amountOfLevels, int grid);
        ~map();
        void play(std::string file, int x);
        char checkCoord(char c);
        void generateMap(int coinPerc, int goombas, int koopa, int shrooms, int nothingPerc);
        std::string getMarioLocation(char** matrix);
        int getMarioRow(char** matrix);
        int getMarioColumn(char** matrix);
    private:
        char*** m_map;
        int dimensions;
        int levels;

};
#endif