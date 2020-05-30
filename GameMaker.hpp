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
#include "common/text2D.hpp"
#include "common/controls.hpp"

#include "MapGenerator.hpp"
#include "Physics.hpp"
#include "Object.hpp"
#include "CubeMap.hpp"

//#include <glm/gtx/string_cast.hpp>

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

        CubeMap* cubeMap;

        int mazeHeight, mazeWidth;

        // Vertex buffer object (VBO)
        GLuint cubeVertexBuffer;
        GLuint floorVertexBuffer;
        GLuint playerVertexBuffer;
        GLuint holeVertexBuffer;

        GLuint uvbuffer;
        GLuint uvbuffer_floor;
        GLuint uvbuffer_hole;
        GLuint uvbuffer_player;

        MapGenerator mg = MapGenerator();

        void transferCubeToGPUMemory(void);
        void transferFloorToGPUMemory(void);
        void transferHoleToGPUMemory(void);

    public:

        Object *player;
        Object *floor, *cube, *hole;

        GLuint Texture_crate;
        GLuint Texture_floor;
        GLuint *Texture_hole;
        GLuint Texture_player;
        GLuint uvmap;

        btVector3 start_point = btVector3(1 + 0.5f, 0.5f, 1 + 0.5f);
        btVector3 end_point = btVector3(1 + 0.5f, 0.5f, 1 + 0.5f);

        bool win = false;


        Physics::PhysicsWorld* physicsWorld;

        Physics::PhysicsBody* playerBody;
        std::vector<Physics::PhysicsBody*> objectBodies;

        // Vertex array object (VAO)
        GLuint VertexArrayID;

        // GLSL program from the shaders
        GLuint shaderTexture;
        GLuint shaderLamp;
        GLuint shaderMaterial;
        GLuint shaderMaterialAndTexture;
        GLuint shaderBasicLightning;
        GLuint shaderStandard;
        GLuint shaderPhong;

        glm::mat4 MVP = glm::mat4(1.0f);

        glm::mat4 Model = glm::mat4(1.0f);

        GLfloat delta = 0.75f;
        GLfloat angulo = 0.0f;

        glm::mat4 Projection = glm::mat4(1.0f);
        glm::mat4 View = glm::mat4(1.0f);

        vec3 cameraAt = vec3(0.0f, 30.0f, 0.001f);
        vec3 lookAt = vec3(0.0f, 0.0f, 0.0f);

        GameMaker(int mazeHeight, int mazeWidth);
        GameMaker(int mazeHeight, int mazeWidth, Physics::PhysicsWorld *pw);

        ~GameMaker();

        void transferDataToGPUMemory(void);
        void loadPlayer();
        glm::mat4 setMVP(void);
        void drawCube(glm::vec3 trans);
        void drawFloor(glm::vec3 trans, bool start, bool end);
        void drawHole(glm::vec3 trans);
        void cleanupDataFromGPU();
        void drawMap();
        void drawPlayer();

        void attachPhysicsWorld(Physics::PhysicsWorld* pw) {physicsWorld = pw;};
        void attachPlayerBody(Physics::PhysicsBody* pb1) {playerBody = pb1;};

        void update(double dt);
        void loadPhysics();

        GLuint getTextureShader() {
            return shaderTexture;
        }

};

#endif