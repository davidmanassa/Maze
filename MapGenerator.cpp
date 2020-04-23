#include "MapGenerator.hpp"

MapGenerator::~MapGenerator() {
    // free memmory
}

MapGenerator::MapGenerator() {

}

MapGenerator::MapGenerator(int height, int width) {
    Init(height, width);
}

void MapGenerator::Init(int height, int width) {
    MapGenerator::height = height;
    MapGenerator::width = width;

    MapGenerator::mapHeight = height * 2 + 1;
    MapGenerator::mapWidth = width * 2 + 1;

    openGLReadyMatrix = NULL;

    map = (Cell**) malloc(height * sizeof(Cell*));
    for (int i = 0; i < height; i++) {
        map[i] = (Cell*) malloc(width * sizeof(Cell));
        for (int j = 0; j < width; j++) {
            Cell cell;
            cell.x = i; // Why not   Why yes   :)
            cell.y = j; // Why not   Why yes   :)   We can use the position i, j  but its Okay
            cell.visited = false;
            map[i][j] = cell;
        }
    }

    charMaze = (char**) malloc((height * 2 - 1 ) * sizeof(char*));
    for (int i = 0; i < height * 2 - 1; i++) {
        charMaze[i] = (char*) malloc((width * 2 - 1 ) * sizeof(char));
        for (int j = 0; j < width * 2 - 1; j++)
            charMaze[i][j] = 'X';
        charMaze[i][width * 2] = '\0';
    }
    //charMaze[height * 2] = '\0';

    srand(time(NULL)); 
	int random = rand() % width;

    // Prepare algoritm 
    map[random][0].visited = true;
    start_point = map[random][0];

    tot_options = 0;
    tot_path = 0;

    path = NULL;
    path = (Cell*) realloc(path, (++tot_path) * sizeof(Cell));
	path[tot_path - 1] = map[random][0];
   
    charMaze[random*2][0] = 'S';

    buildMapDFS();
}

void MapGenerator::buildMapDFS() {

	Cell current = path[tot_path - 1];
	Cell* options = getMovementOptions(current);

    if (options == NULL)
        if (allVisited())
            return;
        else
            while (options == NULL) { // ** Backtrack ** Chegamos a um fim sem saida
            
                path = (Cell*) realloc(path, (--tot_path) * sizeof(Cell));
                current = path[tot_path - 1];

                if (allVisited())
                    return;
                
                options = getMovementOptions(current);

            }

	// Vamos escolher uma célula candidata aleatoriamente
	int random = rand() % tot_options;
    Cell chosed = options[random];

	path = (Cell*) realloc(path, (++tot_path) * sizeof(Cell));
	path[tot_path - 1] = chosed;

    map[chosed.x][chosed.y].visited = true;

    int xChar = chosed.x*2, yChar = chosed.y*2;

    charMaze[xChar][yChar] = '-';

    if (chosed.y > current.y) // cima
        charMaze[xChar][yChar-1] = '-';

    if (chosed.y < current.y) // baixo
        charMaze[xChar][yChar+1] = '-';

    if (chosed.x > current.x) // direita
        charMaze[xChar-1][yChar] = '-';

    if (chosed.x < current.x) // esquerda
        charMaze[xChar+1][yChar] = '-';

	buildMapDFS();

}

bool MapGenerator::allVisited() {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (map[i][j].visited == false) {
                return false;
            }
        }
    }
    return true;
}

Cell* MapGenerator::getMovementOptions(Cell atual) {

	Cell* options = NULL;
	tot_options = 0;

    if (atual.y + 1 < height) { // Cima
        if (map[atual.x][atual.y + 1].visited == false) {
            options = (Cell*) realloc(options, (++tot_options) * sizeof(Cell));
            options[tot_options - 1] = map[atual.x][atual.y + 1];
        }
    }

    if (atual.y - 1 > -1) { // Baixo
        if (map[atual.x][atual.y - 1].visited == false) {
            options = (Cell*) realloc(options, (++tot_options) * sizeof(Cell));
            options[tot_options - 1] = map[atual.x][atual.y - 1];
        }
    }

    if (atual.x + 1 < width) { // Direita
        if (map[atual.x + 1][atual.y].visited == false) {
            options = (Cell*) realloc(options, (++tot_options) * sizeof(Cell));
            options[tot_options - 1] = map[atual.x + 1][atual.y];
        }
    }

    if (atual.x - 1 > -1) { // Esquerda
        if (map[atual.x - 1][atual.y].visited == false) {
            options = (Cell*) realloc(options, (++tot_options) * sizeof(Cell));
            options[tot_options - 1] = map[atual.x - 1][atual.y];
        }
    }

	return(options);

}

char** MapGenerator::getMatrixForOpenGL() {

    if (openGLReadyMatrix != NULL)
        return openGLReadyMatrix;

    char** newMatrix = NULL;
    newMatrix = (char**) malloc((height * 2 + 2) * sizeof(char*));
    for (int i = 0; i < height * 2 + 1; i++) {
        newMatrix[i] = (char*) malloc((width * 2 + 2) * sizeof(char));
        if (i == 0 || i == height * 2) {
            for (int j = 0; j < width * 2 + 1; j++)
                newMatrix[i][j] = 'X';
            newMatrix[i][(width * 2 + 1)] = '\0';
            continue;
        }
        newMatrix[i][0] = 'X';
        for (int j = 1; j < width * 2; j++) {
            newMatrix[i][j] = charMaze[i - 1][j - 1];
        }
        newMatrix[i][(width * 2)] = 'X';
        newMatrix[i][(width * 2 + 1)] = '\0';
    }

    openGLReadyMatrix = newMatrix;

    return newMatrix;
}

void MapGenerator::printMazeMap() {

    if (openGLReadyMatrix == NULL)
        getMatrixForOpenGL();

    for (int i = 0; i < height * 2 + 1; i++) {
        for (int j = 0; j < width * 2 + 1; j++) {
            std::cout << openGLReadyMatrix[i][j];
        }
        std::cout << '\n';
    }
    
}