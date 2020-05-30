#include "GameMaker.hpp"

//#include <glm/gtx/string_cast.hpp>
int i = 0;
float j = 0.0;
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

    shaderTexture = LoadShaders("shaders/VertexShader.vert", "shaders/FragmentShader.frag");
    shaderLamp = LoadShaders("shaders/lampShader.vs", "shaders/lampShader.fs");
    shaderMaterial = LoadShaders("shaders/materials.vs", "shaders/materials.fs");
    shaderMaterialAndTexture = LoadShaders("shaders/lampShader.vs", "shaders/lampShader.fs");
    shaderBasicLightning = LoadShaders("shaders/basic_lighting.vs", "shaders/basic_lighting.fs");
    shaderStandard = LoadShaders("shaders/StandardShading.vs", "shaders/StandardShading.fs");
    shaderPhong = LoadShaders("shaders/phong_ilumination.vs", "shaders/phong_ilumination.fs");

    transferCubeToGPUMemory();
    transferFloorToGPUMemory();
    transferHoleToGPUMemory();
    loadPlayer();

    uvmap = loadDDS("uvmap.DDS");

    cubeMap = new CubeMap();
    cubeMap->load();

}

void GameMaker::update(double dt){

    physicsWorld->dynamicsWorld->stepSimulation(dt, 0.017);

    setMVP();
    drawMap();

    vec3 playerPos = playerBody->getWorldPosition();
    if ((int) playerPos.x == (int) end_point.getX() && (int) playerPos.z == (int) end_point.getZ()) {
        // win
        win = true;
    }

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
    
    GameMaker::floor = new Object();
    floor->loadOBJtoGPU("floor.obj", vec3(1.0f, 1.0f, 1.0f));
    floor->loadLight(vec3(1.0f, 1.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f));
    
    /**

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

    static const GLfloat normals[] = {

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
    **/
    
}

void GameMaker::transferHoleToGPUMemory(void) {

    Texture_hole = (GLuint*) malloc(4 * sizeof(GLuint));

    Texture_hole[0] = loadBMP_custom("images/hole_1.bmp");
    Texture_hole[1] = loadBMP_custom("images/hole_2.bmp");
    Texture_hole[2] = loadBMP_custom("images/hole_3.bmp");
    Texture_hole[3] = loadBMP_custom("images/hole_4.bmp");


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

    GameMaker::hole = new Object();    
    hole->loadToGPU(floor, g_uv_buffer_data_floor, 3*2);

    /** // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &holeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, holeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(floor), floor, GL_STATIC_DRAW);
    
    glGenBuffers(1, &uvbuffer_hole);
	glBindBuffer(GL_ARRAY_BUFFER, uvbuffer_hole);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data_floor), g_uv_buffer_data_floor, GL_STATIC_DRAW);**/
    
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

void GameMaker::drawFloor(glm::vec3 trans, bool start, bool end) {

    floor->setMVP(Projection, View, Model);

    floor->setTranslate(trans);
    //floor->drawWithPhongShader(cameraAt, playerBody->getWorldPosition(), shaderPhong);
   // floor->drawWithMaterialShader(playerBody->getWorldPosition(), cameraAt, shaderMaterial);
    //floor->drawWithStandardShader(uvmap, vec3(1.0f, 10.0f, 1.0f), shaderStandard );

    vec3 lightPos = playerBody->getWorldPosition();

    glm::mat4 rotationMat(1); // Creates a identity matrix
    rotationMat = glm::rotate(rotationMat, glm::radians(rotateX), glm::vec3(1, 0, 0));
    rotationMat = glm::rotate(rotationMat, glm::radians(rotateZ), vec3(0,0,1));
    
    lightPos = glm::vec3(rotationMat * glm::vec4(lightPos, 1.0));


    if (start)
        floor->drawWithBasicLightningShader(lightPos, vec3(1,1,1), vec3(0,1,0), shaderBasicLightning);
    else if (end)
        floor->drawWithBasicLightningShader(lightPos, vec3(1,1,1), vec3(1,0,0), shaderBasicLightning);
    else {
       // floor->drawWithStandardShader(Texture_floor, playerBody->getWorldPosition(), shaderStandard);
        floor->drawWithBasicLightningShader(lightPos, vec3(1,1,1), vec3(0.7,0.7,0.7), shaderBasicLightning);
    }

    
} 

void GameMaker::drawHole(glm::vec3 trans) {

   hole->setMVP(Projection, View, Model);
   hole->setTranslate(trans);
   hole->drawWithTextureShader(Texture_hole[0], shaderTexture);

/*
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

    if(i>=3)
        i=0;
	glBindTexture(GL_TEXTURE_2D, Texture_hole[i]);
    if(j==0.0||j==1.0||j==2.0||j==3.0)
        i = i+1;
    j=j+0.002;
    if(j>=3)
        j= 0.0;
    //std::cout<< "J: "<<j<<" \n";


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
    glDisableVertexAttribArray(1);*/
}

GameMaker::~GameMaker() {
    cleanupDataFromGPU();
}

void GameMaker::cleanupDataFromGPU() {
    glDeleteBuffers(1, &cubeVertexBuffer);
    glDeleteBuffers(1, &floorVertexBuffer);
    glDeleteBuffers(1, &playerVertexBuffer);
    glDeleteBuffers(1, &holeVertexBuffer);
    glDeleteBuffers(1, &uvbuffer);
    glDeleteBuffers(1, &uvbuffer_floor);
    glDeleteBuffers(1, &uvbuffer_hole);
    glDeleteBuffers(1, &uvbuffer_player);
    glDeleteVertexArrays(1, &VertexArrayID);
    glDeleteProgram(shaderTexture);
    glDeleteProgram(shaderLamp);
    glDeleteProgram(shaderMaterial);
    glDeleteProgram(shaderMaterialAndTexture);
}

void GameMaker::loadPlayer() {

    GameMaker::player = new Object();
    player->loadOBJtoGPU("sphere.obj");
                  
}

void GameMaker::drawPlayer() {

        player->setMVP(Projection, View, Model);
        player->setScale(vec3(0.5f, 0.5f, 0.5f));
        player->drawWithLampShader(vec3(1.0f, 1.0f, 1.0f), shaderLamp);

    if(!win){

        player->setTranslate(playerBody->getWorldPosition());
    
    }else{
        if(winPos.x==-999){

            winPos = playerBody->getWorldPosition();
        }


    }

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
                if (map[i][j] == 'E') {
                    end_point = btVector3(x_pos, 0.5f, y_pos);
                }
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
                if (map[i][j] == 'S') {
                    drawFloor(glm::vec3(objectBodies[k]->getWorldPosition().x, objectBodies[k]->getWorldPosition().y, objectBodies[k]->getWorldPosition().z), true, false);
                } else if (map[i][j] == 'E') {
                    drawFloor(glm::vec3(objectBodies[k]->getWorldPosition().x, objectBodies[k]->getWorldPosition().y, objectBodies[k]->getWorldPosition().z), false, true);
                } else {
                    drawFloor(glm::vec3(objectBodies[k]->getWorldPosition().x, objectBodies[k]->getWorldPosition().y, objectBodies[k]->getWorldPosition().z), false, false);
                }
                // gen end

            }
            k++;
        }
    }
     drawPlayer();
    cubeMap->draw(MVP, shaderTexture);

   

    //std::cout << " gravity: " << physicsWorld->dynamicsWorld->getGravity().getX() << " " << physicsWorld->dynamicsWorld->getGravity().getY() << " " << physicsWorld->dynamicsWorld->getGravity().getZ() << " " << std::endl;
    //std::cout << " velocity: " << playerBody->body->getLinearVelocity().getX() << " " << playerBody->body->getLinearVelocity().getY() << " " << playerBody->body->getLinearVelocity().getZ() << " " << std::endl;
    //std::cout << " position: " << playerBody->getWorldPosition().x << " " << playerBody->getWorldPosition().y << " " << playerBody->getWorldPosition().z << std::endl;
    //std::cout << " " << std::endl;

}