#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;

uniform vec3 WorldCameraPosition;
uniform bool DrawSkyBox;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 worldNorm;
out vec3 worldView;

void main()
{
    vec3 worldPos = vec3(model * vec4(vPosition,1.0));
    worldNorm = vec3(model * vec4(vNormal, 0.0));
    worldView = normalize(WorldCameraPosition - worldPos);

    gl_Position = projection * view * model * vec4(vPosition,1.0);
}
