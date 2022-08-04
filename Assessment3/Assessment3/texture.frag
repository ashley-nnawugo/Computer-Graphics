#version 330 core

out vec4 fragColour;

in vec2 tex;
in vec3 nor;
in vec3 FragPos;


uniform sampler2D Texture;
uniform vec3 lightColour;
uniform vec3 lightDirection;
uniform vec3 lightSpotDirection;
uniform vec3 lightPos;
uniform vec3 lightSpotPos;
uniform vec3 camPos;

float CalculateDirectional()
{
	float ambient, diffuse, specular;
	ambient = 0.05f;

	vec3 Nnor = normalize(nor); 
	vec3 Nto_light = normalize(-lightDirection);
	
	diffuse = max(dot(Nnor, Nto_light), 0.f);

	vec3 Nfrom_light = normalize(lightDirection);	
	vec3 NrefLight = reflect(Nfrom_light, Nnor);
	
	vec3 camDirection = camPos - FragPos;
	vec3 NcamDirection = normalize(camDirection);

	specular = pow(max(dot(NcamDirection, NrefLight), 0), 128);


	float phong = ambient + diffuse + specular;

	return phong;
	
}

float CalculateAttenuation()
{
	float constant = 1.5;
	float linear = 0.05;
	float quadratic = 0.02;
	float d = length(lightSpotPos - FragPos);
	float attenuation = 1.0/(constant + (linear * d) +(quadratic *(d * d)));
	return attenuation;
}

float CalculatePositional()
{
	
	float ambient, diffuse, specular;
	ambient = 0.5f;
	vec3 Nnor = normalize(nor); 
	vec3 Nto_light = normalize(lightPos - FragPos);
	
	diffuse = max(dot(Nnor, Nto_light), 0.f);

	vec3 Nfrom_light = normalize(-Nto_light);	
	vec3 NrefLight = reflect(Nfrom_light, Nnor);
	
	vec3 camDirection = camPos - FragPos;
	vec3 NcamDirection = normalize(camDirection);

	specular = pow(max(dot(NcamDirection, NrefLight), 0), 128);

	float attenuation = CalculateAttenuation();

	float phong = (ambient + diffuse + specular) * attenuation;

	return phong;

}

float CalculateSpot()
{
	
	float ambient, diffuse, specular;
	ambient = 1.5f;
	vec3 Nnor = normalize(nor); 
	vec3 Nto_light = normalize(lightSpotPos - FragPos);
	
	diffuse = max(dot(Nnor, Nto_light), 0.f);

	vec3 Nfrom_light = -Nto_light;	
	vec3 NrefLight = reflect(Nfrom_light, Nnor);
	
	vec3 camDirection = camPos - FragPos;
	vec3 NcamDirection = normalize(camDirection);

	specular = pow(max(dot(NcamDirection, NrefLight), 0), 8);

	float attenuation = CalculateAttenuation();
	
	float phi = cos(radians(10.f));
	vec3 NSpotDir = normalize(lightSpotDirection);
	float theta = dot(Nfrom_light, NSpotDir);
	float phong;

	if(theta > phi)
	{
		phong = (ambient + diffuse + specular) * attenuation;
	}
	else
	{
		phong = ambient * attenuation;
	}

	return phong;

}

void main()
{
	float phongSpot = CalculateSpot();
	float phong = CalculatePositional();
	float phongD = CalculateDirectional();
   
    fragColour = (phongSpot +phongD) * texture(Texture, tex);
}
