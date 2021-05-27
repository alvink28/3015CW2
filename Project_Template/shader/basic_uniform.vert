#version 460

//in variables, this are in model coordinates
layout (location = 0) in vec3 VertexPosition; 
layout (location = 1) in vec3 VertexNormal; 
layout (location = 2) in vec2 VertexTexCoord;

//out vector needed for the fragment shader
out vec3 LightIntensity; 
out vec3 Colour;
out vec2 TexCoord;

layout(binding=0) uniform sampler2D Tex1;
 
 //light information struct
uniform struct LightInfo 
{
  vec4 Position; // Light position in eye coords.
  vec3 Ld;       // Diffuse light intensity
  vec3 La;
  vec3 L;
} lights[3];

//material information struct
uniform struct MaterialInfo 
{
   vec3 Ka; // Ambient reflectivity
   vec3 Kd; // Diffuse reflectivity
   vec3 Ks; // Specular reflectivity
   float Shininess; // Specular shininess factor
} Material;

//uniforms for matrices required in the shader
uniform vec4 view;
uniform mat4 ModelViewMatrix;   //model view matrix
uniform mat3 NormalMatrix;		//normal matrix
uniform mat4 MVP;				//model view projection matrix
 
void getCamSpaceValues ( out vec3 norm, out vec3 position )
{
    norm = normalize( NormalMatrix * VertexNormal);
    position = (ModelViewMatrix * vec4(VertexPosition,1.0)).xyz;
}

vec3 phongModel( int light, vec3 position, vec3 n )
{
    vec3 texColor = texture(Tex1, TexCoord).rgb;

    vec3 ambient = texColor * Material.Ka * lights[light].La;

    vec3 s = normalize(vec3(lights[light].Position) - position);
    float sDotN = max( dot(s,n), 0.0 );
    vec3 diffuse = texColor * Material.Kd * sDotN;

    //calculate specular here
    vec3 spec = vec3(0.0);

    if( sDotN > 0.0 )
    {
        vec3 v = normalize(-position.xyz);
        vec3 r = reflect( -s, n ); 
        spec = Material.Ks * pow( max( dot(r,v), 0.0 ), Material.Shininess );
    }
    return ambient + lights[light].L * (diffuse + spec);
}

void main() 
{ 
   TexCoord = VertexTexCoord;

  //transfrom normal from model coordinates to view coordinates
  vec3 n = normalize( NormalMatrix * VertexNormal);

  //transform vertex position from model coordinates to view coordinates
  vec4 pos = ModelViewMatrix * vec4(VertexPosition,1.0);

  vec3 camNorm, camPosition;
  getCamSpaceValues (camNorm, camPosition);

  Colour = vec3(0.1);
  for( int i = 0; i < 3; i++ )
       Colour += phongModel( i, camPosition, camNorm );

  gl_Position = MVP * vec4(VertexPosition,1.0); 
} 