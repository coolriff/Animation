#version 330                                                             
                                                              
layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec4 vColor;
layout(location = 2) in vec2 vTexCoord;
layout(location = 4) in vec3 vNormal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Position;
out vec3 Normal;
out vec2 TexCoord;

uniform vec3 vDiffuseDirection;
out vec3 diffuseDirection;

void main()
{
	TexCoord = vTexCoord;

	Normal = vec3( (view * model) * vec4(vNormal,0.0));
    Position = vec3( (view * model)  * vec4(vPosition,1.0));

	diffuseDirection = (view * vec4(vDiffuseDirection, 0.0)).xyz; 

	gl_Position = projection * vec4(Position, 1.0); 

}