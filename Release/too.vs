#version 330                                                             
                                                              
in vec3 vPosition;
in vec4 vColor;
in vec3 vNormal;					

out vec3 fPosition;
out vec4 fColor;
out vec3 fNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
    fPosition = (view * model * vec4(vPosition, 1.0)).xyz;
	fNormal = (view * model * vec4(vNormal, 0.0)).xyz;
	gl_Position = projection * vec4(fPosition, 1.0);  

	fColor = vColor;
}
