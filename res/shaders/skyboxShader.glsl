#version 330

in vec3 position0;

uniform samplerCube skybox;

void main()
{   
    gl_FragColor = texture(skybox, position0);
}