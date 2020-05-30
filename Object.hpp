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

// RETIRAR VERSÃO FINAL
//#include <glm/gtx/string_cast.hpp>

class Object {

    private:

        std::vector< glm::vec3 > vertices;
        std::vector< glm::vec2 > uvs;
        std::vector< glm::vec3 > normals;
        std::vector< glm::vec3 > colors;


        glm::vec3 material_ambient;
        glm::vec3 material_diffuse;
        glm::vec3 material_specular;
        float material_shininess; // 0.1 * 128 padrão

        glm::vec3 light_pos;
        glm::vec3 view_pos; // Camera position

        glm::vec3 light_color;
        glm::vec3 light_color_diffuse;
        glm::vec3 light_color_ambient;

        GLuint VAO;
        
        GLuint VBO_vertex;
        GLuint VBO_uvs;
        GLuint VBO_normals;
        GLuint VBO_colors;


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

        void loadToGPU(const GLfloat object_buffer[], const GLfloat object_uv_color[], int totalVertices);
        void loadOBJtoGPU(const char path[]);
        void loadOBJtoGPU(const char path[], glm::vec3 color);

        void setMVP(glm::mat4 Projection, glm::mat4 View, glm::mat4 Model);
        void setTranslate(vec3 translate);
        void setScale(vec3 scale);

        void loadLight(vec3 light_color_ambient, vec3 light_color_diffuse);

        void drawWithTextureShader(GLuint texture, GLuint shader);
        void drawWithMaterialShader(vec3 light_pos, vec3 view_pos, GLuint shader);
        void drawWithLampShader(vec3 objectColor, GLuint shader);
        void drawWithStandardShader(GLuint texture, vec3 lightPos, GLuint shader);
        void drawWithPhongShader(vec3 cameraAt, vec3 lightPos, GLuint shader);
        void drawWithBasicLightningShader(vec3 lightPos, vec3 lightColor, vec3 objectColor, GLuint shader);


};

#endif