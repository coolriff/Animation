#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoord;
layout (location = 2) in vec3 normal;
layout (location = 5) in vec3 vTangent;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 position_eye, normal_eye;
out vec2 texCoord0;
out vec3 worldPos0;
out mat3 tbnMatrix;

void main()
{
	position_eye = vec3 (view * model * vec4 (position, 1.0));
	normal_eye = vec3 (view * model * vec4 (normal, 0.0));

    gl_Position = projection * vec4(position_eye,1.0);
    texCoord0 = texCoord;
    worldPos0 = (model * vec4(position,1.0)).xyz; 

    vec3 n = normalize((model * vec4(normal,0.0)).xyz);
	vec3 t = normalize((model * vec4(vTangent,0.0)).xyz);

	t = normalize(t - dot(t,n) * n);

	vec3 biTangent = cross(t, n);
	tbnMatrix = mat3(t,biTangent,n);
}


