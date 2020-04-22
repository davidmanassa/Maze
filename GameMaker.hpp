#ifndef GAMEMAKER_H
#define GAMEMAKER_H

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// GLM header file
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
using namespace glm;

// shaders header file
#include "common/shader.hpp"

#include "MapGenerator.hpp"

/**
        0.0f,  1.0f,  1.0f,    0.0f,  1.0f,  1.0f,    0.0f,  1.0f,  1.0f,    // cyan
        0.0f,  1.0f,  0.0f,    0.0f,  1.0f,  0.0f,    0.0f,  1.0f,  0.0f,    // green
        1.0f,  0.0f,  0.0f,    1.0f,  0.0f,  0.0f,    1.0f,  0.0f,  0.0f,    // red
        1.0f,  0.0f,  1.0f,    1.0f,  0.0f,  1.0f,    1.0f,  0.0f,  1.0f,    // magenta
        1.0f,  1.0f,  0.0f,    1.0f,  1.0f,  0.0f,    1.0f,  1.0f,  0.0f,    // yellow
        0.0f,  0.0f,  1.0f,    0.0f,  0.0f,  1.0f,    0.0f,  0.0f,  1.0f,    // blue
        1.0f,  0.0f,  1.0f,    1.0f,  0.0f,  1.0f,    1.0f,  0.0f,  1.0f     // magenta
        1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,    1.0f,  1.0f,  1.0f,    // white
**/

class GameMaker {

    private:

        // Vertex buffer object (VBO)
        GLuint cubeVertexBuffer;
        GLuint floorVertexBuffer;

        // color buffer object (CBO)
        GLuint cubeColorbuffer;
        GLuint floorColorBuffer;

        MapGenerator mg = MapGenerator();

        void transferCubeToGPUMemory(void);
        void transferFloorToGPUMemory(void);

    public:
        // Vertex array object (VAO)
        GLuint VertexArrayID;

        // GLSL program from the shaders
        GLuint programID;

        // Matrix id of the MVP
        GLuint MatrixID;

        glm::mat4 MVP;

        glm::mat4 Model = glm::mat4(1.0f);

        GLfloat delta = 0.75f;
        GLfloat angulo = 0.0f;

        glm::mat4 Projection = glm::mat4(1.0f);
        glm::mat4 View = glm::mat4(1.0f);

        GLfloat cameraAtX = 0, cameraAtY = 15, cameraAtZ = -10;
        GLfloat lookAtX = 0, lookAtY = 0, lookAtZ = 0;

        GameMaker(int mazeHeight, int mazeWidth);

        void transferDataToGPUMemory(void);
        void setMVP(void);
        void drawCube(GLfloat transX, GLfloat transZ);
        void drawFloor(GLfloat transX, GLfloat transZ);
        void cleanupDataFromGPU();
        void drawMap();

};

#endif