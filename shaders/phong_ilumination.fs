#version 330 core

uniform mat4 model;
uniform vec3 cameraPosition;

uniform float materialShininess;
uniform float specularStrength;
uniform vec3 materialSpecularColor;

uniform struct Light {
   vec3 position;
   vec3 intensities; //a.k.a the color of the light
   float attenuation;
   float ambientCoefficient;
} light;

in vec3 fragmentVertice;
in vec3 fragmentColor;
in vec3 fragmentNormal;


// Ouput data
out vec4 color;

void main(){
	// vec3 norm = normalize(transpose(inverse(mat3(model))) * fragmentNormal);
	// vec3 lightDir = normalize(light.position - fragmentVertice);  

	// float diff = max(dot(norm, lightDir), 0.0);
	// vec3 diffuse = diff * light.intensities;

	// vec3 result = (0.1 + diffuse) * fragmentColor;
	// color = vec4(result, 1.0);
	// color = vec4(fragmentColor, 1.0);

	vec3 normal = normalize(transpose(inverse(mat3(model))) * fragmentNormal);
	vec3 surfacePos = vec3(model * vec4(fragmentVertice, 1));
	vec4 surfaceColor = vec4(fragmentColor.rgb, 1);
	vec3 surfaceToLight = normalize(light.position - surfacePos);
	vec3 surfaceToCamera = normalize(cameraPosition - surfacePos);

	// // ambient
	vec3 ambient = light.ambientCoefficient * surfaceColor.rgb * light.intensities;

	float diffuseCoefficient = max(0.0, dot(normal, surfaceToLight));
	vec3 diffuse = diffuseCoefficient * surfaceColor.rgb * light.intensities;

	// specular
	float specularCoefficient = 0.0;
	if(diffuseCoefficient > 0.0)
		specularCoefficient = pow(max(0.0, dot(surfaceToCamera, reflect(-surfaceToLight, normal))), materialShininess);
	vec3 specular = specularCoefficient * materialSpecularColor * specularStrength * light.intensities;
	// vec3 specular = vec3(0.0);

	// attenuation
	float distanceToLight = length(light.position - surfacePos);
	float attenuation = 1.0 / (1.0 + light.attenuation * pow(distanceToLight, 2));

	// linear color (color before gamma correction)
	vec3 linearColor = ambient + attenuation*(diffuse + specular);

	// //final color (after gamma correction)
	// vec3 gamma = vec3(1.0/2.2);
	color = vec4(linearColor.rgb, surfaceColor.a);
	// color = vec4(vec3(specularStrength),1);
	// color = vec4(pow(linearColor, gamma), surfaceColor.a);
}