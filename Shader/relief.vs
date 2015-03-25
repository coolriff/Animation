#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;
layout(location = 5) in vec3 vTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 vLightDir;

out vec3 eyePos, eyeNormal;
out vec2 pTexCoord;
out mat3 tbnMatrix;



// tangent space vector to the light
out vec3 to_light;

//tangent space vector to the eye
out vec3 to_eye;

// fragment position in tangent space
out vec3 position_tan;

// eye space vector to fragment. used for depth correct.
out vec3 eye_to_pos;


void main()
{
    eyePos = vec3 (view * model * vec4 (vPosition, 1.0));
    eyeNormal = vec3 (view * model * vec4 (vNormal, 0.0));

    vec3 n = normalize((model * vec4(vNormal,0.0)).xyz);
    vec3 t = normalize((model * vec4(vTangent,0.0)).xyz);

    t = normalize(t - dot(t,n) * n);
    vec3 biTangent = cross(t, n);
    tbnMatrix = mat3(t,biTangent,n);

    pTexCoord = vTexCoord;
    gl_Position = projection * vec4(eyePos,1.0);
    //gl_Position = projection * view * model * vec4(vPosition, 1.0);

    to_light = (vLightDir - vec3(vPosition)) * tbnMatrix;
    to_eye = (eyePos - vec3(vPosition)) * tbnMatrix;

    position_tan = vec3(vPosition) * tbnMatrix;

    eye_to_pos = vec3 (view * model * vec4 (vPosition, 1.0));
}