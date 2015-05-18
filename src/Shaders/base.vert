#version 440
layout(location = 0) in vec4 in_position;
layout(location = 1)uniform vec3 worldPosition;
layout(location = 2)uniform mat4 viewMatrix;
layout(location = 3)uniform mat4 projectionMatrix;
layout(location = 4)uniform mat4 modelMatrix;
out vec2 UV;


void main()
{
    gl_Position = (in_position + vec4(worldPosition, 1)) * viewMatrix * projectionMatrix * modelMatrix;
	UV = (in_position.xy + vec2(1, 1)) / 2.0;
}