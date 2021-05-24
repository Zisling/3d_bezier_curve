#version 330
attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
attribute vec2 texCoords;

out vec2 texCoord0;
out vec3 normal0;
out vec3 color0;
out vec3 position0;

uniform mat4 Normal;
uniform mat4 MVP;

void main()
{
    position0 = position;
    mat4 giantize = mat4(1) * 200;
    gl_Position = MVP * Normal * vec4(position, 1.0);
}  