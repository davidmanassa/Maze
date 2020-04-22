#include "GameMaker.hpp"

GameMaker::GameMaker(int mazeHeight, int mazeWidth) {  

    // Generate Maze
    mg.Init(mazeHeight, mazeWidth);
    mg.printMazeMap();

}

void GameMaker::transferDataToGPUMemory(void) {
    transferCubeToGPUMemory();
    transferFloorToGPUMemory();
}

void GameMaker::transferCubeToGPUMemory(void) {

   static const GLfloat cube[] = {
      //  0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, // base
       // 1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f, // cima
        1.0f, 1.0f, 1.0f,  0.0f, 1.0f, 1.0f,  1.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f, // parede 1
        1.0f, 1.0f, 0.0f,  1.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,

        1.0f, 0.0f, 0.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f, 1.0f, // parede 2
        1.0f, 0.0f, 0.0f,  1.0f, 1.0f, 0.0f,  1.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f, // parede 3
        1.0f, 1.0f, 1.0f,  1.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f,

        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f, 1.0f, // parede 4
        0.0f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 1.0f

    };
    
    // One color for each vertex. They were generated randomly.
    static const GLfloat cube_colors[] = {
      //  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan
    // 0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan

        0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan
        0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan

        0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan
        0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan

        0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan
        0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan

        0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan
        0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan

        0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan
        0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,  0.00f, 1.0f, 1.0f,    // cyan
    };

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &cubeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    
    // Move color data to video memory; specifically to CBO called colorbuffer
    glGenBuffers(1, &cubeColorbuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cubeColorbuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_colors), cube_colors, GL_STATIC_DRAW);
    
}

void GameMaker::transferFloorToGPUMemory(void) {

   static const GLfloat floor[] = {
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, // base
        1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f
    };
    
    // One color for each vertex. They were generated randomly.
    static const GLfloat floor_colors[] = {
        1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,   // white
        1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f    // white
    };

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &floorVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, floorVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
    
    // Move color data to video memory; specifically to CBO called colorbuffer
    glGenBuffers(1, &floorColorBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, floorColorBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor_colors), floor_colors, GL_STATIC_DRAW);
    
}

void GameMaker::setMVP(void) {

    // Get a handle for our "MVP" uniform
    MatrixID = glGetUniformLocation(programID, "MVP");
    
    // Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    
    // Camera matrix
    View = glm::lookAt(
            glm::vec3(cameraAtX, cameraAtY, cameraAtZ), // Camera is at (4,3,-3), in World Space
            glm::vec3(lookAtX, lookAtY, lookAtZ), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

    // Model matrix depends on the object, so it is defined somewhere in the program
    Model = glm::rotate(Model, glm::radians(angulo), glm::vec3(0,1,0));

}

void GameMaker::drawCube(GLfloat transX, GLfloat transZ) {
    
    glUseProgram(programID);

    glm::mat4 Trans = glm::translate(glm::mat4(1.0f), glm::vec3(transX, 0.0f, transZ));
    
    MVP =  Projection * View * Model * Trans;
    
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, which is now MVP
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, cubeColorbuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glDrawArrays(GL_TRIANGLES, 0, 2*3*5);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void GameMaker::drawFloor(GLfloat transX, GLfloat transZ) {
    
    glUseProgram(programID);

    glm::mat4 Trans = glm::translate(glm::mat4(1.0f), glm::vec3(transX, 0.0f, transZ));
    
    MVP =  Projection * View * Model * Trans;
    
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, which is now MVP
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, floorVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, floorColorBuffer);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glDrawArrays(GL_TRIANGLES, 0, 2*3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void GameMaker::cleanupDataFromGPU() {
    glDeleteBuffers(1, &cubeVertexBuffer);
    glDeleteBuffers(1, &cubeColorbuffer);
    glDeleteBuffers(1, &floorVertexBuffer);
    glDeleteBuffers(1, &floorColorBuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(programID);
}

void GameMaker::drawMap() {

    char** map = mg.getMatrixForOpenGL();

    for (int i = 0; i < mg.mapHeight; i++) {
        for (int j = 0; j < mg.mapWidth; j++) {
            if (map[i][j] == 'X') {
                drawCube(i, j);
            } else {
                drawFloor(i, j);
            }
        }
    }

}