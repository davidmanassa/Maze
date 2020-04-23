#include <iostream>

#include "GameMaker.hpp"

GLFWwindow* window;

GLint windowWidth = 1024;
GLint windowHeight = 768;
int mazeHeight = 15, mazeWidth = 15;

GameMaker gm = GameMaker(mazeHeight, mazeWidth);


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS)
        gm.angulo += gm.delta;
    if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_RELEASE)
        gm.angulo -= gm.delta;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
        gm.angulo -= gm.delta;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
        gm.angulo += gm.delta;
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    std::cout << xoffset << " " << yoffset << "\n";
    if(yoffset>0)
        gm.cameraAtY -= 0.25;
    else
    {
        gm.cameraAtY += 0.25;
    }
    
    

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
    
    // White background
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    // Clear the screen
    glClear( GL_COLOR_BUFFER_BIT );
    
    // Enable depth test
    glEnable(GL_DEPTH_TEST);
    // Accept fragment if it closer to the camera than the former one
    glDepthFunc(GL_LESS);

    // transfer my data (vertices, colors, and shaders) to GPU side
    glGenVertexArrays(1, &gm.VertexArrayID);
    glBindVertexArray(gm.VertexArrayID);
    // Create and compile our GLSL program from the shaders
    gm.programID = LoadShaders("TransformVertexShader.vert", "ColorFragmentShader.frag");
    gm.transferDataToGPUMemory();

    // set the model-view-projection matrix
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // render scene for each frame
    do {
        
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        gm.setMVP(); // let us update the rotation angle

        if(glfwGetKey(window,GLFW_KEY_W)) {
            gm.cameraAtZ += 0.25f;
            gm.lookAtZ += 0.25f;
        }
        if(glfwGetKey(window,GLFW_KEY_S)) {
            gm.cameraAtZ -= 0.25f;
            gm.lookAtZ -= 0.25f;
        }
        if(glfwGetKey(window,GLFW_KEY_D)) {
            gm.cameraAtX -= 0.25f;
            gm.lookAtX -= 0.25f;
        }
        if(glfwGetKey(window,GLFW_KEY_A)) {
            gm.cameraAtX += 0.25f;
            gm.lookAtX += 0.25f;
        }

        gm.setMVP();
        gm.drawMap();

        // Swap buffers
        glfwSwapBuffers(window);
        // looking for events
        glfwPollEvents();
        

    } // Check if the ESC key was pressed or the window was closed
    while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0 );
    
    // Cleanup VAO, VBOs, and shaders from GPU
    gm.cleanupDataFromGPU();
    
    // Close OpenGL window and terminate GLFW
    glfwTerminate();
    
    return 0;
}