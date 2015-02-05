#version 440
layout(location = 0) in vec3 vertexPosition_modelspace;
//layout(location = 0) uniform mat4 matrix;
layout(location = 0) uniform mat4 modelMatrix;
layout(location = 1) uniform mat4 viewMatrix;
layout(location = 2) uniform mat4 projectionMatrix;
void main()
{
	vec4 v = vec4(vertexPosition_modelspace,1);
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * v;
}