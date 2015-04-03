#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;
layout(location = 5) in vec3 vTangent;
layout(location = 6) in vec3 vBitangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 vLightDir;
uniform vec3 cameraPos;

out vec3 eyePos, eyeNormal;
out vec2 pTexCoord;
out mat3 tbnMatrix;

out vec3 LightDirection_tangentspace;
out vec3 EyeDirection_tangentspace;

// fragment position in tangent space
out vec3 position_tangentspace;
out vec3 vertexPosition_cameraspace;


void main()
{
	vertexPosition_cameraspace = vec3 (view * model * vec4(vPosition,1.0));

	mat3 MV3x3 = mat3(view * model);

	vec3 vertexTangent_cameraspace = MV3x3 * vTangent;
	vec3 vertexBitangent_cameraspace = MV3x3 * vBitangent;
	vec3 vertexNormal_cameraspace = MV3x3 * vNormal;

    tbnMatrix = transpose(mat3(vertexTangent_cameraspace,vertexBitangent_cameraspace,vertexNormal_cameraspace));

	vec3 vertexPosition_cameraspace = ( view * model * vec4(vPosition,1)).xyz;
	vec3 EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
	
	vec3 LightPosition_cameraspace = ( view * vec4(vLightDir,1)).xyz;
	vec3 LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;

    LightDirection_tangentspace = tbnMatrix * LightDirection_cameraspace;
    EyeDirection_tangentspace = tbnMatrix * EyeDirection_cameraspace;

    pTexCoord = vTexCoord;

    position_tangentspace = vec3(vPosition) * tbnMatrix;

	gl_Position = projection * view * model * vec4(vPosition,1.0);
}