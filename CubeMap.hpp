#ifndef CUBEMAP_HPP
#define CUBEMAP_HPP

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

class CubeMap {

    public:

        void load();
        void draw();

    private:

        GLuint shader;
        GLuint skyboxShader;

};

#endif