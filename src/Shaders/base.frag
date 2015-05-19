#version 440
precision highp float;


in vec2 UV;

uniform sampler2D textureSampler;

layout( location = 0 ) out vec4 FragColor;

void main(void) {
	FragColor = vec4(texture(textureSampler, UV).rgb, 1.0);
}