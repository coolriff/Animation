#version 330

layout(location = 0) in vec3 vPosition;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;
layout(location = 5) in vec3 vTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 position_eye, normal_eye;
out vec2 texCoord0;
out vec3 worldPos0;
out mat3 tbnMatrix;

void main()
{
	position_eye = vec3 (view * model * vec4 (vPosition, 1.0));
	normal_eye = vec3 (view * model * vec4 (vNormal, 0.0));

    gl_Position = projection * vec4(position_eye,1.0);
    texCoord0 = vTexCoord;
    worldPos0 = (model * vec4(vPosition,1.0)).xyz; 

    vec3 n = normalize((model * vec4(vNormal,0.0)).xyz);
	vec3 t = normalize((model * vec4(vTangent,0.0)).xyz);

	t = normalize(t - dot(t,n) * n);

	vec3 biTangent = cross(t, n);
	tbnMatrix = mat3(t,biTangent,n);
}


