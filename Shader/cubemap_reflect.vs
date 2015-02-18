#version 430

layout (location = 0) in vec3 VertexPosition;
layout (location = 1) in vec3 VertexNormal;
layout (location = 2) in vec2 VertexTexCoord;

out vec3 ReflectDir;

uniform bool DrawSkyBox;
uniform vec3 WorldCameraPosition;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    if( DrawSkyBox ) {
        ReflectDir = VertexPosition;
    } else {
        vec3 worldPos = vec3( model * vec4(VertexPosition,1.0));
        vec3 worldNorm = vec3( model * vec4(VertexNormal, 0.0));
        vec3 worldView = normalize( WorldCameraPosition - worldPos );

        ReflectDir = reflect(-worldView, worldNorm );
    }

    gl_Position = projection * view * model * vec4(VertexPosition,1.0);
}
