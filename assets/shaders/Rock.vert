#version 460 core

layout (location = 0) in vec3 pos;

/*
uniform int ScaleX;
uniform int ScaleY;
uniform int ScaleUnit;
*/

void main()
{
	gl_Position = vec4(pos,1.0);
}