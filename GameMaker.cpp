#include "GameMaker.hpp"

GameMaker::GameMaker(int mazeHeight, int mazeWidth) {  

    GameMaker::mazeHeight = mazeHeight;
    GameMaker::mazeWidth = mazeWidth;
    
    // Generate Maze
    mg.Init(mazeHeight, mazeWidth);
    mg.printMazeMap();

}
GameMaker::GameMaker(int mazeHeight, int mazeWidth, Physics::PhysicsWorld *pw) {

    GameMaker::mazeHeight = mazeHeight;
    GameMaker::mazeWidth = mazeWidth;

    // Generate Maze
    mg.Init(mazeHeight, mazeWidth);
    mg.printMazeMap();

    physicsWorld = pw;

}

void GameMaker::transferDataToGPUMemory(void) {

     programID2 = LoadShaders("TransformVertexShader.vert", "ColorFragmentShader.frag");
     programID = LoadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader" );

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

	Texture = loadBMP_custom("crate.bmp");
	
	// Get a handle for our "myTextureSampler" uniform
	TextureID  = glGetUniformLocation(programID, "myTextureSampler");

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
    static const GLfloat g_uv_buffer_data[] = {

        0.0f,0.0f,  1.0f,0.0f,  0.0f,1.0f,
        1.0f,1.0f,  1.0f,0.0f,  0.0f,1.0f,

        0.0f,0.0f,  1.0f,0.0f,  0.0f,1.0f,
        1.0f,1.0f,  1.0f,0.0f,  0.0f,1.0f,

        0.0f,0.0f,  1.0f,0.0f,  0.0f,1.0f,
        1.0f,1.0f,  1.0f,0.0f,  0.0f,1.0f,

        0.0f,0.0f,  1.0f,0.0f,  0.0f,1.0f,
        1.0f,1.0f,  1.0f,0.0f,  0.0f,1.0f,
    };

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &cubeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
    
    
	glGenBuffers(1, &uvbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);
    
}

void GameMaker::transferFloorToGPUMemory(void) {

    Texture_floor = loadBMP_custom("floor.bmp");
	
	// Get a handle for our "myTextureSampler" uniform
	TextureID_floor  = glGetUniformLocation(programID, "myTextureSampler");

   static const GLfloat floor[] = {
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, // base
        1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f
    };
    
    // One color for each vertex. They were generated randomly.
    static const GLfloat g_uv_buffer_data_floor[] = {

        0.0f,0.0f,  1.0f,0.0f,  0.0f,1.0f,
        1.0f,1.0f,  1.0f,0.0f,  0.0f,1.0f,

    };

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &floorVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, floorVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
    
    glGenBuffers(1, &uvbuffer_floor);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_floor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data_floor), g_uv_buffer_data_floor, GL_STATIC_DRAW);
    
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
    // Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID, 0);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);;
    
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
    // Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture_floor);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(TextureID_floor, 0);
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, floorVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_floor);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);;
    
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

    glUseProgram(programID2);

    glm::mat4 model1;

    //cameraAtX = wp.x;
    //cameraAtY = wp.y + 15;
    //cameraAtZ = wp.z + 5;

    //lookAtX = wp.x;
    //lookAtY = wp.y;
    //lookAtZ = wp.z;

    model1 = glm::translate(glm::mat4(1.0f), playerBody->getWorldPosition());

    glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(scale, scale, scale));

    MVP =  Projection * View * Model * model1 * Scale;
    
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

}

void GameMaker::loadPhysics() {
    char** map = mg.getMatrixForOpenGL();
    for (int i = 0; i < mg.mapHeight; i++) {
        for (int j = 0; j < mg.mapWidth; j++) {

            int x_pos = i - ((mazeWidth*2)/2), y_pos = j - ((mazeHeight*2)/2);

            if (map[i][j] == 'S') {
                start_point = btVector3(x_pos, 0.6f, y_pos);
                cameraAtY = glm::max(mazeHeight*2, mazeWidth*2);
            }
            if (map[i][j] == 'X') {
                btCollisionShape* colShape = physicsWorld->createBoxShape(btVector3(0.5f, 0.5f, 0.5f));
                Physics::PhysicsBody* pb = physicsWorld->createPhysicsBody(btVector3(x_pos, 0.0f, y_pos), colShape, btScalar(0));
                objectBodies.push_back(pb); 
            } else if (map[i][j] == 'B') {
            
            } else {
                btCollisionShape* colShape = physicsWorld->createBoxShape(btVector3(0.5f, 0.001f, 0.5f));
                Physics::PhysicsBody* pb = physicsWorld->createPhysicsBody(btVector3(x_pos, -0.5f, y_pos), colShape, btScalar(0));
                objectBodies.push_back(pb);
            }
        }
    }
    std::cout << "Start: " << start_point.getX() << " " << start_point.getZ();
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
            
            } else if (map[i][j] == 'B') {
                k--;
            } else {
                drawFloor(glm::vec3(objectBodies[k]->getWorldPosition().x - 0.5f, objectBodies[k]->getWorldPosition().y, objectBodies[k]->getWorldPosition().z - 0.5f));
            }
            k++;
        }
    }

    drawPlayer(0.5f);

    std::cout << " g: " << physicsWorld->dynamicsWorld->getGravity().getX() << " " << physicsWorld->dynamicsWorld->getGravity().getY() << " " << physicsWorld->dynamicsWorld->getGravity().getZ() << " " << std::endl;
    std::cout << " v: " << playerBody->body->getLinearVelocity().getX() << " " << playerBody->body->getLinearVelocity().getY() << " " << playerBody->body->getLinearVelocity().getZ() << " " << std::endl;

}