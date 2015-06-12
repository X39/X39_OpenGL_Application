#version 440
precision highp float;


in vec2 UV;

uniform sampler2D textureSampler;
uniform mat4 scaleMatrix;
uniform float[6] UV_Manipulators;

layout( location = 0 ) out vec4 FragColor;

void main(void)
{
	vec3 fragColor3 = texture(textureSampler, UV).rgb;
	FragColor = vec4(fragColor3, 1);//fragColor3.r);
}