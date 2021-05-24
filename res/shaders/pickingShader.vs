#version 330

attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
attribute vec2 texCoords;

uniform mat4 MVP;
uniform mat4 Normal;

//out vec3 color0;

void main()
{
	//color0 = color;
	gl_Position = MVP *Normal* vec4(position, 1.0);
}
