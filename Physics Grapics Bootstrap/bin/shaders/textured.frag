// A simple shader for flat colors
#version 410

uniform sampler2D diffuseTexture;
in vec2 vTexCoord;

out vec4 FragColor;


void main()
{
    FragColor = texture(diffuseTexture, vTexCoord);
}