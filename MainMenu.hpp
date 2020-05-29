#ifndef MAINMENU_HPP
#define MAINMENU_HPP

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
#include "common/text2D.hpp"

class MainMenu {

    public:

        bool isOn = true; // Start by Main menu

        void load();
        void draw(vec2 mousePos, vec2 windowDimensions, bool pressed);

    private:

        void setMVP();

        glm::mat4 MVP;

        GLuint MatrixID; // Matrix id of the MVP

        GLuint shader;

        GLuint texture_normal;
        GLuint textureID_normal;
        GLuint texture_hover;
        GLuint textureID_hover;
        GLuint texture_pressed;
        GLuint textureID_pressed;

        // VBO
        GLuint buttonVertexBuffer;

        // CBO
        GLuint buttonUVbuffer;

};

#endif