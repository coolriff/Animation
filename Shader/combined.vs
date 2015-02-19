#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;

out vec3 ReflectDir;

uniform float ratioR;
uniform float ratioG;
uniform float ratioB;

uniform vec3 WorldCameraPosition;
uniform bool DrawSkyBox;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 RefractDirR;
out vec3 RefractDirG;
out vec3 RefractDirB;

void main()
{

    vec3 worldPos = vec3( model * vec4(vPosition,1.0) );
    vec3 worldNorm = vec3( model * vec4(vNormal, 0.0));
    vec3 worldView = normalize( WorldCameraPosition - worldPos );

    ReflectDir = reflect(-worldView, worldNorm );
	ReflectDir = vec3 (inverse (view) * vec4 (ReflectDir, 0.0));

	vec3 RefractDirR = refract(-worldView, worldNorm, ratioR);
	RefractDirR = vec3 (inverse(view) * vec4(RefractDirR, 0.0));

	vec3 RefractDirG = refract(-worldView, worldNorm, ratioG);
	RefractDirG = vec3 (inverse(view) * vec4(RefractDirG, 0.0));

	vec3 RefractDirB = refract(-worldView, worldNorm, ratioB);
	RefractDirB = vec3 (inverse(view) * vec4(RefractDirB, 0.0));


    gl_Position = projection * view * model * vec4(vPosition,1.0);
}
