#version 330 core

in vec3 col;
in vec3 nor;
in vec3 FragPos;

uniform vec3 lightColour;
uniform vec3 lightDirection;
uniform vec3 camPos;


out vec4 fragColour;

void main()
{
	float ambient, diffuse, specular;
	ambient = 0.2f;

	vec3 Nnor = normalize(nor); 
	vec3 Nto_light = normalize(-lightDirection);
	
	diffuse = max(dot(Nnor, Nto_light), 0.f);

	vec3 Nfrom_light = normalize(lightDirection);	
	vec3 NrefLight = reflect(Nfrom_light, Nnor);
	
	vec3 camDirection = camPos - FragPos;
	vec3 NcamDirection = normalize(camDirection);

	specular = pow(max(dot(NcamDirection, NrefLight), 0), 128);


	float phong = ambient + diffuse + specular;

	fragColour = vec4(phong * col * lightColour, 1.f);

}
