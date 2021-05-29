#version 460
#define PI 3.14159265

//in variable that receives the diffuse calculation from the vertex shader
in vec3 LightIntensity;
in vec3 Colour;

in vec2 TexCoord;


//out variable, this typical for all fragment shaders
layout (location = 0) out vec4 FragColor;
layout (location = 0) out vec3 VertexPosition; 
layout (location = 1) out vec3 VertexNormal; 

uniform vec4 Color;
uniform sampler2D NoiseTex;
uniform vec4 SkyColor = vec4(0.3, 0.3, 0.9, 1.0);
uniform vec4 CloudColor = vec4(1.0, 1.0, 1.0, 1.0);


void main()
{
    vec4 noise = texture(NoiseTex, TexCoord);

    float t = (cos( noise.a * PI) + 1.0) / 2.0;
    vec4 color = mix(SkyColor, CloudColor, t);

    FragColor = vec4(Colour, 1.0);
}
