#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "particle.h"
#include "Constraint.h"
#include "ObjectBuffer.h"

#define CONSTRAINT_ITERATIONS 15

class Cloth
{
public:

	int numNode,numTri;
	int num_particles_width; 
	int num_particles_height; 
	std::vector<Particle> particles; 
	std::vector<Constraint> constraints; 

	ObjectBuffer* clothBuffer;
	std::vector<glm::vec3> v;
	std::vector<glm::vec3> n;
	std::vector<glm::vec4> c;

	Cloth(float width, float height, int num_particles_width, int num_particles_height)
	{
		clothBuffer = new ObjectBuffer();
		this->num_particles_width = num_particles_width;
		this->num_particles_height = num_particles_height;
		particles.resize(num_particles_width*num_particles_height); 


		for(int y=0; y<num_particles_height; y++)
		{
			for(int x=0; x<num_particles_width; x++)
			{
				glm::vec3 pos = glm::vec3(width * (x/(float)num_particles_width), -height * (y/(float)num_particles_height),0);
				particles[y*num_particles_width+x] = Particle(pos);
				particles[y*num_particles_width+x].id = y+x;
				//particles[y*num_particles_width+x].setMass(1/(num_particles_width * num_particles_height));
			}
		}


		float mass = 1.f/( num_particles_width * num_particles_height);


		//
		for(int y=0; y<num_particles_height; y++)
		{
			for(int x=0; x<num_particles_width; x++)
			{
				particles[y*num_particles_width+x].setMass(1/mass);
			}
		}

		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				if (x<num_particles_width-1) makeConstraint(getParticle(x,y),getParticle(x+1,y));
				if (y<num_particles_height-1) makeConstraint(getParticle(x,y),getParticle(x,y+1));
				if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(getParticle(x,y),getParticle(x+1,y+1));
				if (x<num_particles_width-1 && y<num_particles_height-1) makeConstraint(getParticle(x+1,y),getParticle(x,y+1));
			}
		}

		for(int x=0; x<num_particles_width; x++)
		{
			for(int y=0; y<num_particles_height; y++)
			{
				if (x<num_particles_width-2) makeConstraint(getParticle(x,y),getParticle(x+2,y));
				if (y<num_particles_height-2) makeConstraint(getParticle(x,y),getParticle(x,y+2));
				if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(getParticle(x,y),getParticle(x+2,y+2));
				if (x<num_particles_width-2 && y<num_particles_height-2) makeConstraint(getParticle(x+2,y),getParticle(x,y+2));			
			}
		}

		for(int i=0;i<3; i++)
		{
			getParticle(0+i, 0)->offsetPos(glm::vec3(0.5,0.0,0.0)); 
			getParticle(0+i, 0)->makeUnmovable(); 

			getParticle(num_particles_width-i-2, 0)->offsetPos(glm::vec3(-0.5,0.0,0.0)); 
			getParticle(num_particles_width-i-2, 0)->makeUnmovable();
		}

		v.clear();
		n.clear();
		c.clear();

		for(int x = 0; x<num_particles_width-2; x++)
		{
			for(int y=0; y<num_particles_height-2; y++)
			{
				glm::vec3 normal = calcTriangleNormal(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);
				n.push_back(getParticle(x+1,y)->getNormal());
				n.push_back(getParticle(x,y)->getNormal());
				n.push_back(getParticle(x,y+1)->getNormal());

				normal = calcTriangleNormal(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));
				getParticle(x+1,y+1)->addToNormal(normal);
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);
				n.push_back(getParticle(x+1,y+1)->getNormal());
				n.push_back(getParticle(x+1,y)->getNormal());
				n.push_back(getParticle(x,y+1)->getNormal());
			}
		}

		for(int x = 0; x<num_particles_width-2; x++)
		{
			for(int y=0; y<num_particles_height-2; y++)
			{
				v.push_back(getParticle(x+1,y)->getPos());
				v.push_back(getParticle(x,y)->getPos());
				v.push_back(getParticle(x,y+1)->getPos());
				v.push_back(getParticle(x+1,y+1)->getPos());
				v.push_back(getParticle(x+1,y)->getPos());
				v.push_back(getParticle(x,y+1)->getPos());
			}
		}

		for (int i=0; i<v.size(); i++)
		{
			c.push_back(glm::vec4(1,0,0,0));
		}

		clothBuffer->GenerateVBO(v,c,n);
		clothBuffer->LinkBufferToShaderWithNormal();
	}

	glm::vec3 calcTriangleNormal(Particle *p1,Particle *p2,Particle *p3)
	{
		glm::vec3 v1 = p2->getPos() - p1->getPos();
		glm::vec3 v2 = p3->getPos() - p1->getPos(); 

		return glm::cross(v1,v2);
	}
	
	Particle* getParticle(int x, int y) {return &particles[y*num_particles_width + x];}

	void makeConstraint(Particle *p1, Particle *p2) {constraints.push_back(Constraint(p1,p2));}

	void timeStep()
	{
		std::vector<Constraint>::iterator constraint;
		for(int i=0; i<CONSTRAINT_ITERATIONS; i++) 
		{
			for(constraint = constraints.begin(); constraint != constraints.end(); constraint++ )
			{
				(*constraint).satisfyConstraint(); 
			}
		}

		//v.clear();
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).timeStep(); 
			//v.push_back((*particle).pos);
		}
	}

	void removePins()
	{
		for(int i=0;i<3; i++)
		{
			getParticle(0+i, 0)->makeMovable(); 
			getParticle(num_particles_width-i-2, 0)->makeMovable();
		}
	}

	void addForce(glm::vec3 direction)
	{
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).addForce(direction); 
		}
	}

	void windForce(glm::vec3 direction)
	{
		for(int x = 0; x<num_particles_width-1; x++)
		{
			for(int y=0; y<num_particles_height-1; y++)
			{
				addWindForcesForTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),direction);
				addWindForcesForTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),direction);
			}
		}
	}

	void addWindForcesForTriangle(Particle *p1,Particle *p2,Particle *p3, glm::vec3 direction)
	{
		glm::vec3 normal = calcTriangleNormal(p1,p2,p3);
		glm::vec3 d = glm::normalize(normal);
		glm::vec3 force = normal*(glm::dot(d,direction));
		p1->addForce(force);
		p2->addForce(force);
		p3->addForce(force);
	}

	void ballCollision(glm::vec3 center, float radius )
	{
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			glm::vec3 v = (*particle).getPos()-center;
			float l = glm::length(v);
			if ( glm::length(v) < radius) 
			{
				(*particle).offsetPos(glm::normalize(v)*(radius-l)); 
			}
		}
	}

	void ballTearing(glm::vec3 center, float radius )
	{
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			glm::vec3 v = (*particle).getPos()-center;
			float l = glm::length(v);
			if ( glm::length(v) < radius) 
			{
				//(*particle).offsetPos(glm::normalize(v)*(radius-l)); 
				//particles.

				//std::vector<Particle>::iterator newEnd = std::remove(particles.begin(), particles.end(), particle->id);
				particles.erase(particles.begin()+particle->id);
				for (int i=0; i<constraints.size(); i++)
				{
					if (constraints[i].p1->id == particle->id || constraints[i].p2->id == particle->id )
					{
						constraints.erase(constraints.begin()+i);
					}
				}
			}
		}
	}

	void planeCollision(glm::vec3 planePos)
	{
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			if ((*particle).pos.y < planePos.y)
			{
				(*particle).pos.y = planePos.y;
			}
		}
	}

	void selfCollision()
	{
		//TODO
	}

	/*
	(x,y)   *--* (x+1,y)
	        | /|
	        |/ |
	(x,y+1) *--* (x+1,y+1)
	*/

	void Cloth::drawShaded()
	{
		numNode = 0;
		numTri = 0;

		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).resetNormal();
			numNode += 1;
		}

		v.clear();
		n.clear();
		//c.clear();

		for(int x = 0; x<num_particles_width-2; x++)
		{
			for(int y=0; y<num_particles_height-2; y++)
			{
				glm::vec3 normal = calcTriangleNormal(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);
				n.push_back(getParticle(x+1,y)->getNormal());
				n.push_back(getParticle(x,y)->getNormal());
				n.push_back(getParticle(x,y+1)->getNormal());

				normal = calcTriangleNormal(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));
				getParticle(x+1,y+1)->addToNormal(normal);
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);
				n.push_back(getParticle(x+1,y+1)->getNormal());
				n.push_back(getParticle(x+1,y)->getNormal());
				n.push_back(getParticle(x,y+1)->getNormal());

				v.push_back(getParticle(x+1,y)->getPos());
				v.push_back(getParticle(x,y)->getPos());
				v.push_back(getParticle(x,y+1)->getPos());
				v.push_back(getParticle(x+1,y+1)->getPos());
				v.push_back(getParticle(x+1,y)->getPos());
				v.push_back(getParticle(x,y+1)->getPos());

			}
		}

		clothBuffer->Update(v,c,n);
		clothBuffer->LinkBufferToShaderWithNormal();

// 		for(int x = 0; x<num_particles_width-1; x++)
// 		{
// 			for(int y=0; y<num_particles_height-1; y++)
// 			{
// 				glm::vec3 color(0,0,0);
// 				if (x%3)
// 				{
// 					color = glm::vec3(0.6f,0.4f,0.1f);
// 				}
// 				else
// 				{
// 					color = glm::vec3(0.9f,0.9f,0.9f);
// 				}
// 
// 				drawTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),color);
// 				numTri += 1;
// 				drawTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),color);
// 				numTri += 1;
// 			}
// 		}


	}

// 	void drawTriangle(Particle *p1, Particle *p2, Particle *p3, const Vec3 color)
// 	{
// 
// 	}
};