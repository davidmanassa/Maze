#include "Object.hpp"

Object::Object() {


}

Object::Object(glm::vec3 material_ambient, glm::vec3 material_diffuse, glm::vec3 material_specular, float material_shininess) {

    Object::material_ambient = material_ambient;
    Object::material_diffuse = material_diffuse;
    Object::material_specular = material_specular;
    Object::material_shininess = material_shininess;

}

void Object::loadToGPU(const GLfloat object_buffer[], const GLfloat object_uv_color[], int totalVertices) {

    std::cout << object_buffer << std::endl;

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &VBO_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glBufferData(GL_ARRAY_BUFFER, sizeof(object_buffer), object_buffer, GL_STATIC_DRAW);
    
    glGenBuffers(1, &VBO_uvs);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
	glBufferData(GL_ARRAY_BUFFER, sizeof(object_uv_color), object_uv_color, GL_STATIC_DRAW);

    objSize = totalVertices;

}

// For phong shader
void Object::loadOBJtoGPU(const char path[], glm::vec3 color) {

    bool res = loadOBJ(path, vertices, uvs, normals);

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &VBO_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        
    // Move color data to video memory; specifically to CBO called colorbuffer
    glGenBuffers(1, &VBO_uvs);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() *sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    // move normal to VRAM
    glGenBuffers(1, &VBO_normals);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() *sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);


    for(int i = 0; i < vertices.size(); i++){
        colors.push_back(color);
    }

    glGenBuffers(1, &VBO_colors);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
    glBufferData(GL_ARRAY_BUFFER, colors.size() *sizeof(glm::vec3), &colors[0], GL_STATIC_DRAW);

    objSize = vertices.size()*3;

}

void Object::loadOBJtoGPU(const char path[]) {

    bool res = loadOBJ(path, vertices, uvs, normals);

    // VAO
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Move vertex data to video memory; specifically to VBO called vertexbuffer
    glGenBuffers(1, &VBO_vertex);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);
        
    // Move color data to video memory; specifically to CBO called colorbuffer
    glGenBuffers(1, &VBO_uvs);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
    glBufferData(GL_ARRAY_BUFFER, uvs.size() *sizeof(glm::vec2), &uvs[0], GL_STATIC_DRAW);

    // move normal to VRAM
    glGenBuffers(1, &VBO_normals);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, normals.size() *sizeof(glm::vec3), &normals[0], GL_STATIC_DRAW);

    objSize = vertices.size()*3;

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

void Object::drawWithTextureShader(GLuint texture, GLuint shader) {

    glUseProgram(shader);

    MVP =  Projection * View * Model * Translate;

    // Send our transformation to the currently Vid shader,
    // in the "MVP" uniform, which is now MVP
    glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
    // Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(glGetUniformLocation(shader, "myTextureSampler"), 0);
    
    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
		glVertexAttribPointer(
			1,                                // attribute. No particular reason for 1, but must match the layout in the shader.
			2,                                // size : U+V => 2
			GL_FLOAT,                         // type
			GL_FALSE,                         // normalized?
			0,                                // stride
			(void*)0                          // array buffer offset
		);;
    
    glDrawArrays(GL_TRIANGLES, 0, objSize);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
}

void Object::drawWithMaterialShader(vec3 lightPos, vec3 viewPos, GLuint shader) {
    
    glUseProgram(shader);

    MVP = Projection * View * Model * Translate * Scale;


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

    glm::mat4 Model1 = Model * Translate * Scale;

    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &Model1[0][0]);

        // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);;

        // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);;
    
    glDrawArrays(GL_TRIANGLES, 0, objSize);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}

void Object::drawWithLampShader(vec3 objectColor, GLuint shader) {

    glUseProgram(shader);

    MVP = Projection * View * Model * Translate * Scale;

    glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);

    glUniform3f(glGetUniformLocation(shader, "objectColor"), objectColor.x, objectColor.y, objectColor.z);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);;

        // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);;
    
    glDrawArrays(GL_TRIANGLES, 0, objSize);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}

void Object::drawWithStandardShader(GLuint texture, vec3 lightPos, GLuint shader) {

    glUseProgram(shader);

    MVP =  Projection * View * Model * Translate;

    // Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(glGetUniformLocation(shader, "MVP"), 1, GL_FALSE, &MVP[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "M"), 1, GL_FALSE, &Model[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "V"), 1, GL_FALSE, &View[0][0]);

	//glm::vec3 lightPos = glm::vec3(4,4,4);
	glUniform3f(glGetUniformLocation(shader, "LightPosition_worldspace"), light_pos.x, light_pos.y+10, light_pos.z); // ATTENTION

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set our "myTextureSampler" sampler to use Texture Unit 0
	glUniform1i(glGetUniformLocation(shader, "myTextureSampler"), 0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);;

        // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);;
    
    glDrawArrays(GL_TRIANGLES, 0, objSize);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}

void Object::drawWithBasicLightningShader(vec3 lightPos, vec3 lightColor, vec3 objectColor, GLuint shader) {

    glUseProgram(shader);

    glm::mat4 Model1 = Model * Translate * Scale;

	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &Model1[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, &View[0][0]);
	glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, &Projection[0][0]);

	glUniform3f(glGetUniformLocation(shader, "lightPos"), lightPos.x, lightPos.y+10, lightPos.z); /// ATTENTION
    glUniform3f(glGetUniformLocation(shader, "lightColor"), lightColor.x, lightColor.y, lightColor.z);
    glUniform3f(glGetUniformLocation(shader, "objectColor"), objectColor.x, objectColor.y, objectColor.z);


    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_uvs);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);;

        // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);;
    
    glDrawArrays(GL_TRIANGLES, 0, objSize);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}

void Object::drawWithPhongShader(vec3 cameraAt, vec3 lightPos, GLuint shader) {

    glUseProgram(shader);

    Model = Model * Translate;

    glUniformMatrix4fv(glGetUniformLocation(shader, "camera"), 1, GL_FALSE, &(Projection*View)[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, &Model[0][0]);
    // glm::vec3 cp(0,0,0);
    // glUniform3fv(glGetUniformLocation(shader, "cameraPosition"), 1, value_ptr(cp));
    glUniform3f(glGetUniformLocation(shader, "cameraPosition"), cameraAt.x, cameraAt.y, cameraAt.z);
    glUniform1f(glGetUniformLocation(shader, "materialShininess"), 32);
    // std::cout << "specular strength "<< specularStrength << std::endl;
    glUniform1f(glGetUniformLocation(shader, "specularStrength"), 0.15f);
    // std::cout << "specular color ("<< specularColor.x << ", " << specularColor.y << ", "<<specularColor.z << ")" << std::endl;
    glUniform3f(glGetUniformLocation(shader, "materialSpecularColor"), 1, 1, 1);
    // glUniform3fv(glGetUniformLocation(shader, "light.position"), 1, value_ptr(cp));
    glUniform3f(glGetUniformLocation(shader, "light.position"), lightPos.x, lightPos.y, lightPos.z);
    // std::cout << "light color ("<< light->color.x << ", " << light->color.y << ", "<<light->color.z << ")" << std::endl;
    glUniform3f(glGetUniformLocation(shader, "light.intensities"), 1.0f, 1.0f, 1.0f);
    glUniform1f(glGetUniformLocation(shader, "light.attenuation"), 0.1f);
    // float ac = 1.0f;
    // glUniform1fv(glGetUniformLocation(shader, "light.ambientCoefficient"), 1, &ac);
    glUniform1f(glGetUniformLocation(shader, "light.ambientCoefficient"),  0.3f);
    glUniform3f(glGetUniformLocation(shader, "light.ambientColor"), 19/255.0, 9/255.0, 22/255.0);

    // 1rst attribute buffer : vertices
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_vertex);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_colors);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);;

        // 2nd attribute buffer : UVs
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normals);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);;
    
    glDrawArrays(GL_TRIANGLES, 0, objSize);
    
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(2);

}