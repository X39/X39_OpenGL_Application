#version 440


layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV_modelspace;

uniform vec2 screenPosition;
uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 scaleMatrix;
uniform mat4 modelMatrix;
uniform vec3 worldPosition;

out vec2 UV;
uniform float[6] UV_Manipulators;


void main()
{
    gl_Position = vec4((vertexPosition_modelspace + worldPosition), 1) * viewMatrix * projectionMatrix * modelMatrix * scaleMatrix;
	vec2 newUV = vertexUV_modelspace;
	if(newUV.x > 0)
	{
		newUV.x = (UV_Manipulators[0] + UV_Manipulators[2]) / UV_Manipulators[4];
	}
	else
	{
		newUV.x = (UV_Manipulators[0]) / UV_Manipulators[4];
	}
	if(newUV.y > 0)
	{
		newUV.y = (UV_Manipulators[1] + UV_Manipulators[3]) / UV_Manipulators[5];
	}
	else
	{
		newUV.y = (UV_Manipulators[1]) / UV_Manipulators[5];
	}
	UV = newUV;
}