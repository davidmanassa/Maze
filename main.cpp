#include <iostream>
#include <chrono>

#include "GameMaker.hpp"

GLFWwindow* window;

GLint windowWidth = 1024;
GLint windowHeight = 768;
const float ASPECT = float(windowWidth)/windowHeight;
int mazeHeight = 15, mazeWidth = 15;

GameMaker* gm;

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
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    if(yoffset > 0)
        gm->cameraAtY -= 0.25;
    else
        gm->cameraAtY += 0.25;
}

void handleKeyboardInput(GLFWwindow* window) {
    if(glfwGetKey(window,GLFW_KEY_W)) {
        gm->cameraAtZ += 0.25f;
        gm->lookAtZ += 0.25f;
    }
    if(glfwGetKey(window,GLFW_KEY_S)) {
        gm->cameraAtZ -= 0.25f;
        gm->lookAtZ -= 0.25f;
    }
    if(glfwGetKey(window,GLFW_KEY_D)) {
        gm->cameraAtX -= 0.25f;
        gm->lookAtX -= 0.25f;
    }
    if(glfwGetKey(window,GLFW_KEY_A)) {
        gm->cameraAtX += 0.25f;
        gm->lookAtX += 0.25f;
    }
    /**if(glfwGetKey(window,GLFW_KEY_UP)) {
        gm->playerBody->body->activate();
        btVector3 v = gm->playerBody->body->getLinearVelocity();
        if (v.getZ() < 5.9f)
            v.setZ(v.getZ() + 0.2f);
        gm->playerBody->body->setLinearVelocity(v);
    }
    if(glfwGetKey(window,GLFW_KEY_DOWN)) {
        gm->playerBody->body->activate();
        btVector3 v = gm->playerBody->body->getLinearVelocity();
        if (v.getZ() > -5.9f)
            v.setZ(v.getZ() - 0.2f);
        gm->playerBody->body->setLinearVelocity(v);
    }
    if(glfwGetKey(window,GLFW_KEY_RIGHT)) {
        gm->playerBody->body->activate();
        btVector3 v = gm->playerBody->body->getLinearVelocity();
        if (v.getX() > -5.9f)
            v.setX(v.getX() - 0.2f);
        gm->playerBody->body->setLinearVelocity(v);
    }
    if(glfwGetKey(window,GLFW_KEY_LEFT)) {
        gm->playerBody->body->activate();
        btVector3 v = gm->playerBody->body->getLinearVelocity();
        if (v.getX() < 5.9f)
            v.setX(v.getX() + 0.2f);
        gm->playerBody->body->setLinearVelocity(v);
    }**/

    // mouse input
	vec2 currentMousePos = getMousePosition(window);
	if(currentMousePos != lastMousePosition){
		vec2 gravity = currentMousePos - vec2(windowWidth/2, windowHeight/2);
        vec2 windowDimensions = vec2(windowWidth, windowHeight);
		float distanceFromCenter = std::min(length(gravity)/length(windowDimensions/vec2(2)),1.0f);
		// std::cout << "distance: " << distanceFromCenter << std::endl;
		gravity = normalize(gravity);
		// std::cout << "New gravity, old:" << gravity.x*5*distanceFromCenter << ", "<< gravity.y*5*distanceFromCenter << std::endl;
		gm->physicsWorld->dynamicsWorld->setGravity(btVector3(gravity.x*(3.14159265*7.25)*.8*distanceFromCenter,-10,gravity.y*(3.14159265*7.25)*.8*distanceFromCenter));
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

    Physics::PhysicsWorld pw = Physics::PhysicsWorld(btVector3(0, -10 , 0));
    GameMaker gm1 = GameMaker(mazeHeight, mazeWidth, &pw);
    gm = &gm1;

    // transfer my data (vertices, colors, and shaders) to GPU side
    glGenVertexArrays(1, &gm->VertexArrayID);
    glBindVertexArray(gm->VertexArrayID);

    gm->transferDataToGPUMemory();

    gm->loadPhysics();

    // set the model-view-projection matrix
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    bool cameraUpdated = false;
    auto last_time = std::chrono::high_resolution_clock::now();
	auto current_time = std::chrono::high_resolution_clock::now();

    vec2 lastMousePos = getMousePosition(window);

    double dt;
	float x, z;

    // render scene for each frame
    do {
        
        current_time  = std::chrono::high_resolution_clock::now();
		dt = std::chrono::duration<double, std::milli>(current_time-last_time).count()/1000.0;
		last_time = current_time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        handleKeyboardInput(window);

        x = gm->physicsWorld->dynamicsWorld->getGravity().z();
		z = -gm->physicsWorld->dynamicsWorld->getGravity().x();

        gm->setMVP(); // let us update the rotation angle

        // Rotate board
		gm->Model = glm::translate(glm::mat4(1), glm::vec3(0,-8,0));
		gm->Model = glm::rotate(gm->Model, glm::radians(x), vec3(1,0,0));
		gm->Model = glm::rotate(gm->Model, glm::radians(z), vec3(0,0,1));

        if (gm->playerBody->getWorldPosition().y < -10.0f) {
            gm->playerBody->body->activate();
            btVector3 v = gm->playerBody->body->getLinearVelocity();
            v.setX(0.0f);
            v.setY(0.0f);
            v.setZ(0.0f);
            gm->playerBody->body->setLinearVelocity(v);

            btTransform transform = gm->playerBody->body->getCenterOfMassTransform();
            transform.setOrigin(gm->start_point);
            gm->playerBody->body->setCenterOfMassTransform(transform);
        }

        gm->update(dt);

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