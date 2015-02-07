#version 330                                                             
                                                              
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;				

out vec3 fPosition;
out vec4 fColor;
out vec3 fNormal;

uniform vec3 vDiffuseDirection;
out vec3 diffuseDirection;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    fPosition = (view * model * vec4(vPosition, 1.0)).xyz;
	fNormal = (view * model * vec4(vNormal, 0.0)).xyz;
	gl_Position = projection * vec4(fPosition, 1.0);  

	diffuseDirection = (view * vec4(vDiffuseDirection, 0.0)).xyz;

	fColor = vColor;
}
