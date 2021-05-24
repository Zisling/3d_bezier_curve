#version 330

in vec2 texCoord0;
in vec3 normal0;
in vec3 color0;
in vec3 position0;

uniform sampler2D sampler1;
uniform vec4 camPos;

void main()
{
	vec4 texture = texture2D(sampler1, texCoord0) * vec4(color0, 1.0);
	vec3 L = normalize(vec3(0.0, -0.7, -1.5));
	vec4 I_d = vec4(1.0, 1, 1, 1.0);
	vec4 I_l = vec4(0.7, 0.7, 0.7, 1.0);
	vec4 k_d = texture ;
	vec4 I_a = vec4(0.1, 0.1, 0.1, 1.0);
	vec4 k_a = texture;
	vec4 R = normalize(vec4(reflect(L, normal0), 0));
	vec4 V = normalize(camPos - vec4(position0, 0));
	vec4 k_s = texture;
	vec4 I = k_a * I_a + k_d * dot(normal0,(-L)) * I_d + k_s * pow(max(0, dot(V, R)), 4) * I_l;
	gl_FragColor = I; //you must have gl_FragColor
}
