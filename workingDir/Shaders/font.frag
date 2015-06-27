#version 440
precision highp float;


in vec2 UV;

uniform sampler2D textureSampler;
uniform mat4 scaleMatrix;
uniform float[6] UV_Manipulators;
uniform float[4] color_manipulator;

layout( location = 0 ) out vec4 FragColor;

void main(void)
{
	vec3 fragColor3 = vec3(texture(textureSampler, UV).rgb);
	FragColor = vec4(
		fragColor3.r * color_manipulator[0],
		fragColor3.g * color_manipulator[1],
		fragColor3.b * color_manipulator[2],
		fragColor3.r * color_manipulator[3]
	);
}