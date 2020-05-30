#include <iostream>
#include <chrono>
#include "GameMaker.hpp"
#include "MainMenu.hpp"
#include "CubeMap.hpp"

GLFWwindow* window;

GLint windowWidth = 1024;
GLint windowHeight = 768;
const float ASPECT = float(windowWidth)/windowHeight;
int mazeHeight = 15, mazeWidth = 15;

float mouse_press = false;

GameMaker* gm;
MainMenu* menu;
CubeMap* cubeMap;

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
         gm->cameraAt.y -= 0.25;
    else
        if(gm->cameraAt.y < 43.5){
          gm->cameraAt.y += 0.25;}
}

void handleKeyboardInput(GLFWwindow* window) {
    if(glfwGetKey(window,GLFW_KEY_W)) {
        gm->cameraAt.z -= 0.25f;
        gm->lookAt.z -= 0.25f;
    }
    if(glfwGetKey(window,GLFW_KEY_S)) {
        gm->cameraAt.z += 0.25f;
        gm->lookAt.z += 0.25f;
    }
    if(glfwGetKey(window,GLFW_KEY_D)) {
        gm->cameraAt.x += 0.25f;
        gm->lookAt.x += 0.25f;
    }
    if(glfwGetKey(window,GLFW_KEY_A)) {
        gm->cameraAt.x -= 0.25f;
        gm->lookAt.x -= 0.25f;
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
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    Physics::PhysicsWorld pw = Physics::PhysicsWorld(btVector3(0, -100 , 0));
    gm = new GameMaker(mazeHeight, mazeWidth, &pw);

    // transfer my data (vertices, colors, and shaders) to GPU side
    glGenVertexArrays(1, &gm->VertexArrayID);
    glBindVertexArray(gm->VertexArrayID);

    menu = new MainMenu();
    menu->load();

    gm->transferDataToGPUMemory();
    gm->loadPhysics();

    // Initialize our little text library with the Holstein font
	initText2D("Holstein.DDS");

    // set the model-view-projection matrix
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    bool cameraUpdated = false;
    auto last_time = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();

    vec2 lastMousePos = getMousePosition(window);

    double dt;
	float x, z;

    double start_time = -1.0;
    double final_score = -1.0;
     gm->cameraAt.y += 8; // Ajustar para ficar com o score fora do maze
    // render scene for each frame
    do {
        
        current_time  = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<double, std::milli>(current_time-last_time).count()/1000.0;
		last_time = current_time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        handleKeyboardInput(window);


        if (menu->isOn) { // MENU

            vec2 mousePos = getMousePosition(window);
            menu->draw(mousePos, vec2(windowWidth, windowHeight), mouse_press, gm->getTextureShader());

        } else { // GAME

            if (start_time == -1.0)
                start_time = glfwGetTime();

            x = gm->physicsWorld->dynamicsWorld->getGravity().z();
            z = -gm->physicsWorld->dynamicsWorld->getGravity().x();

            // Rotate board
            //gm->Model = glm::translate(glm::mat4(1), glm::vec3(0,-8,0));
            gm->Model = glm::mat4(1.0f);
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
                //PlaySound(TEXT("suction.wav"), NULL, SND_FILENAME);
                //system("canberra-gtk-play -f suction.wav");


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

            if (gm->win) {
                if (final_score == -1.0)
                    final_score = glfwGetTime()-start_time;
                char text[256];
                sprintf(text,"Score: %.2f", final_score );
                printText2D(text, 200, 250, 30);

                char text2[256];
                sprintf(text2,"Venceu!!!");
                printText2D(text2, 100, 300, 70);

            } else {
                char text[256];
                sprintf(text,"Score: %.2f", glfwGetTime()-start_time );
                printText2D(text, 400, 560, 30);
            }

        }

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