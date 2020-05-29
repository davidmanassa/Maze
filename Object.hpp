#ifndef OBJECT_H
#define OBJECT_H

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

class Object {

    private:

        glm::vec3 material_ambient;
        glm::vec3 material_diffuse;
        glm::vec3 material_specular;
        float material_shininess; // 0.1 * 128 padrão

        glm::vec3 light_pos;
        glm::vec3 view_pos; // Camera position

        glm::vec3 light_color;
        glm::vec3 light_color_diffuse;
        glm::vec3 light_color_ambient;

        GLuint texture;

        // VAO
        GLuint vertexBuffer;

        // CBO
        GLuint colorBuffer;


        int vertex_number;
        int color_vertex_number;

        glm::mat4 MVP = glm::mat4(1.0f);
        glm::mat4 Projection = glm::mat4(1.0f);
        glm::mat4 View = glm::mat4(1.0f);
        glm::mat4 Model = glm::mat4(1.0f);

        glm::mat4 Translate = glm::mat4(1.0f);
        glm::mat4 Scale = glm::mat4(1.0f);

        vec3 world_pos;

        int objSize;

    public:

        Object(glm::vec3 material_ambient, glm::vec3 material_diffuse, glm::vec3 material_specular, float material_shininess);
        Object();

        void loadToGPU(GLfloat object_buffer[], GLfloat object_uv_color[], int vertexNumber, int colorNumber);
        void loadToGPU(char img_path[], GLfloat object_buffer[], GLfloat object_uv_color[], int vertexNumber, int colorNumber);
        void loadOBJtoGPU(const char path[]);

        void setMVP(glm::mat4 Projection, glm::mat4 View, glm::mat4 Model);
        void setTranslate(vec3 translate);
        void setScale(vec3 scale);

        void loadLight(vec3 light_color_ambient, vec3 light_color_diffuse);

        void drawWithMaterialShader(vec3 light_pos, vec3 view_pos, GLuint shader);
        void drawWithLampShader(vec3 objectColor, GLuint shader);


};

#endif