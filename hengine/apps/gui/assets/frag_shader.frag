#version 330 core

out vec4 colour;
in vec4 fragmentColour;
in vec2 UV;

uniform sampler2D sampler;

void main()
{
   vec4 texColour = texture(sampler, UV);
   colour = fragmentColour;
   colour = texColour * fragmentColour;
}