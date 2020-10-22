#version 410

layout(location = 1) in vec3 inColor;

out vec4 frag_color;

void main() { 
	
	frag_color = vec4(1.0-inColor.r,1.0-inColor.g,1.0-inColor.b, 1.0);
}