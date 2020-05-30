#version 330 core

// Input vertex data, different for all executions of this shader.
layout(location = 0) in vec3 vertex;
layout(location = 1) in vec3 vertexColor;
layout(location = 2) in vec3 vertexNormals;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 camera;
uniform mat4 model;

// Output data ; will be interpolated for each fragment.
out vec3 fragmentVertice;
out vec3 fragmentColor;
out vec3 fragmentNormal;

void main(){	
	// Pass some variables to the fragment shader
	fragmentColor = vertexColor;
    fragmentNormal = vertexNormals;
    fragmentVertice = vec3(model * vec4(vertex, 1.0));

	// Output position of the vertex, in clip space : MVP * position
	gl_Position =  camera * model * vec4(vertex,1);
}
