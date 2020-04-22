#include <iostream>

#include "MapGenerator.hpp"

int main( void ) {

    MapGenerator mg = MapGenerator(15, 15);
    mg.printMazeMap();

    return 0;

}