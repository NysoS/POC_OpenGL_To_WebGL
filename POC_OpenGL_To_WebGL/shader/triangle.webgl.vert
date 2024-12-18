#version 300 es

layout (location = 0) in vec2 position;
layout (location = 1) in vec3 color;

out vec3 oColor;

void main()
{
	gl_Position = vec4(position, 0, 1.0);
	
	oColor = color;
}