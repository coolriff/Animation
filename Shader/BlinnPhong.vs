#version 330                                                             
                                                              
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 4) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 position_eye;
out vec3 normal_eye;
out vec4 fColor;

void main()
{
	fColor = vColor;
    position_eye = vec3( view * model  * vec4(vPosition,1.0));
	normal_eye = vec3( view * model * vec4(vNormal,0.0));
	gl_Position = projection * vec4(position_eye, 1.0); 
}