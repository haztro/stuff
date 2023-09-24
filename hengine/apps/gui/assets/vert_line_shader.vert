#version 330 core

layout (location = 0) in vec2 vertexPosition;
layout (location = 1) in vec4 vertexColour;

out vec4 fragmentColour;

uniform mat4 P;

void main()
{
   gl_Position.xy = (P * vec4(vertexPosition, 0.0, 1.0)).xy;
   gl_Position.z = 0.0;
   gl_Position.w = 1.0;
   fragmentColour = vertexColour;
}