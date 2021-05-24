#version 330

//varying vec3 color0;

uniform sampler2D sampler;
uniform vec4 lightDirection;
uniform vec4 lightColor;

void main()
{
	gl_FragColor = lightColor;
}
