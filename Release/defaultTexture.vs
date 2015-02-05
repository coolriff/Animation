#version 330                                                             
                                                              
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoord0; 

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0);
	texCoord0 = vTexCoord;
}
