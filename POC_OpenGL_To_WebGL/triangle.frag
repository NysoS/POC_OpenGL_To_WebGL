#version 430 core

out vec4 fragColor;

in vec3 oColor;

void main()
{
	//fragColor = vec4((pos.x + 1.) / 2, (pos.y + 1.) / 2, pos.z, 1.0);
	fragColor = vec4(oColor, 1.0);
}