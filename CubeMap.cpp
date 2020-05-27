#include "CubeMap.hpp"

void CubeMap::load() {

     // Init Shader
    shader = LoadShaders("shaders/VertexShader.vert", "shaders/FragmentShader.frag");

    // load Textures
    texture = (GLuint*) malloc(6 * sizeof(GLuint));
    textureID = (GLuint*) malloc(6 * sizeof(GLuint));

    texture[0] = loadBMP_custom("images/crate.bmp");
    textureID[0] = glGetUniformLocation(texture[0], "myTextureSampler");
    texture[1] = loadBMP_custom("images/crate.bmp");
    textureID[1] = glGetUniformLocation(texture[1], "myTextureSampler");
    texture[2] = loadBMP_custom("images/crate.bmp");
    textureID[2] = glGetUniformLocation(texture[2], "myTextureSampler");
    texture[3] = loadBMP_custom("images/crate.bmp");
    textureID[3] = glGetUniformLocation(texture[3], "myTextureSampler");
    texture[4] = loadBMP_custom("images/crate.bmp");
    textureID[4] = glGetUniformLocation(texture[4], "myTextureSampler");
    texture[5] = loadBMP_custom("images/crate.bmp");
    textureID[5] = glGetUniformLocation(texture[5], "myTextureSampler");

    // ORDER -> top - left - front - right - back - bottom

    float cubeVertices[] = {
        -50.0f, -50.0f, -50.0f,
         50.0f, -50.0f, -50.0f, 
         50.0f,  50.0f, -50.0f,  
         50.0f,  50.0f, -50.0f, 
        -50.0f,  50.0f, -50.0f,  
        -50.0f, -50.0f, -50.0f, 

        -50.0f, -50.0f,  50.0f,  
         50.0f, -50.0f,  50.0f,  
         50.0f,  50.0f,  50.0f,  
         50.0f,  50.0f,  50.0f,  
        -50.0f,  50.0f,  50.0f,  
        -50.0f, -50.0f,  50.0f, 

        -50.0f,  50.0f,  50.0f,  
        -50.0f,  50.0f, -50.0f,
        -50.0f, -50.0f, -50.0f, 
        -50.0f, -50.0f, -50.0f, 
        -50.0f, -50.0f,  50.0f,  
        -50.0f,  50.0f,  50.0f,  

         50.0f,  50.0f,  50.0f,
         50.0f,  50.0f, -50.0f, 
         50.0f, -50.0f, -50.0f,  
         50.0f, -50.0f, -50.0f,  
         50.0f, -50.0f,  50.0f,  
         50.0f,  50.0f,  50.0f,  

        -50.0f, -50.0f, -50.0f,  
         50.0f, -50.0f, -50.0f, 
         50.0f, -50.0f,  50.0f,  
         50.0f, -50.0f,  50.0f,  
        -50.0f, -50.0f,  50.0f,  
        -50.0f, -50.0f, -50.0f,  

        -50.0f,  50.0f, -50.0f,  
         50.0f,  50.0f, -50.0f,  
         50.0f,  50.0f,  50.0f,  
         50.0f,  50.0f,  50.0f,  
        -50.0f,  50.0f,  50.0f,  
        -50.0f,  50.0f, -50.0f,  
    };


    float cubeUVs[] = {
        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        0.0f, 0.0f,
        1.0f, 0.0f,
        1.0f, 1.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        1.0f, 0.0f,
        1.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 1.0f,
        0.0f, 0.0f,
        1.0f, 0.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f,

        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f,
        0.0f, 1.0f

    };
	
    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &cubeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &cubeUVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeUVbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeUVs), cubeUVs, GL_STATIC_DRAW);

}

void CubeMap::draw(glm::mat4 MVP) {


    glUseProgram(shader);

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0f, 0.0f));
    glm::mat4 MVP2 = MVP * translate;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, which is now MVP
    glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP2[0][0]);
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);

    glBindTexture(GL_TEXTURE_2D, texture[0]);
    // Set our "myTextureSampler" sampler to use Texture Unit 0
    glUniform1i(textureID[0], 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, cubeUVbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);;
    
    glDrawArrays(GL_TRIANGLES, 0, 2*3*6);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}
