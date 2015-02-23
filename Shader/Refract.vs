// Code based on GLSL cookbook.
#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;

out vec3 ReflectDir;
out vec3 RefractDir;

struct MaterialInfo {
    float Eta;              // Index of refraction
    float ReflectionFactor; // Percentage of reflected light
};
uniform MaterialInfo Material;

uniform vec3 WorldCameraPosition;
uniform bool DrawSkyBox;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    if( DrawSkyBox ) 
	{
        ReflectDir = vPosition;
    }
	else 
	{
        vec3 worldPos = vec3( model * vec4(vPosition,1.0) );
        vec3 worldNorm = vec3( model * vec4(vNormal, 0.0));
        vec3 worldView = normalize( WorldCameraPosition - worldPos );

        ReflectDir = reflect(-worldView, worldNorm );
        RefractDir = refract(-worldView, worldNorm, Material.Eta );
    }
    gl_Position = projection * view * model * vec4(vPosition,1.0);
}
