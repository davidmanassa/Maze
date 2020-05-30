#include "CubeMap.hpp"

CubeMap::~CubeMap() {

    glDeleteBuffers(1, &cubeVertexBuffer);
    glDeleteBuffers(1, &cubeUVbuffer);

}

void CubeMap::load() {

    // load Textures
    texture = (GLuint*) malloc(6 * sizeof(GLuint));

    texture[1] = loadBMP_custom("images/cubemap/bottom.bmp");
    texture[0] = loadBMP_custom("images/cubemap/top.bmp");
    texture[4] = loadBMP_custom("images/cubemap/front.bmp");
    texture[5] = loadBMP_custom("images/cubemap/back.bmp");
    texture[2] = loadBMP_custom("images/cubemap/left.bmp");
    texture[3] = loadBMP_custom("images/cubemap/right.bmp");
    

    // ORDER -> top - left - front - right - back - bottom

    float cubeVertices[] = {
        -30.0f, -30.0f, -30.0f, // top
         30.0f, -30.0f, -30.0f, 
         30.0f,  30.0f, -30.0f,  
         30.0f,  30.0f, -30.0f, 
        -30.0f,  30.0f, -30.0f,  
        -30.0f, -30.0f, -30.0f, 

        -30.0f, -30.0f,  30.0f,  // bottom
         30.0f, -30.0f,  30.0f,  
         30.0f,  30.0f,  30.0f,  
         30.0f,  30.0f,  30.0f,  
        -30.0f,  30.0f,  30.0f,  
        -30.0f, -30.0f,  30.0f, 

        -30.0f,  30.0f,  30.0f,  // left
        -30.0f,  30.0f, -30.0f,
        -30.0f, -30.0f, -30.0f, 
        -30.0f, -30.0f, -30.0f, 
        -30.0f, -30.0f,  30.0f,  
        -30.0f,  30.0f,  30.0f,  

         30.0f,  30.0f,  30.0f, //right
         30.0f,  30.0f, -30.0f, 
         30.0f, -30.0f, -30.0f,  
         30.0f, -30.0f, -30.0f,  
         30.0f, -30.0f,  30.0f,  
         30.0f,  30.0f,  30.0f,  

        -30.0f, -30.0f, -30.0f,  //front
         30.0f, -30.0f, -30.0f, 
         30.0f, -30.0f,  30.0f,  
         30.0f, -30.0f,  30.0f,  
        -30.0f, -30.0f,  30.0f,  
        -30.0f, -30.0f, -30.0f,  

        -30.0f,  30.0f, -30.0f,  // back
         30.0f,  30.0f, -30.0f,  
         30.0f,  30.0f,  30.0f,  
         30.0f,  30.0f,  30.0f,  
        -30.0f,  30.0f,  30.0f,  
        -30.0f,  30.0f, -30.0f,  
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

    GLubyte indices[36];
	
    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &cubeVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    
    glGenBuffers(1, &cubeUVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, cubeUVbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeUVs), cubeUVs, GL_STATIC_DRAW);

}

void CubeMap::draw(glm::mat4 MVP, GLuint shader) {

    glUseProgram(shader);

    glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(-15.0f, 15.0f, -15.0f));
    glm::mat4 MVP2 = MVP * translate;

    for (int i = 0; i < 6; i++) {

        // Send our transformation to the currently bound shader,
        // in the "MVP" uniform, which is now MVP
        glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP2[0][0]);
        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);

        glBindTexture(GL_TEXTURE_2D, texture[i]);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(glGetUniformLocation(shader, "myTextureSampler"), 0);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, cubeVertexBuffer);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        // 2nd attribute buffer : UVs
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, cubeUVbuffer);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);
        
        glDrawArrays(GL_TRIANGLES, (i*6), (2*3));

        glDisableVertexAttribArray(0);
        glDisableVertexAttribArray(1);

    }

}
