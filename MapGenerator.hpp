#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

typedef struct {
    int x, y;
    bool visited = false;
} Cell;

class MapGenerator {
    
	private:
		// variáveis auxiliares
        int height, width;

        Cell **map;

        Cell start_point;
        Cell current;

        Cell *path;
        int tot_path;;

        int tot_options;

        char **charMaze;
        char **openGLReadyMatrix;

        // métodos auxiliares
        void buildMapDFS();
        bool allVisited();
        void printVisited();
        Cell* getMovementOptions(Cell atual);

	public:
        int mapHeight, mapWidth;

        MapGenerator(int height, int width); // Construtor -> Aloca as matrizes necessárias para construção do mapa
        void printMazeMap();
        ~MapGenerator(); // Finalizador -> Liberta o espaço alocado
        char** getMatrixForOpenGL();

        MapGenerator();
        void Init(int height, int width);

};

#endif