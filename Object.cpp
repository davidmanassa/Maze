#include "Object.hpp"

Object::Object() {


}

Object::Object(glm::vec3 material_ambient, glm::vec3 material_diffuse, glm::vec3 material_specular, float material_shininess) {

    Object::material_ambient = material_ambient;
    Object::material_diffuse = material_diffuse;
    Object::material_specular = material_specular;
    Object::material_shininess = material_shininess;

}

void Object::loadToGPU(GLfloat object_buffer[], GLfloat object_uv_color[], int vertexNumber, int colorNumber) {

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(object_buffer), object_buffer, GL_STATIC_DRAW);
    
    glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(object_uv_color), object_uv_color, GL_STATIC_DRAW);

    vertex_number = vertexNumber;
    color_vertex_number = colorNumber;

}

void Object::loadOBJtoGPU(const char path[]) {

    std::vector< glm::vec3 > vertices;
    std::vector< glm::vec2 > uvs;
    std::vector< glm::vec3 > normals; // Won't be used at the moment.
    bool res = loadOBJ(path, vertices, uvs, normals);

    // One color for each vertex. They were generated randomly.
    static const GLfloat g_uv_buffer_data_floor[] = {

        0.0f,0.0f,  1.0f,0.0f,  0.0f,1.0f,
        1.0f,1.0f,  1.0f,0.0f,  0.0f,1.0f,

    };

	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data_floor), g_uv_buffer_data_floor, GL_STATIC_DRAW);

    objSize = vertices.size();

}


void Object::loadToGPU(char img_path[], GLfloat object_buffer[], GLfloat object_uv_color[], int vertexNumber, int colorNumber) {

    texture = loadBMP_custom(img_path);

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(object_buffer), object_buffer, GL_STATIC_DRAW);
    
    glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(object_uv_color), object_uv_color, GL_STATIC_DRAW);

    vertex_number = vertexNumber;
    color_vertex_number = colorNumber;

}

void Object::setTranslate(glm::vec3 translate) {
    Object::Translate = glm::translate(glm::mat4(1.0f), translate);
    world_pos = translate;
}

void Object::setScale(glm::vec3 scale) {
    Object::Scale = glm::scale(glm::mat4(1.0f), scale);
}

void Object::setMVP(glm::mat4 Projection, glm::mat4 View, glm::mat4 Model) {

    Object::Projection = Projection;
    Object::View = View;
    Object::Model = Model;

}

void Object::loadLight(vec3 light_color_ambient, vec3 light_color_diffuse) {
    Object::light_color_ambient = light_color_ambient;
    Object::light_color_diffuse = light_color_diffuse;
}

void Object::drawWithMaterialShader(vec3 light_pos, vec3 view_pos, GLuint shader) {
    
    glUseProgram(shader);

    //glm::mat4 translate = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 30.0f, 0.0f));
    //glm::mat4 MVP2 = MVP * translate;

    // Send our transformation to the currently bound shader,
    // in the "MVP" uniform, which is now MVP
    glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    // Bind our texture in Texture Unit 0

   // if (texture != NULL) {
      //  glActiveTexture(GL_TEXTURE0);

      //  glBindTexture(GL_TEXTURE_2D, texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
     //   glUniform1i(glGetUniformLocation(shader, "myTextureSampler"), 0);
    //}

    glVertexAttrib3f(glGetUniformLocation(shader, "light.position"), light_pos.x, light_pos.y, light_pos.z);
    glVertexAttrib3f(glGetUniformLocation(shader, "light.ambient"), light_color_ambient.x, light_color_ambient.y, light_color_ambient.z);
    glVertexAttrib3f(glGetUniformLocation(shader, "light.diffuse"), light_color_diffuse.x, light_color_diffuse.y, light_color_diffuse.z);

    glVertexAttrib3f(glGetUniformLocation(shader, "viewPos"), view_pos.x, view_pos.y, view_pos.z);

    glVertexAttrib3f(glGetUniformLocation(shader, "material.ambient"), material_ambient.x, material_ambient.y, material_ambient.z);
    glVertexAttrib3f(glGetUniformLocation(shader, "material.diffuse"), material_diffuse.x, material_diffuse.y, material_diffuse.z);
    glVertexAttrib3f(glGetUniformLocation(shader, "material.specular"), material_specular.x, material_specular.y, material_specular.z);
    glVertexAttrib1f(glGetUniformLocation(shader, "material.shininess"), material_shininess);

    glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &Projection[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &View[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &Model[0][0]);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    int uvOrColor = vertex_number > color_vertex_number ? 2 : 3;
    glVertexAttribPointer(1, uvOrColor, GL_FLOAT, GL_FALSE, 0, (void*)0);;
    
    glDrawArrays(GL_TRIANGLES, 0, vertex_number);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}

void Object::drawWithLampShader(vec3 objectColor, GLuint shader) {

    glUseProgram(shader);

    MVP = Projection * View * Model * Translate * Scale;

    glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);

    glUniform3f(glGetUniformLocation(shader, "objectColor"), objectColor.x, objectColor.y, objectColor.z);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);;
    
    glDrawArrays(GL_TRIANGLES, 0, objSize);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

}
