#version 430

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;
layout(location = 5) in vec3 vTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 eyePos, eyeNormal;
out vec2 pTexCoord;
out mat3 tbnMatrix;

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
}


