#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>
#include <vector>
#include "ObjectBuffer.h"
#include "Particle.h"

class Triangle
{
public:

	ObjectBuffer* triangleBuffer;
	std::vector<glm::vec3> v;
	std::vector<glm::vec3> n;
	std::vector<glm::vec4> c;
	Particle* p1;
	Particle* p2;
	Particle* p3;

	bool drawable;

	Triangle(Particle* particle1, Particle* particle2, Particle* particle3) 
	{
		triangleBuffer = new ObjectBuffer();
		this->p1 = particle1;
		this->p2 = particle2;
		this->p3 = particle3;
		drawable = true;

		for (int i=0; i<4; i++)
		{
			c.push_back(glm::vec4(1,0,0,0));
		}

		init();
	}

	void setP1(Particle *particle1) {this->p1 = particle1;}
	void setP2(Particle *particle2) {this->p2 = particle2;}
	void setP3(Particle *particle3) {this->p3 = particle3;}

	void init()
	{
		v.push_back(p1->pos);
		v.push_back(p2->pos);
		v.push_back(p3->pos);

		n.push_back(p1->accumulated_normal);
		n.push_back(p2->accumulated_normal);
		n.push_back(p3->accumulated_normal);

		triangleBuffer->GenerateVBO(v,c,n);
		triangleBuffer->LinkBufferToShaderWithNormal();
	}

	void update(Particle* particle1, Particle* particle2, Particle* particle3)
	{
		setP1(particle1);
		setP2(particle2);
		setP3(particle3);

		v.clear();
		n.clear();

		v.push_back(p1->pos);
		v.push_back(p2->pos);
		v.push_back(p3->pos);

		n.push_back(p1->accumulated_normal);
		n.push_back(p2->accumulated_normal);
		n.push_back(p3->accumulated_normal);

		triangleBuffer->Update(v,c,n);
		triangleBuffer->LinkBufferToShaderWithNormal();
	}
};

