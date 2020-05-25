#ifndef GAMEMAKER_H
#define GAMEMAKER_H

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

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
#include "common/texture.hpp"
#include "common/objloader.hpp"

#include "MapGenerator.hpp"

#include "Physics.hpp"

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

        int mazeHeight, mazeWidth;

        // Vertex buffer object (VBO)
        GLuint cubeVertexBuffer;
        GLuint floorVertexBuffer;
        GLuint playerVertexBuffer;
         GLuint holeVertexBuffer;

        // color buffer object (CBO)
        GLuint cubeColorbuffer;
        GLuint floorColorBuffer;
        GLuint playerColorBuffer;

        GLuint holeColorBuffer;

        int size = 0;

        MapGenerator mg = MapGenerator();

        void transferCubeToGPUMemory(void);
        void transferFloorToGPUMemory(void);
        void transferHoleToGPUMemory(void);

    public:
        GLuint Texture;
        GLuint TextureID;
         GLuint Texture_floor;
        GLuint TextureID_floor;
        GLuint Texture_hole;
        GLuint TextureID_hole;
        GLuint Texture_player;
        GLuint TextureID_player;
        GLuint uvbuffer;
        GLuint uvbuffer_floor;
        GLuint uvbuffer_hole;
         GLuint uvbuffer_player;
        btVector3 start_point = btVector3(1 + 0.5f, 0.5f, 1 + 0.5f);

        Physics::PhysicsWorld* physicsWorld;

        Physics::PhysicsBody* playerBody;
        std::vector<Physics::PhysicsBody*> objectBodies;

        // Vertex array object (VAO)
        GLuint VertexArrayID;

        // GLSL program from the shaders
        GLuint programID;
         GLuint programID2;

        // Matrix id of the MVP
        GLuint MatrixID;

        glm::mat4 MVP;

        glm::mat4 Model = glm::mat4(1.0f);

        GLfloat delta = 0.75f;
        GLfloat angulo = 0.0f;

        glm::mat4 Projection = glm::mat4(1.0f);
        glm::mat4 View = glm::mat4(1.0f);

        GLfloat cameraAtX = 0, cameraAtY = 20, cameraAtZ = 0.001;
        GLfloat lookAtX = 0, lookAtY = 0, lookAtZ = 0;

        GameMaker(int mazeHeight, int mazeWidth);
        GameMaker(int mazeHeight, int mazeWidth, Physics::PhysicsWorld *pw);

        void transferDataToGPUMemory(void);
        void loadPlayer();
        void setMVP(void);
        void drawCube(glm::vec3 trans);
        void drawFloor(glm::vec3 trans);
        void drawHole(glm::vec3 trans);
        void cleanupDataFromGPU();
        void drawMap();
        void drawPlayer(GLfloat scale);

        void attachPhysicsWorld(Physics::PhysicsWorld* pw) {physicsWorld = pw;};
        void attachPlayerBody(Physics::PhysicsBody* pb1) {playerBody = pb1;};

        void update(double dt);
        void loadPhysics();

};

#endif