#include "MainMenu.hpp"


void MainMenu::load() {

    //cubeMap = new CubeMap();
    //cubeMap->load();

    // load Textures
    texture_normal = loadBMP_custom("images/start_1_b.bmp");

    texture_hover = loadBMP_custom("images/start_2_b.bmp");

    texture_pressed = loadBMP_custom("images/sand.bmp");

    // Data to GPU memory
    static const GLfloat button[] = { // TWO TRIANGLES -> A RECTANGLE
        0.0f, 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  2.0f, 0.0f, 0.0f,
        2.0f, 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  2.0f, 0.0f, 0.0f
    };
    
    static const GLfloat g_uv_buffer_data_button[] = {
        0.0f, 1.0f,  0.0f, 0.0f,  1.0f, 1.0f,
        1.0f, 0.0f,  0.0f, 0.0f,  1.0f, 1.0f,
    };

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &buttonVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, buttonVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(button), button, GL_STATIC_DRAW);
    
    glGenBuffers(1, &buttonUVbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, buttonUVbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data_button), g_uv_buffer_data_button, GL_STATIC_DRAW);

}

void MainMenu::setMVP() {
    
    // Projection matrix : 45∞ Field of View, 4:3 ratio, display range : 0.1 unit <-> 100 units
    glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
    
    // Camera matrix
    glm::mat4 View = glm::lookAt(
            glm::vec3(0.0f, 20.0f, 0.001), // Camera at
            glm::vec3(0.0f, 0.0f, 0.0f), // looks at
            glm::vec3(0, 1, 0)  // Head is up
        );

    // Model matrix depends on the object, so it is defined somewhere in the program
    //glm::mat4 Model = glm::rotate(Model, glm::radians(angulo), glm::vec3(0,1,0));

    MVP = Projection * View;

}

void MainMenu::draw(vec2 mousePos, vec2 windowDimensions, bool pressed, GLuint shader) { // DO THIS FOR EACH BOTTON
    
    char text[256];

    sprintf(text,"3D MAZE");
    printText2D(text, 130, 400, 80);

    sprintf(text,"Carlos Esteves");
    printText2D(text, 550, 80, 17);
    
    sprintf(text,"David Pires");
    printText2D(text, 550, 50, 17);

    setMVP();
    //cubeMap->draw(MVP, shader);

    // draw menus
    glUseProgram(shader);

    glm::mat4 model;
    model = glm::translate(glm::mat4(1.0f), glm::vec3(-4.0f, 0.0f, 0.0f));
    model = glm::scale(model, vec3(4.0f, 0.0f, 2.0f));
    /**
     *  Buttom is 2x 1z
     *  Pos at 0, 0
     *  So: x 0 -> 2 z 0 -> 1 is Button
     * 
     *  Scale 4 0 2 ->
     *  x 0 -> 8 and z 0 -> 2
     * 
     *  Translation x -4
     *  x -4 -> 4 and z 0 -> 2
     *  
     **/
    
    MVP =  MVP * model;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, which is now MVP
    glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    // Bind our texture in Texture Unit 0
    glActiveTexture(GL_TEXTURE0);
    
    bool isHover = false;
    // check by coords
    float X = mousePos[0] - windowDimensions[0]/2;
    float Y = mousePos[1] - windowDimensions[1]/2;
    X = X/46;
    Y = - (Y/46);
    //std::cout << X << " " << Y << std::endl;
    if (X > -4 && X < 4 && Y > -2 && Y < 0) {
        isHover = true;
    }

    if (!isHover) {
        glBindTexture(GL_TEXTURE_2D, texture_normal);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(glGetUniformLocation(shader, "myTextureSampler"), 0);
    } else if (pressed) {
        glBindTexture(GL_TEXTURE_2D, texture_pressed);
        glUniform1i(glGetUniformLocation(shader, "myTextureSampler"), 0);
        isOn = false;
        //delete cubeMap;
    } else {
        glBindTexture(GL_TEXTURE_2D, texture_hover);
        glUniform1i(glGetUniformLocation(shader, "myTextureSampler"), 0);
    }

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, buttonVertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, buttonUVbuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);;
    
    glDrawArrays(GL_TRIANGLES, 0, 2*3);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    
}