#version 330 core

out vec4 colour;
in vec4 fragmentColour;

void main()
{
   colour = fragmentColour;
}