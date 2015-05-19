#version 440


layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

uniform vec3 worldPosition;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;

out vec2 UV;

void main()
{
    gl_Position = vec4((vertexPosition_modelspace + worldPosition), 1) * viewMatrix * projectionMatrix * modelMatrix;
	UV = vertexUV;
}