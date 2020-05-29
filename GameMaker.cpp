#include "GameMaker.hpp"

#include <glm/gtx/string_cast.hpp>

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

    //programID2 = LoadShaders("shaders/transform.vs", "shaders/transform.fs");
    shaderTexture = LoadShaders("shaders/VertexShader.vert", "shaders/FragmentShader.frag");
    shaderLamp = LoadShaders("shaders/lampShader.vs", "shaders/lampShader.fs");
    //materialShader = LoadShaders("shaders/materials.vs", "shaders/materials.fs");
    //materialAndTextureShader = LoadShaders("shaders/lampShader.vs", "shaders/lampShader.fs");

    

    transferCubeToGPUMemory();
    transferFloorToGPUMemory();
    transferHoleToGPUMemory();
    loadPlayer();

    cubeMap = new CubeMap();
    cubeMap->load();

}

void GameMaker::update(double dt){

    physicsWorld->dynamicsWorld->stepSimulation(dt, 0.017);

    setMVP();
    drawMap();

}

void GameMaker::transferCubeToGPUMemory(void) {

	Texture_crate = loadBMP_custom("images/crate.bmp");

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

        0.0f,0.0f,  1.0f,0.0f,  1.0f,1.0f,
        0.0f,0.0f,  0.0f,1.0f,  1.0f,1.0f,

        0.0f,0.0f,  1.0f,0.0f,  0.0f,1.0f,
        1.0f,1.0f,  1.0f,0.0f,  0.0f,1.0f,

        0.0f,0.0f,  1.0f,0.0f,  1.0f,1.0f,
        0.0f,0.0f,  0.0f,1.0f,  1.0f,1.0f,
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

    Texture_floor = loadBMP_custom("images/wall.bmp");

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

void GameMaker::transferHoleToGPUMemory(void) {

    Texture_hole = loadBMP_custom("images/hole.bmp");

    static const GLfloat floor[] = {
            0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f, // base
            1.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f, 0.0f
        };
    
    // One color for each vertex. They were generated randomly.
    static const GLfloat g_uv_buffer_data_floor[] = {
        1.0f,0.0f,    1.0f,1.0f, 0.0f,0.0f, // imagem rodada
        1.0f,0.0f,    0.0f,0.0f, 1.0f,1.0f,

        //0.0f,0.0f,  1.0f,0.0f,  0.0f,1.0f, // imagem normal
        //1.0f,1.0f,  1.0f,0.0f,  0.0f,1.0f,

    };

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &holeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, holeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
    
    glGenBuffers(1, &uvbuffer_hole);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_hole);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data_floor), g_uv_buffer_data_floor, GL_STATIC_DRAW);
    
}
glm::mat4 GameMaker::setMVP(void) {
    
    // Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    
    // Camera matrix
    View = glm::lookAt(
            glm::vec3(cameraAt.x, cameraAt.y, cameraAt.z), // Camera is at (4,3,-3), in World Space
            glm::vec3(lookAt.x, lookAt.y, lookAt.z), // and looks at the origin
            glm::vec3(0, 1, 0)  // Head is up (set to 0,-1,0 to look upside-down)
        );

    // Model matrix depends on the object, so it is defined somewhere in the program
    Model = glm::rotate(Model, glm::radians(angulo), glm::vec3(0,1,0));

    return Projection * View * Model;

}

void GameMaker::drawCube(glm::vec3 trans) {
    
    glUseProgram(shaderTexture);

    glm::mat4 Trans = glm::translate(glm::mat4(1.0f), trans);
    //glm::mat4 Trans = glm::translate(glm::mat4(1.0f), glm::vec3(transX, 0.0f, transZ));

    MVP =  Projection * View * Model * Trans;

    // Send our transformation to the currently Vid shader,
    // in the "MVP" uniform, which is now MVP
    glUniformMatrix4fv(glGetUniformLocation(shaderTexture, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    // Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture_crate);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(glGetUniformLocation(shaderTexture, "myTextureSampler"), 0);
    
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

    glUseProgram(shaderTexture);

  
    //glm::mat4 model;
    //model = glm::rotate(glm::mat4(1.0f), glm::radians(z), vec3(0,0,1));
    //model = glm::translate(glm::mat4(1.0f), playerBody->getWorldPosition());
    //model = glm::scale(game.getPlayer()->model, vec3(.038,.038,.038));


    glm::mat4 Trans = glm::translate(glm::mat4(1.0f), trans);

    
    MVP =  Projection * View * Model * Trans;
    
    
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, which is now MVP
    glUniformMatrix4fv(glGetUniformLocation(shaderTexture, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    // Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture_floor);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(glGetUniformLocation(shaderTexture, "myTextureSampler"), 0);
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

    /**

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


    glUseProgram(programID);

    glm::mat4 Trans = glm::translate(glm::mat4(1.0f), trans);
    //glm::mat4 Trans = glm::translate(glm::mat4(1.0f), glm::vec3(transX, 0.0f, transZ));
    
    MVP = Projection * View * Model * Trans;

    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

    
    
    glUseProgram(materialShader);

    glm::mat4 Trans = glm::translate(glm::mat4(1.0f), trans);
    
    // MVP =  Projection * View * Model * Trans;

    glm::mat4 model = Model * Trans;

    vec3 light_pos = playerBody->getWorldPosition();
    vec3 light_color_ambient = vec3(1.0f);
    vec3 light_color_diffuse = vec3(1.0f);


    glUniform3f(glGetUniformLocation(materialShader, "light.position"), light_pos.x, light_pos.y, light_pos.z);
    glUniform3f(glGetUniformLocation(materialShader, "light.ambient"), light_color_ambient.x, light_color_ambient.y, light_color_ambient.z);
    glUniform3f(glGetUniformLocation(materialShader, "light.diffuse"), light_color_diffuse.x, light_color_diffuse.y, light_color_diffuse.z);

    glUniform3f(glGetUniformLocation(materialShader, "viewPos"), cameraAt.x, cameraAt.y, cameraAt.z);

    glUniform3f(glGetUniformLocation(materialShader, "material.ambient"), 0.135, 0.2225, 0.1575);
    glUniform3f(glGetUniformLocation(materialShader, "material.diffuse"), 0.54, 0.89, 0.63);
    glUniform3f(glGetUniformLocation(materialShader, "material.specular"), 0.316228, 0.316228, 0.316228);
    glUniform1f(glGetUniformLocation(materialShader, "material.shininess"), 0.1 * 128.0);

    glUniformMatrix4fv(glGetUniformLocation(materialShader, "projection"), 1, GL_FALSE, &Projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(materialShader, "view"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(materialShader, "model"), 1, GL_FALSE, &model[0][0]);

    
   
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
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glDrawArrays(GL_TRIANGLES, 0, 2*3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    **/
}

void GameMaker::drawHole(glm::vec3 trans) {

   glUseProgram(shaderTexture);
  
    //glm::mat4 model;
    //model = glm::rotate(glm::mat4(1.0f), glm::radians(z), vec3(0,0,1));
    //model = glm::translate(glm::mat4(1.0f), playerBody->getWorldPosition());
    //model = glm::scale(game.getPlayer()->model, vec3(.038,.038,.038));


    glm::mat4 Trans = glm::translate(glm::mat4(1.0f), trans);

    
    MVP =  Projection * View * Model * Trans;
    
    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, which is now MVP
    glUniformMatrix4fv(glGetUniformLocation(shaderTexture, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    // Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, Texture_hole);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(glGetUniformLocation(shaderTexture, "myTextureSampler"), 0);
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, holeVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_hole);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);
    
    glDrawArrays(GL_TRIANGLES, 0, 2*3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

GameMaker::~GameMaker() {
    cleanupDataFromGPU();
}

void GameMaker::cleanupDataFromGPU() {
    glDeleteBuffers(1, &cubeVertexBuffer);
   // glDeleteBuffers(1, &cubeColorbuffer);
    glDeleteBuffers(1, &floorVertexBuffer);
    //glDeleteBuffers(1, &floorColorBuffer);
   // glDeleteBuffers(1, &playerColorBuffer);
    glDeleteBuffers(1, &playerVertexBuffer);
   // glDeleteBuffers(1, &holeColorBuffer);
    glDeleteBuffers(1, &holeVertexBuffer);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(shaderTexture);
   // glDeleteProgram(programID2);
    glDeleteProgram(shaderLamp);
   // glDeleteProgram(materialShader);
   // glDeleteProgram(materialAndTextureShader);
}

void GameMaker::loadPlayer() {

    GameMaker::player = new Object();
    player->loadOBJtoGPU("sphere.obj");
                  
}

void GameMaker::drawPlayer() {

    player->setMVP(Projection, View, Model);

    player->setScale(vec3(0.5f, 0.5f, 0.5f));
    player->setTranslate(playerBody->getWorldPosition());
    player->drawWithLampShader(vec3(1.0f, 1.0f, 1.0f), shaderLamp);

}

void GameMaker::loadPhysics() {
    char** map = mg.getMatrixForOpenGL();
    for (int i = 0; i < mg.mapHeight; i++) {
        for (int j = 0; j < mg.mapWidth; j++) {

            int x_pos = i - ((mazeWidth*2)/2), y_pos = j - ((mazeHeight*2)/2);

            if (map[i][j] == 'S') {
                start_point = btVector3(x_pos, 0.6f, y_pos);
                cameraAt.y = glm::max(mazeHeight*2, mazeWidth*2);
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
    std::cout << "Start: " << start_point.getX() << " " << start_point.getZ() << std::endl;
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
                int x_pos = i - ((mazeWidth*2)/2) -1, z_pos = j - ((mazeHeight*2)/2) -1;
                drawHole(glm::vec3(x_pos + 0.5f, -0.5, z_pos + 0.5f));
                
                k--;
            } else {
                drawFloor(glm::vec3(objectBodies[k]->getWorldPosition().x - 0.5f, objectBodies[k]->getWorldPosition().y, objectBodies[k]->getWorldPosition().z - 0.5f));
            }
            k++;
        }
    }

    cubeMap->draw(MVP, shaderTexture);

    drawPlayer();

    std::cout << " gravity: " << physicsWorld->dynamicsWorld->getGravity().getX() << " " << physicsWorld->dynamicsWorld->getGravity().getY() << " " << physicsWorld->dynamicsWorld->getGravity().getZ() << " " << std::endl;
    std::cout << " velocity: " << playerBody->body->getLinearVelocity().getX() << " " << playerBody->body->getLinearVelocity().getY() << " " << playerBody->body->getLinearVelocity().getZ() << " " << std::endl;
    std::cout << " position: " << playerBody->getWorldPosition().x << " " << playerBody->getWorldPosition().y << " " << playerBody->getWorldPosition().z << std::endl;
    std::cout << " " << std::endl;

}