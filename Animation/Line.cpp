#include "Line.h"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

Line::Line()
{
}

void Line::GenerateBuffer(std::vector<glm::vec3> & vertices, std::vector<glm::vec4> & colors)
{
	//Initialize VAO
	glGenVertexArrays( 1, &vao );
	glBindVertexArray( vao );

	//Calc Array Sizes
	vSize = vertices.size() * sizeof(glm::vec3);
	cSize = colors.size() * sizeof(glm::vec4);

	//Initialize VBO
	glGenBuffers( 1, &vbo );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );
	glBufferData( GL_ARRAY_BUFFER, vSize + cSize, NULL, GL_STATIC_DRAW );
	glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]) );
	glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&colors[0]) );

	//Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Line::ChangeColor(glm::vec4 &color)
{	
	for(unsigned int i=0; i<colors.size(); ++i)
	{
		colors[i] = color;
	}

	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );

	glBufferSubData( GL_ARRAY_BUFFER, vSize, cSize, (const GLvoid*)(&colors[0]));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

}

void Line::SetFromTo(glm::vec3 from, glm::vec3 to)
{
	vertices.push_back(from);
	vertices.push_back(to);

	glBindVertexArray(vao);
	glBindBuffer( GL_ARRAY_BUFFER, vbo);

	glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, (const GLvoid*)(&vertices[0]));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}


void Line::linkShader(GLuint shaderID)
{
	// Bind buffer
	glBindVertexArray( vao );
	glBindBuffer( GL_ARRAY_BUFFER, vbo );

	//Set up Vertex Arrays  
	GLuint vPositionID = glGetAttribLocation(shaderID, "vPosition");
	GLuint vColorID = glGetAttribLocation(shaderID, "vColor");

	glEnableVertexAttribArray( vPositionID );
	glVertexAttribPointer( vPositionID, 3, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );

	glEnableVertexAttribArray( vColorID );
	glVertexAttribPointer( vColorID, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vSize));

	// Unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Line::Render(GLuint shaderID)
{
	glBindVertexArray (vao);
	glDrawArrays (GL_LINE_LOOP, 0, vertices.size());
	glBindVertexArray(0);
}