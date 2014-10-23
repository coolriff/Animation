#ifndef __CYLINDER_H__
#define __CYLINDER_H__

#include <glm/glm.hpp>
#include <GL/Glew.h>
#include <vector>
#include <glm/gtc/type_ptr.hpp>


// Macro for indexing vertex buffer
#define BUFFER_OFFSET(i) ((char *)NULL + (i))

class Cylinder {

public:
	Cylinder(){};

	Cylinder(const float height, const float radiusTop, const float radiusBottom, const glm::vec4& colorTop, const glm::vec4& colorBottom, const int segments = 16) 
	{
		generateVertices(height, radiusTop, radiusBottom, colorTop, colorBottom, segments);
	};

	// generate a cylinder with the bottom base center at (0,0,0), up on the Y axis
	void generateVertices(const float height, const float radiusTop, const float radiusBottom, const glm::vec4& colorTop, const glm::vec4& colorBottom, const int segments)
	{
		vertex_positions.empty();
		vertex_colors.empty();
		nbSegments = segments;

		double angle = 0.0;
		vertex_positions.push_back(glm::vec4(0, height, 0,1));
		vertex_colors.push_back(colorTop);
		for(unsigned int i = 0; i<nbSegments; ++i)
		{
			angle = ((double)i)/((double)nbSegments)*2.0*3.14;
			vertex_positions.push_back(glm::vec4(radiusTop*std::cos(angle),height,radiusTop*std::sin(angle),1.0));
			vertex_colors.push_back(colorTop);
			vertex_indexes.push_back(0);
			vertex_indexes.push_back((i+1)%nbSegments + 1);
			vertex_indexes.push_back(i+1);
		}
		
		vertex_positions.push_back(glm::vec4(0, 0, 0,1));
		vertex_colors.push_back(colorBottom);
		for(unsigned int i = 0; i<nbSegments; ++i)
		{
			angle = ((double)i)/((double)nbSegments)*2.0*3.14;
			vertex_positions.push_back(glm::vec4(radiusBottom*std::cos(angle),0.0,radiusBottom*std::sin(angle),1.0));
			vertex_colors.push_back(colorBottom);
			vertex_indexes.push_back(nbSegments+1);
			vertex_indexes.push_back(nbSegments+2+(i+1)%nbSegments);
			vertex_indexes.push_back(nbSegments+i+2);
		}
		
		for(unsigned int i = 0; i<nbSegments; ++i)
		{
			vertex_indexes.push_back(i+1);
			vertex_indexes.push_back((i+1)%nbSegments + 1);
			vertex_indexes.push_back(nbSegments+2+(i+1)%nbSegments);

			vertex_indexes.push_back(i+1);
			vertex_indexes.push_back(nbSegments+2+(i+1)%nbSegments);
			vertex_indexes.push_back(nbSegments+i+2);
		}

	}

	void generateObjectBuffer(GLuint shaderProgramID) 
	{
		//Initialize VAO
		glGenVertexArrays( 1, &vertex_array_object_id );
		glBindVertexArray( vertex_array_object_id );

		//Calc Array Sizes
		vertexArraySize = vertex_positions.size()*sizeof(glm::vec4);
		colorArraySize = vertex_colors.size()*sizeof(glm::vec4);

		//Initialize VBO
		glGenBuffers( 1, &buffer_object_id );
		glBindBuffer( GL_ARRAY_BUFFER, buffer_object_id );
		glBufferData( GL_ARRAY_BUFFER, vertexArraySize + colorArraySize, NULL, GL_STATIC_DRAW );
		glBufferSubData( GL_ARRAY_BUFFER, 0, vertexArraySize, (const GLvoid*)(&vertex_positions[0]) );
		glBufferSubData( GL_ARRAY_BUFFER, vertexArraySize, colorArraySize, (const GLvoid*)(&vertex_colors[0]));

// 		GLuint positionID = glGetAttribLocation(shaderProgramID, "vPosition");
// 		GLuint colorID = glGetAttribLocation(shaderProgramID, "vColor");

		//Set up Vertex Arrays  
		glEnableVertexAttribArray( 0 ); //SimpleShader attrib at position 0 = "vPosition"
		glVertexAttribPointer( (GLuint)0, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(0) );
		glEnableVertexAttribArray( 1 ); //SimpleShader attrib at position 1 = "vColor"
		glVertexAttribPointer( (GLuint)1, 4, GL_FLOAT, GL_FALSE, 0, BUFFER_OFFSET(vertexArraySize));

		//Unbind
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		//Initialize IBO
		glGenBuffers(1, &index_buffer_object_id);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_id);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertex_indexes.size()*sizeof(GLuint), (const GLvoid*)(&vertex_indexes[0]), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}

	//void update(GLuint modelLoc, glm::mat4 ModelMatrix, GLuint shaderProgramID)
	void update(glm::mat4 ModelMatrix, GLuint shaderProgramID)
	{
		GLuint modelLoc = glGetUniformLocation(shaderProgramID, "model");
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, &ModelMatrix[0][0]);
	}

	void draw(){
		glBindVertexArray(vertex_array_object_id);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_buffer_object_id);
		int size;  
		glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);
		size /= sizeof(GLuint);
		glDrawElements(GL_TRIANGLES, size, GL_UNSIGNED_INT, 0);

		glBindVertexArray(0);
	}

	int nbSegments;
	std::vector<glm::vec4> vertex_positions;
	std::vector<glm::vec4> vertex_colors;
	std::vector<GLuint>	 vertex_indexes;

	GLuint vertexArraySize;
	GLuint colorArraySize;

	GLuint buffer_object_id;
	GLuint vertex_array_object_id;
	GLuint index_buffer_object_id;
};

#endif // __CYLINDER_H__