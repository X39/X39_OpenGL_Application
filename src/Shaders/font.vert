#version 440


layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV_modelspace;

uniform vec2 screenPosition;
uniform mat4 scaleMatrix;

out vec2 UV;


void main()
{
    gl_Position = vec4((vertexPosition_modelspace + vec3(screenPosition, 1)), 1) * scaleMatrix;
	UV = vertexUV_modelspace;
}