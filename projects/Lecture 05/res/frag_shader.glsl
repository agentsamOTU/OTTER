#version 410

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 2) in vec3 inNormal;

uniform vec3 LightPos;


out vec4 frag_color;


void main() {
	vec3 lightColor = vec3(1.0,1.0,1.0);

	float ambientStrength = 0.05;
	vec3 ambient = ambientStrength * lightColor * inColor;

	//diffuse
	vec3 N = normalize(inNormal);
	vec3 lightDir = normalize(LightPos-inPosition);

	float dif = max(dot(N,lightDir),0.0);
	vec3 diffuse = dif * inColor;

	//attenuation
	float dist = length(LightPos-inPosition);
	diffuse = diffuse / dist;

	//specular
	vec3 camPos = vec3(0.0,0.0,3.0);
	float specularStrength = 1.0;

	vec3 camDir = normalize(camPos-inPosition);
	vec3 halfway = normalize(camDir+lightDir);

	vec3 reflectDir = reflect(-lightDir,N);
	float spec = pow(max(dot(inNormal,halfway),0.0),128);//shininess coefficient

	vec3 specular =specularStrength*spec*lightColor;

	vec3 result = ambient+diffuse+specular;

	frag_color = vec4(result, 1.0);
}