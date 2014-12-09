#version 330

in vec2 TexCoord0;
in vec3 Normal0;      
in vec3 WorldPos0;                                                                                                                           

uniform sampler2D gColorMap;                                                                                                                                                                                              
 
out vec4 FragColor;

void main()
{                                 
    FragColor = vec4(texture(gColorMap, TexCoord0));    
}
