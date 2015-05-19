#version 440
precision highp float;


in vec2 UV;

uniform sampler2D texture_sampler;
uniform mat4 scaleMatrix;
uniform vec2 textureMeasurements;

layout( location = 0 ) out vec4 FragColor;

void main(void) {
	FragColor = vec4(texture(texture_sampler, UV / textureMeasurements).rgba) * scaleMatrix;
}