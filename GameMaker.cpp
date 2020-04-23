#include "GameMaker.hpp"

GameMaker::GameMaker(int mazeHeight, int mazeWidth) {  

    // Generate Maze
    mg.Init(mazeHeight, mazeWidth);
    mg.printMazeMap();

}
GameMaker::GameMaker(int mazeHeight, int mazeWidth, Physics::PhysicsWorld *pw) {  

    // Generate Maze
    mg.Init(mazeHeight, mazeWidth);
    mg.printMazeMap();

    physicsWorld = pw;

}

void GameMaker::transferDataToGPUMemory(void) {

    transferCubeToGPUMemory();
    transferFloorToGPUMemory();

    loadPlayer();

}

void GameMaker::update(double dt){

    physicsWorld->dynamicsWorld->stepSimulation(dt, 0.017);

    setMVP();
    drawMap();

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

void GameMaker::drawCube(glm::vec3 trans) {
    
    glUseProgram(programID);
 glm::mat4 Trans = glm::translate(glm::mat4(1.0f), trans);
    //glm::mat4 Trans = glm::translate(glm::mat4(1.0f), glm::vec3(transX, 0.0f, transZ));
    
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

void GameMaker::drawFloor(glm::vec3 trans) {
    
    glUseProgram(programID);

  
    //glm::mat4 model;
    //model = glm::rotate(glm::mat4(1.0f), glm::radians(z), vec3(0,0,1));
    //model = glm::translate(glm::mat4(1.0f), playerBody->getWorldPosition());
    //model = glm::scale(game.getPlayer()->model, vec3(.038,.038,.038));


    glm::mat4 Trans = glm::translate(glm::mat4(1.0f), trans);
    
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

void GameMaker::loadPlayer() {

    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals; // Won't be used at the moment.
    bool res = loadOBJ("sphere.obj", vertices, uvs, normals);

	glGenBuffers(1, &playerVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, playerVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &playerColorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, playerColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    GameMaker::size = vertices.size();
                    
}

void GameMaker::drawPlayer(GLfloat scale) {

    glUseProgram(programID);

    std::cout << " C " << playerBody->getWorldPosition().x << " " << playerBody->getWorldPosition().y << " " << playerBody->getWorldPosition().z << " " << std::endl;

    glm::mat4 model;
    //model = glm::rotate(glm::mat4(1.0f), glm::radians(z), vec3(0,0,1));
    model = glm::translate(glm::mat4(1.0f), playerBody->getWorldPosition());
    //model = glm::scale(game.getPlayer()->model, vec3(.038,.038,.038));

    //glm::mat4 Trans = glm::translate(glm::mat4(1.0f), glm::vec3(transX, transY, transZ));
    glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));

    MVP =  Projection * View * Model * model * Scale;
    
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, which is now MVP
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, playerVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : colors
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, playerColorBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glDrawArrays(GL_TRIANGLES, 0, GameMaker::size);

    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    std::cout << " B " << std::endl;

}

void GameMaker::loadPhysics() {
    char** map = mg.getMatrixForOpenGL();
    for (int i = 0; i < mg.mapHeight; i++) {
        for (int j = 0; j < mg.mapWidth; j++) {
            if (map[i][j] == 'S') {
                start_point = btVector3(i + 0.5f, 0.7f, j + 0.5f);
            }
            if (map[i][j] == 'X') {
                btCollisionShape* colShape = physicsWorld->createBoxShape(btVector3(0.5f, 0.5f, 0.5f));
                Physics::PhysicsBody* pb = physicsWorld->createPhysicsBody(btVector3(i, 0.0f, j), colShape, btScalar(0));
                objectBodies.push_back(pb); 
            } else {
                btCollisionShape* colShape = physicsWorld->createBoxShape(btVector3(0.5f, 0.001f, 0.5f));
                Physics::PhysicsBody* pb = physicsWorld->createPhysicsBody(btVector3(i, -0.5f, j), colShape, btScalar(0));
                objectBodies.push_back(pb);
            }
        }
    }
    btCollisionShape* colShape = physicsWorld->createSphereShape(0.35f);
    playerBody = physicsWorld->createPhysicsBody(start_point, colShape, btScalar(1));
}

void GameMaker::drawMap() {

    char** map = mg.getMatrixForOpenGL();

    int k = 0;

    for (int i = 0; i < mg.mapHeight; i++) {
        for (int j = 0; j < mg.mapWidth; j++) {
            if (map[i][j] == 'X') {
                drawCube(glm::vec3(objectBodies[k]->getWorldPosition().x - 0.5f, objectBodies[k]->getWorldPosition().y - 0.5f, objectBodies[k]->getWorldPosition().z - 0.5f));
            } else {
                drawFloor(glm::vec3(objectBodies[k]->getWorldPosition().x - 0.5f, objectBodies[k]->getWorldPosition().y, objectBodies[k]->getWorldPosition().z - 0.5f));
            }
            k++;
        }
    }

    drawPlayer(0.5f);

}