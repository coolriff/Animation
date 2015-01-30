#include "CreateMesh.h"

#define CUBE 36
#define PI 3.1415926

void CreateMesh::createCubeMesh()
{
	glm::vec3 v[] = {
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3( 0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f,-0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f,-0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f, 0.5f, 0.5f),
		glm::vec3(-0.5f, 0.5f, 0.5f),
		glm::vec3(0.5f,-0.5f, 0.5f)
	};

	for (int i=0; i<CUBE; i++ )
	{
		vertices.push_back(v[i]);
	}

	glm::vec4 c[] = {
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f),
		glm::vec4(0.85f,  0.85f,  0.85f, 1.0f)
	};

	for (int i=0; i<CUBE; i++ )
	{
		colors.push_back(c[i]);
	}

	for(unsigned int i=0; i<CUBE; i+=3)
	{
		glm::vec3 v1 = v[i+1] - v[i];
		glm::vec3 v2 = v[i+2] - v[i];

		glm::vec3 n = glm::normalize(glm::cross(v1, v2));
		normals.push_back(n);
		normals.push_back(n);
		normals.push_back(n);
	}

}

void CreateMesh::createSphereMesh(int numSegments)
{
	std::vector< glm::vec3 > v;
	std::vector< glm::vec3 > n;

	std::vector<glm::vec3> points;
	for (int j= 0; j < numSegments; ++j)
	{
		float theta = (glm::pi<float>()*j)/(numSegments);

		for( int i=0; i<numSegments; ++i)
		{
			float phi = (2*glm::pi<float>()*i)/(numSegments);
			points.push_back(glm::vec3(glm::sin(theta)*glm::cos(phi), glm::sin(theta)*glm::sin(phi), glm::cos(theta)) / 2.0f);
		}
	}

	for (int j= 0; j < numSegments; j ++)
	{ 

		for( int i=0; i<numSegments; i++ )
		{
			int iNext = i+1;
			if (i == numSegments - 1)
				iNext = 0;

			int index = (j*numSegments*6)+(i*6);

			v.push_back(points[j*numSegments+i]);
			v.push_back(points[j*numSegments+iNext]);

			if (j != numSegments -1)
				v.push_back(points[((j+1)*numSegments)+i]);
			else
				v.push_back(glm::vec3( 0, 0, -0.5f));

			n.push_back(v[index]);
			n.push_back(v[index+1]);
			n.push_back(v[index+2]);

			v.push_back(v[index+2]);
			v.push_back(v[index+1]);

			if (j != numSegments - 1)
				v.push_back(points[((j+1)*numSegments)+iNext]);
			else
				v.push_back(glm::vec3( 0,0,-0.5f));

			n.push_back(v[index+3]);
			n.push_back(v[index+4]);
			n.push_back(v[index+5]);
		}
	}

	std::vector<glm::vec4> c;
	glm::vec4 color = glm::vec4(float(rand())/RAND_MAX * 0.5f + 0.5f, float(rand())/RAND_MAX * 0.5f + 0.5f, float(rand())/RAND_MAX * 0.5f + 0.5f, 1.0f);
	for(unsigned int i=0; i<v.size(); ++i)
	{
		c.push_back(color);
	}

	vertices = v;
	colors = c;
	normals = n;
}

void CreateMesh::createBoundingSphereMesh(float radius, int resolution)
{
	float X1,Y1,X2,Y2,Z1,Z2;
	float inc1,inc2,inc3,inc4,inc5,Radius1,Radius2;

	for(int w = 0; w < resolution; w++) 
	{
		for(int h = (-resolution/2); h < (resolution/2); h++)
		{

			inc1 = (w/(float)resolution)*2*PI;
			inc2 = ((w+1)/(float)resolution)*2*PI;

			inc3 = (h/(float)resolution)*PI;
			inc4 = ((h+1)/(float)resolution)*PI;

			X1 = glm::sin(inc1);
			Y1 = glm::cos(inc1);
			X2 = glm::sin(inc2);
			Y2 = glm::cos(inc2);

			// store the upper and lower radius, remember everything is going to be drawn as triangles
			Radius1 = radius*glm::cos(inc3);
			Radius2 = radius*glm::cos(inc4);

			Z1 = radius*glm::sin(inc3); 
			Z2 = radius*glm::sin(inc4);

			// insert the triangle coordinates
			vertices.push_back(glm::vec3(Radius1*X1,Z1,Radius1*Y1));
			vertices.push_back(glm::vec3(Radius1*X2,Z1,Radius1*Y2));
			vertices.push_back(glm::vec3(Radius2*X2,Z2,Radius2*Y2));

			vertices.push_back(glm::vec3(Radius1*X1,Z1,Radius1*Y1));
			vertices.push_back(glm::vec3(Radius2*X2,Z2,Radius2*Y2));
			vertices.push_back(glm::vec3(Radius2*X1,Z2,Radius2*Y1));

			//indexVBO(v, t, n, indices, indexed_vertices, indexed_uvs, indexed_normals);	 
		}
	}

	for (int i=0; i<vertices.size(); i++ )
	{
		colors.push_back(glm::vec4(0.85f,  0.85f,  0.85f, 1.0f));
	}

	for(unsigned int i=0; i<vertices.size(); i+=3)
	{
		glm::vec3 v1 = vertices[i+1] - vertices[i];
		glm::vec3 v2 = vertices[i+2] - vertices[i];

		glm::vec3 n = glm::normalize(glm::cross(v1, v2));
		normals.push_back(n);
		normals.push_back(n);
		normals.push_back(n);
	}
}
