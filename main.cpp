#include <iostream>
#include <chrono>
#include <vector>

#include "GameMaker.hpp"
#include "MainMenu.hpp"
#include "stb_image.h"
#include "shader_s.h"
#include "camera.h"

GLFWwindow* window;

GLint windowWidth = 1024;
GLint windowHeight = 768;
GLint cmap_programID;
const float ASPECT = float(windowWidth)/windowHeight;
int mazeHeight = 15, mazeWidth = 15;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = (float)windowWidth / 2.0;
float lastY = (float)windowHeight / 2.0;
float mouse_press = false;

GameMaker* gm;

mainMenu* menu;

vec2 lastMousePosition;

vec2 getMousePosition(GLFWwindow *window){
	double x,y;
	glfwGetCursorPos(window,&x, &y);

	return vec2(x,y);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        gm->angulo += gm->delta;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        gm->angulo -= gm->delta;
    if (action = GLFW_PRESS)
        mouse_press = true;
    else
        mouse_press = false;
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if(yoffset > 0)
        gm->cameraAtY -= 0.25;
    else
        gm->cameraAtY += 0.25;
}

void handleKeyboardInput(GLFWwindow* window) {
    if(glfwGetKey(window,GLFW_KEY_W)) {
        gm->cameraAtZ -= 0.25f;
        gm->lookAtZ -= 0.25f;
    }
    if(glfwGetKey(window,GLFW_KEY_S)) {
        gm->cameraAtZ += 0.25f;
        gm->lookAtZ += 0.25f;
    }
    if(glfwGetKey(window,GLFW_KEY_D)) {
        gm->cameraAtX += 0.25f;
        gm->lookAtX += 0.25f;
    }
    if(glfwGetKey(window,GLFW_KEY_A)) {
        gm->cameraAtX -= 0.25f;
        gm->lookAtX -= 0.25f;
    }
    if(glfwGetKey(window,GLFW_KEY_E)) { // POWER UP
        gm->playerBody->body->activate();
        btVector3 v = gm->playerBody->body->getLinearVelocity();

        vec2 currentMousePos = getMousePosition(window);

        vec2 windowDimensions = vec2(windowWidth, windowHeight);

        int X = currentMousePos[0] - windowDimensions[0]/2;
        int Z = currentMousePos[1] - windowDimensions[1]/2;

        v.setX(X*0.1);
        v.setZ(Z*0.1);
        gm->playerBody->body->setLinearVelocity(v);
    }

    // mouse input
	vec2 currentMousePos = getMousePosition(window);
	if(currentMousePos != lastMousePosition){
		vec2 gravity = currentMousePos - vec2(windowWidth/2, windowHeight/2);
        vec2 windowDimensions = vec2(windowWidth, windowHeight);
		float distanceFromCenter = std::min(length(gravity)/length(windowDimensions/vec2(2)),1.0f);
		// std::cout << "distance: " << distanceFromCenter << std::endl;
		gravity = normalize(gravity);
		// std::cout << "New gravity, old:" << gravity.x*5*distanceFromCenter << ", "<< gravity.y*5*distanceFromCenter << std::endl;
		gm->physicsWorld->dynamicsWorld->setGravity(btVector3(gravity.x*(3.14159265*7.25)*.8*distanceFromCenter,-50,gravity.y*(3.14159265*7.25)*.8*distanceFromCenter));
		lastMousePosition = currentMousePos;
	}

}

void window_size_callback(GLFWwindow* window, int width, int height)
{

    glViewport(0, 0, width, height);

}

 
unsigned int loadCubemap(std::vector<std::string> faces)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    int width, height, nrChannels;
    for (unsigned int i = 0; i < faces.size(); i++)
    {
        unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 
                         0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
            );
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Cubemap tex failed to load at path: " << faces[i] << std::endl;
            stbi_image_free(data);
        }
    }
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    return textureID;
} 

int main( void ) {

    // Initialise GLFW
    glfwInit();
    
    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Open a window
    window = glfwCreateWindow(windowWidth, windowHeight, "Maze", NULL, NULL);
    
    // Create window context
    glfwMakeContextCurrent(window);


    // Initialize GLEW
    glewExperimental = true; // Needed for core profile
    glewInit();
    
    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    glfwSetWindowSizeCallback(window, window_size_callback);
    
    // White background
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    
    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

   /* std::vector<std::string> faces{
    "right.png",
    "left.png",
    "top.png",
    "bottom.png",
    "front.png",
    "back.png",
};*/
    std::vector<std::string> faces
    {
        "skybox/right.jpg",
        "skybox/left.jpg",
        "skybox/top.jpg",
        "skybox/bottom.jpg",
        "skybox/front.jpg",
        "skybox/back.jpg"
    };

GLfloat skyboxVertices[] = {
    // positions          
    -10.0f,  10.0f, -10.0f,
    -10.0f, -10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,
     10.0f,  10.0f, -10.0f,
    -10.0f,  10.0f, -10.0f,

    -10.0f, -10.0f,  10.0f,
    -10.0f, -10.0f, -10.0f,
    -10.0f,  10.0f, -10.0f,
    -10.0f,  10.0f, -10.0f,
    -10.0f,  10.0f,  10.0f,
    -10.0f, -10.0f,  10.0f,

     10.0f, -10.0f, -10.0f,
     10.0f, -10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,

    -10.0f, -10.0f,  10.0f,
    -10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f, -10.0f,  10.0f,
    -10.0f, -10.0f,  10.0f,

    -10.0f,  10.0f, -10.0f,
     10.0f,  10.0f, -10.0f,
     10.0f,  10.0f,  10.0f,
     10.0f,  10.0f,  10.0f,
    -10.0f,  10.0f,  10.0f,
    -10.0f,  10.0f, -10.0f,

    -10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f,  10.0f,
     10.0f, -10.0f, -10.0f,
     10.0f, -10.0f, -10.0f,
    -10.0f, -10.0f,  10.0f,
     10.0f, -10.0f,  10.0f
};

// skybox VAO
    unsigned int skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glBindVertexArray(skyboxVAO);
    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    unsigned int cubemapTexture = loadCubemap(faces); 

    Shader skyboxShader("cubemap.vertexshader", "cubemap.fragshader");
    
    Physics::PhysicsWorld pw = Physics::PhysicsWorld(btVector3(0, -100 , 0));
    GameMaker gm1 = GameMaker(mazeHeight, mazeWidth, &pw);
    gm = &gm1;

    // transfer my data (vertices, colors, and shaders) to GPU side
    glGenVertexArrays(1, &gm->VertexArrayID);
    glBindVertexArray(gm->VertexArrayID);

    gm->transferDataToGPUMemory();

    gm->loadPhysics();

    mainMenu menu1 = mainMenu();
    menu = &menu1;
    menu->load();

    // Initialize our little text library with the Holstein font
	initText2D( "Holstein.DDS" );

    // set the model-view-projection matrix
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    bool cameraUpdated = false;
    auto last_time = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();

    vec2 lastMousePos = getMousePosition(window);

    double dt;
	float x, z;

    skyboxShader.use();
    skyboxShader.setInt("skybox", 0);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = camera.GetViewMatrix();
    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float) windowWidth / (float) windowHeight, 0.1f, 100.0f);

    // render scene for each frame
    do {
        
        current_time  = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<double, std::milli>(current_time-last_time).count()/1000.0;
		last_time = current_time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        handleKeyboardInput(window);

      

        if (menu->isOn) { // MENU

            vec2 mousePos = getMousePosition(window);
            menu->draw(mousePos, vec2(windowWidth, windowHeight), mouse_press);

        } else { // GAME

            x = gm->physicsWorld->dynamicsWorld->getGravity().z();
            z = -gm->physicsWorld->dynamicsWorld->getGravity().x();

            gm->setMVP(); // let us update the rotation angle

            // Rotate board
            gm->Model = glm::translate(glm::mat4(1), glm::vec3(0,-8,0));
            gm->Model = glm::rotate(gm->Model, glm::radians(x), vec3(1,0,0));
            gm->Model = glm::rotate(gm->Model, glm::radians(z), vec3(0,0,1));

            btVector3 v = gm->playerBody->body->getLinearVelocity();
            if (v.getY() > 10) { // POINT TO SKY IS NOT A OPTION
                gm->playerBody->body->activate();
                v.setY(10);
                gm->playerBody->body->setLinearVelocity(v);
            }
            int maxVelocity = 20;
            if (v.getX() > maxVelocity) { // SLOW DOWN !!! 
                gm->playerBody->body->activate();
                v.setX(maxVelocity);
                gm->playerBody->body->setLinearVelocity(v);
            }
            if (v.getZ() > maxVelocity) { // SLOW DOWN !!! 
                gm->playerBody->body->activate();
                v.setZ(maxVelocity);
                gm->playerBody->body->setLinearVelocity(v);
            }
            if (v.getX() < -maxVelocity) { // SLOW DOWN !!! 
                gm->playerBody->body->activate();
                v.setX(-maxVelocity);
                gm->playerBody->body->setLinearVelocity(v);
            }
            if (v.getZ() < -maxVelocity) { // SLOW DOWN !!! 
                gm->playerBody->body->activate();
                v.setZ(-maxVelocity);
                gm->playerBody->body->setLinearVelocity(v);
            }
            if (gm->playerBody->getWorldPosition().y < -5.0f) { // CAIU FORA DO MAPA
                gm->playerBody->body->activate();
                v.setX(0.0f);
                v.setY(0.0f);
                v.setZ(0.0f);
                gm->playerBody->body->setLinearVelocity(v);

                btTransform transform = gm->playerBody->body->getCenterOfMassTransform();
                transform.setOrigin(gm->start_point);
                gm->playerBody->body->setCenterOfMassTransform(transform);
            }
            if (gm->playerBody->getWorldPosition().y > 0.5f) { // QUER VOAR !? NOT YET
                std::cout << " Ball want to fly =O " << std::endl;
                gm->playerBody->body->activate();
                v.setY(0.0f);
                gm->playerBody->body->setLinearVelocity(v);

                btTransform transform = gm->playerBody->body->getCenterOfMassTransform();
                glm::vec3 pos = gm->playerBody->getWorldPosition();
                btVector3 newPos = btVector3(pos.x, 0.5f, pos.z);
                transform.setOrigin(newPos);
                gm->playerBody->body->setCenterOfMassTransform(transform);
            }

            gm->update(dt);

            /**
             * printText2D -> Text, x, y, size
             * 
             *   x and y will be coordinates in [0-800][0-600]
             *  
             **/
            char text[256];
            sprintf(text,"Score: %.2f", glfwGetTime() );
            printText2D(text, 300, 550, 40);

        }

         // draw skybox as last
        glDepthFunc(GL_LEQUAL);  // change depth function so depth test passes when values are equal to depth buffer's content
        skyboxShader.use();
        view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // remove translation from the view matrix
        skyboxShader.setMat4("view", view);
        skyboxShader.setMat4("projection", projection);

        // skybox cube
        glBindVertexArray(skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
        glDepthFunc(GL_LESS); // set depth function back to default

        // Swap buffers
        glfwSwapBuffers(window);
        // looking for events
        glfwPollEvents();
        

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Cleanup VAO, VBOs, and shaders from GPU
    gm->cleanupDataFromGPU();
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}