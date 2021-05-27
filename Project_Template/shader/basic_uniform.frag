#version 460

//in variable that receives the diffuse calculation from the vertex shader
in vec3 LightIntensity;
in vec3 Colour;

in vec2 TexCoord;

//out variable, this typical for all fragment shaders
layout (location = 0) out vec4 FragColor;
layout (location = 0) out vec3 VertexPosition; 
layout (location = 1) out vec3 VertexNormal; 


void main()
{
    FragColor = vec4(Colour, 1.0);
}
