#pragma once
#include <glm/glm.hpp>
#include <vector>
#include "Particle.h"
#include "Constraint.h"
#include "ObjectBuffer.h"
#include "Triangle.h"

#define CONSTRAINT_ITERATIONS 3

struct Simplex
{
	glm::vec3 minkowskiDifference;
	glm::vec3 pointA;
	glm::vec3 pointB;
};

struct Face
{
	Face(int index1, int index2, int index3, std::vector<Simplex> &simplex) : i1(index1), i2(index2), i3(index3)
	{
		v1 = simplex[i1];
		v2 = simplex[i2];
		v3 = simplex[i3];

		normal = glm::normalize(glm::cross(v3.minkowskiDifference - v1.minkowskiDifference, v2.minkowskiDifference - v1.minkowskiDifference));
		if(glm::dot(v1.minkowskiDifference, normal) < 0)
		{
			normal = -normal;
		}
	}

	Simplex v1, v2, v3;
	int i1, i2, i3;

	glm::vec3 normal;
};

class Cloth
{
public:

	bool pined;
	int numNode,numTri;
	int num_particles_width; 
	int num_particles_height; 
	std::vector<Particle> particles; 
	std::vector<Particle*> tearingParticles; 
	std::vector<Constraint> constraints; 
	std::vector<Triangle> triangles; 

	ObjectBuffer* clothBuffer;
	std::vector<glm::vec3> v;
	std::vector<glm::vec3> n;
	std::vector<glm::vec4> c;

	glm::vec3 initPos;
	glm::vec3 collisionNormal;

	Cloth(float width, float height, int num_particles_width, int num_particles_height, glm::vec3 initPos, bool pined)
	{
		clothBuffer = new ObjectBuffer();
		this->num_particles_width = num_particles_width;
		this->num_particles_height = num_particles_height;
		particles.resize(num_particles_width*num_particles_height); 
		tearingParticles.resize(num_particles_width*num_particles_height); 


		for(int y=0; y<num_particles_height; y++)
		{
			for(int x=0; x<num_particles_width; x++)
			{
				glm::vec3 pos = glm::vec3(width * (x/(float)num_particles_width) + initPos.x, -height * (y/(float)num_particles_height) + initPos.y ,0 + initPos.z);
				particles[y*num_particles_width+x] = Particle(pos);
				//tearingParticles[y*num_particles_width+x] = Particle(pos);
				//particles[y*num_particles_width+x].id = y+x;
				//particles[y*num_particles_width+x].setMass(1/(num_particles_width * num_particles_height));
			}
		}

		for (int i=0; i<particles.size(); i++)
		{
			particles[i].id = i;
		}


// 		float mass = 1.f/( num_particles_width * num_particles_height);
// 
// 
// 		//
// 		for(int y=0; y<num_particles_height; y++)
// 		{
// 			for(int x=0; x<num_particles_width; x++)
// 			{
// 				particles[y*num_particles_width+x].setMass(1/mass);
// 			}
// 		}

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

// 		for(int i=0;i<3; i++)
// 		{
// 			getParticle(0+i, 0)->offsetPos(glm::vec3(0.5,0.0,0.0)); 
// 			getParticle(0+i, 0)->makeUnmovable(); 
// 
// 			getParticle(num_particles_width-i-1, 0)->offsetPos(glm::vec3(-0.5,0.0,0.0)); 
// 			getParticle(num_particles_width-i-1, 0)->makeUnmovable();
// 		}
		if (pined)
		{
			//getParticle(0, 0)->offsetPos(glm::vec3(0.5,0.0,0.0)); 
			getParticle(0, 0)->makeUnmovable();
			getParticle(num_particles_width-1, num_particles_height-1)->makeUnmovable();

			//getParticle(num_particles_width-2, 0)->offsetPos(glm::vec3(0.5,0.0,0.0)); 
			getParticle(num_particles_width-1, 0)->makeUnmovable();
			getParticle(0, num_particles_height-1)->makeUnmovable();

			getParticle(num_particles_width-1, num_particles_height-1)->pos = glm::vec3(initPos.y, initPos.y, -initPos.y);
			getParticle(0, num_particles_height-1)->pos = glm::vec3(0, initPos.y, -initPos.y);
		}
		else
		{
			getParticle(0, 0)->makeUnmovable();
			getParticle(num_particles_width-1, 0)->makeUnmovable();
		}


// 		v.clear();
// 		n.clear();
// 		c.clear();

		for(int x = 0; x<num_particles_width-1; x++)
		{
			for(int y=0; y<num_particles_height-1; y++)
			{
				glm::vec3 normal = calcTriangleNormal(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);

				triangles.push_back(Triangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1)));

				normal = calcTriangleNormal(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));
				getParticle(x+1,y+1)->addToNormal(normal);
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);

				triangles.push_back(Triangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1)));
			}
		}

// 		for(int x = 0; x<num_particles_width-2; x++)
// 		{
// 			for(int y=0; y<num_particles_height-2; y++)
// 			{
// 				v.push_back(getParticle(x+1,y)->getPos());
// 				v.push_back(getParticle(x,y)->getPos());
// 				v.push_back(getParticle(x,y+1)->getPos());
// 				v.push_back(getParticle(x+1,y+1)->getPos());
// 				v.push_back(getParticle(x+1,y)->getPos());
// 				v.push_back(getParticle(x,y+1)->getPos());
// 			}
// 		}
// 
// 		for (int i=0; i<v.size(); i++)
// 		{
// 			c.push_back(glm::vec4(1,0,0,0));
// 		}
// 
// 		clothBuffer->GenerateVBO(v,c,n);
// 		clothBuffer->LinkBufferToShaderWithNormal();
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


		//v.clear();
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).timeStep(); 
			//v.push_back((*particle).pos);
		}

		std::vector<Constraint>::iterator constraint;
		for(int i=0; i<CONSTRAINT_ITERATIONS; i++) 
		{
			for(constraint = constraints.begin(); constraint != constraints.end(); constraint++ )
			{
				if ((*constraint).drawable)
				{
					(*constraint).satisfyConstraint(); 
				}
			}
		}
	}

	void removePins()
	{
// 		for(int i=0;i<3; i++)
// 		{
// 			getParticle(0+i, 0)->makeMovable(); 
// 			getParticle(num_particles_width-i-2, 0)->makeMovable();
// 		}

		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).makeMovable(); 
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
// 		for(int x = 0; x<num_particles_width-1; x++)
// 		{
// 			for(int y=0; y<num_particles_height-1; y++)
// 			{
// 				addWindForcesForTriangle(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1),direction);
// 				addWindForcesForTriangle(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1),direction);
// 			}
// 		}

		std::vector<Triangle>::iterator triangle;


		for (triangle = triangles.begin(); triangle != triangles.end(); triangle++)
		{
			if ((*triangle).drawable)
			{
				addWindForcesForTriangle((*triangle).p1,(*triangle).p2,(*triangle).p3,direction);
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
				(*particle).isContacted = true;
				//(*particle).offsetPos((*particle).oldPosition); 
			}
		}
	}

	int conter;

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
				//particles.erase(particles.begin()+(*particle).id);
				//std::vector<Particle>::iterator constraint;
				for (int i=0; i<constraints.size(); i++)
				{
					if (constraints[i].p1->id == (*particle).id || constraints[i].p2->id == (*particle).id )
					{
						//constraints.erase(constraints.begin()+i);
						constraints[i].drawable = false;
					}
				}

// 				for (constraint = constraints.begin(); constraint != constraints.end(); constraint++)
// 				{
// 					if (constraint->p1->id == (*particle).id || constraint->p2->id == (*particle).id )
// 					{
// 						constraints.erase(constraint);
// 					}
// 				}

				for (int i=0; i<triangles.size(); i++)
				{
					if (triangles[i].p1->id == (*particle).id || triangles[i].p2->id == (*particle).id || triangles[i].p3->id == (*particle).id)
					{
						triangles[i].drawable = false;
					}
				}
			}
		}

// 		for (int i=0; i<particles.size(); i++)
// 		{
// 			if (particles[i].pos.y < -13)
// 			{
// 				particles.erase(particles.begin()+i);
// 			}
// 		}
	}

	void planeCollision(glm::vec3 planePos)
	{
		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			if ((*particle).pos.y < planePos.y)
			{
				(*particle).pos.y = planePos.y;
				//(*particle).makeUnmovable();
			}
		}
	}

	void reflectDirection()
	{
		std::vector<Particle>::iterator particle;
		for (particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).posToOldDirection = (*particle).oldPosition - (*particle).pos;
		}
	}

	void selfCollision()
	{
 		std::vector<Particle>::iterator particle;
 		std::vector<Triangle>::iterator triangle;

		for (particle = particles.begin(); particle != particles.end(); particle++)
		{
			for (triangle = triangles.begin(); triangle != triangles.end(); triangle++)
			{
				if ((*particle).id != (*triangle).p1->id && (*particle).id  != (*triangle).p2->id && (*particle).id  != (*triangle).p3->id && (*triangle).drawable == true)
				{
					if (PointInTriangle((*triangle).p1->pos, (*triangle).p2->pos, (*triangle).p3->pos, (*particle).pos))
					{

						(*particle).pos = (*particle).oldPosition;
						(*triangle).p1->pos = (*triangle).p1->oldPosition;
						(*triangle).p2->pos = (*triangle).p2->oldPosition;
						(*triangle).p3->pos = (*triangle).p3->oldPosition;
						if (PointInTriangle((*triangle).p1->pos, (*triangle).p2->pos, (*triangle).p3->pos, (*particle).pos))
						{
							(*particle).pos += (*particle).posToOldDirection * 0.5f;
							(*triangle).p1->pos += (*triangle).p1->posToOldDirection * 0.5f;
							(*triangle).p2->pos += (*triangle).p2->posToOldDirection * 0.5f;
							(*triangle).p3->pos += (*triangle).p3->posToOldDirection * 0.5f;
						}

						//printf("ye");
					}
					//testTriangleIntersect((*&particle));
				}
			}
		}
	}

	void clothWithClothCollision(std::vector<Particle> particle)
	{
		//std::vector<Particle>::iterator particle;
		std::vector<Triangle>::iterator triangle;

		for (int i=0; i<particle.size(); i++)
		{
			for (triangle = triangles.begin(); triangle != triangles.end(); triangle++)
			{
				if (PointInTriangle((*triangle).p1->pos, (*triangle).p2->pos, (*triangle).p3->pos, particle[i].pos))
				{

					particle[i].pos = particle[i].oldPosition;
					(*triangle).p1->pos = (*triangle).p1->oldPosition;
					(*triangle).p2->pos = (*triangle).p2->oldPosition;
					(*triangle).p3->pos = (*triangle).p3->oldPosition;
					if (PointInTriangle((*triangle).p1->pos, (*triangle).p2->pos, (*triangle).p3->pos, particle[i].pos))
					{
						particle[i].pos += particle[i].posToOldDirection * 0.5f;
						(*triangle).p1->pos += (*triangle).p1->posToOldDirection * 0.5f;
						(*triangle).p2->pos += (*triangle).p2->posToOldDirection * 0.5f;
						(*triangle).p3->pos += (*triangle).p3->posToOldDirection * 0.5f;
					}
				}
			}
		}
	}

// 	bool testTriangleIntersect(Particle* currentParticle)
// 	{
// 		bool testResult = false;
// 
// 		std::vector<Triangle>::iterator triangle;
// 		for (triangle = triangles.begin(); triangle != triangles.end(); triangle++)
// 		{
// 			if (triangle->p1->id != currentParticle->id && triangle->p1->id != currentParticle->id && triangle->p3->id != currentParticle->id)
// 			{
// 				//Triangle currentface = faces[i];
// 				glm::vec3 n = glm::normalize(calcTriangleNormal(triangle->p1,triangle->p2,triangle->p3));
// 
// 				glm::vec3 p = currentParticle->pos;
// 				glm::vec3 rayDirection = p - currentParticle->oldPosition;
// 				float nDotD = glm::dot(rayDirection, n);
// 				if (abs(nDotD) <= 0.0001)
// 				{
// 					return false;
// 				}
// 				else
// 				{
// 					float d = glm::dot(n, triangle->p1->getPos());
// 					//find out the intersect point
// 					float t = (d-glm::dot(n,currentParticle->oldPosition))/(nDotD);
// 					//float t = dot(n,(faces[i].particleA->getPos()-currentParticle->old_pos))/(nDotD);
// 					glm::vec3 intersectQ = p + t*rayDirection;
// 					if (((intersectQ.x)>glm::max(p.x,currentParticle->oldPosition.x)) || ((intersectQ.y)>glm::max(p.y,currentParticle->oldPosition.y)) || ((intersectQ.z)>glm::max(p.z,currentParticle->oldPosition.z)) ||
// 						((intersectQ.x)<glm::min(p.x,currentParticle->oldPosition.x)) || ((intersectQ.y)<glm::min(p.y,currentParticle->oldPosition.y)) || ((intersectQ.z)<glm::min(p.z,currentParticle->oldPosition.z))
// 						)
// 					{
// 						return false;
// 					}
// 
// 					if(PointInTriangle(triangle->p1->getPos(),triangle->p2->getPos(),triangle->p3->getPos(),intersectQ))
// 					{
// 						currentParticle->pos = currentParticle->oldPosition; 
// 
// 						printf("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
// 						return true;
// 					}
// 				}	
// 			}
// 		}
// 		return false;
// 	}

	bool PointInTriangle(glm::vec3 A, glm::vec3 B, glm::vec3 C, glm::vec3 P)
	{
		// Prepare our barycentric variables
		glm::vec3 u = B - A;
		glm::vec3 v = C - A;
		glm::vec3 w = P - A;

		glm::vec3 vCrossW = glm::cross(v, w);
		glm::vec3 vCrossU = glm::cross(v, u);

		// Test sign of r
		if (glm::dot(vCrossW, vCrossU) < 0)
			return false;

		glm::vec3 uCrossW = glm::cross(u, w);
		glm::vec3 uCrossV = glm::cross(u, v);

		// Test sign of t
		if (glm::dot(uCrossW, uCrossV) < 0)
			return false;

		// At this point, we know that r and t and both > 0.
		// Therefore, as long as their sum is <= 1, each must be less <= 1
		float denom = glm::length(uCrossV);
		float r = glm::length(vCrossW) / denom;
		float t = glm::length(uCrossW) / denom;


		return (r + t <= 1);
	}

	//GJK 
	bool CheckCollisionNarrow(Triangle &body1, Triangle &body2)
	{
		//demoResult.clear();
		std::vector<Simplex> simplex;

		glm::vec3 direction = body1.p1->pos - body2.p1->pos;

		simplex.push_back(support(direction, body1, body2));

		direction = -simplex[0].minkowskiDifference;
		int counter = 100;

		while (counter > 0)
		{
			Simplex tempSimplex;
			tempSimplex = support(direction, body1, body2);

			// Last point added was not past the origin in this direction
			if(glm::dot(tempSimplex.minkowskiDifference, direction) < 0)
			{
				return false;
			}
			simplex.push_back(tempSimplex);

			//check intersect
			if (processSimplex(simplex, direction))
			{
				if (simplex.size() == 3)
				{
					collisionNormal = EPA(simplex, body1, body2);
					return true;
				}
				return true;
			}
			counter--;
		}
	}

	Simplex support(glm::vec3 direction, Triangle &body1, Triangle &body2)
	{
		Simplex s;
		s.pointA = getFarthestPointInDirection(direction, body1.v);
		s.pointB = getFarthestPointInDirection(-direction, body2.v);
		s.minkowskiDifference = s.pointA - s.pointB;

		return s;
	}

	glm::vec3 getFarthestPointInDirection(glm::vec3 direction, const std::vector<glm::vec3>& vertices)
	{
		float maxDot = glm::dot(vertices[0],direction);

		int indexDot = 0;
		float currentDot;
		for (int i = 1; i < vertices.size(); i++)
		{
			currentDot = glm::dot(direction,vertices[i]);
			if (currentDot > maxDot){
				maxDot = currentDot;
				indexDot = i;
			}
		}

		return vertices[indexDot];
	}

	bool processSimplex(std::vector<Simplex> &simplex, glm::vec3 &direction)
	{
		Simplex A,B,C,D;
		glm::vec3 AB,AC,AD,AO;

		switch(simplex.size())
		{
		case 2:

			A = simplex.at(1);
			B = simplex.at(0);

			AB = B.minkowskiDifference - A.minkowskiDifference;
			AO = -A.minkowskiDifference;


			if(isSameDirection(AO,AB))
			{
				direction = glm::cross(glm::cross(AB, AO), AB);
			}
			else
			{
				direction = AO;
			}

			return false;
		case 3:

			//simplex.erase(simplex.begin());

			return checkTriangle(simplex, direction);

			//return false;
// 		case 4:
// 			A = simplex.at(3);
// 			B = simplex.at(2);
// 			C = simplex.at(1);
// 			D = simplex.at(0);
// 
// 			AB = B.minkowskiDifference - A.minkowskiDifference;
// 			AC = C.minkowskiDifference - A.minkowskiDifference;
// 			AD = D.minkowskiDifference - A.minkowskiDifference;
// 			AO = -A.minkowskiDifference;
// 
// 			glm::vec3 ABC = glm::cross(AB, AC);
// 			glm::vec3 ADB = glm::cross(AD, AB);
// 			glm::vec3 ACD = glm::cross(AC, AD);
// 
// 			if(isSameDirection(ABC, AO))
// 			{			
// 				simplex.erase(simplex.begin());
// 
// 				return checkTriangle(simplex, direction);
// 			}
// 			else if(isSameDirection(ADB, AO))
// 			{
// 				simplex.erase(simplex.begin() + 1);
// 				simplex[0] = B;
// 				simplex[1] = D;
// 
// 				return checkTriangle(simplex, direction);
// 			}
// 			else if(isSameDirection(ACD, AO))
// 			{
// 				simplex.erase(simplex.begin() + 2);
// 
// 				return checkTriangle(simplex, direction);
// 			}
// 
// 			return true;
 		}
	}

	glm::vec3 EPA(std::vector<Simplex>& simplex, Triangle &body1, Triangle &body2)
	{
		std::vector<Face> faces;

		faces.push_back(Face(0, 1, 2, simplex));

		int counter = 100;
		while (counter > 0)
		{
			//Face face = faces.at[0];
			Simplex newPoint = support(faces[0].normal, body1, body2);

			if(glm::dot(newPoint.minkowskiDifference - faces[0].v1.minkowskiDifference, faces[0].normal) - glm::dot(faces[0].v1.minkowskiDifference, faces[0].normal) < 0.01f) 
			{
				glm::vec3 result = -faces[0].normal * glm::dot(-faces[0].v1.minkowskiDifference, -faces[0].normal);

				if(result != glm::vec3())
				{
					glm::vec3 collidingNormal = glm::normalize(result);
					return collidingNormal;
				}
				else
				{
					return glm::vec3(glm::vec3::_null);
				}
			}
			counter--;
		}

		return glm::vec3(glm::vec3::_null);
	}

	bool isSameDirection(glm::vec3 &a, glm::vec3 &b)
	{
		float dot = glm::dot(a, b);
		return dot > 0.0f;
	}

	bool checkTriangle(std::vector<Simplex> &simplex, glm::vec3 &direction)
	{
		Simplex A,B,C;
		glm::vec3 AB,AC,AO;

		A = simplex[2];
		B = simplex[1];
		C = simplex[0];

		AB = B.minkowskiDifference - A.minkowskiDifference;
		AC = C.minkowskiDifference - A.minkowskiDifference;
		AO = -A.minkowskiDifference;

		glm::vec3 ABC = glm::cross(AB, AC);

		if(isSameDirection(glm::cross(ABC, AC), AO)) // AC plane 
		{
			if(isSameDirection(AC, AO)) // outside AC edge
			{
				direction = glm::cross(glm::cross(AC, AO), AC);
				simplex.erase(simplex.begin() + 1);
			}
			else
			{
				if(isSameDirection(AB, AO)) // outside AB edge
				{
					direction = glm::cross(glm::cross(AB, AO), AB);
					simplex.erase(simplex.begin());
				}
				else // outside A
				{
					direction = AO;
					simplex.erase(simplex.begin());
					simplex.erase(simplex.begin());
				}
			}
		}
		else // inside AC 
		{
			if(isSameDirection(glm::cross(AB, ABC), AO)) // AB plane 
			{
				if(isSameDirection(AB, AO)) // outside AB plane
				{
					direction = glm::cross(glm::cross(AB, AO), AB);
					simplex.erase(simplex.begin());
				}
				else // outside A
				{
					direction = AO;
					simplex.erase(simplex.begin());
					simplex.erase(simplex.begin());
				}
			}
			else // orthogonal to face
			{
				if(isSameDirection(ABC, AO)) // outside face
				{
					direction = ABC;
				}
				else // inside face
				{
					simplex[0] = B;
					simplex[1] = C;

					direction = -ABC;
				}
			}
		}
		return false;
	}

	/*
	(x,y)   *--* (x+1,y)
	        | /|
	        |/ |
	(x,y+1) *--* (x+1,y+1)
	*/

	void Cloth::drawShaded()
	{
		numTri = 0;

		std::vector<Particle>::iterator particle;
		for(particle = particles.begin(); particle != particles.end(); particle++)
		{
			(*particle).resetNormal();
		}

		tearingParticles.clear();
// 		v.clear();
// 		n.clear();
		//c.clear();

		for(int x = 0; x<num_particles_width-1; x++)
		{
			for(int y=0; y<num_particles_height-1; y++)
			{
				glm::vec3 normal = calcTriangleNormal(getParticle(x+1,y),getParticle(x,y),getParticle(x,y+1));
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);
				tearingParticles.push_back(getParticle(x+1,y));
				tearingParticles.push_back(getParticle(x,y));
				tearingParticles.push_back(getParticle(x,y+1));

				normal = calcTriangleNormal(getParticle(x+1,y+1),getParticle(x+1,y),getParticle(x,y+1));
				getParticle(x+1,y+1)->addToNormal(normal);
				getParticle(x+1,y)->addToNormal(normal);
				getParticle(x,y+1)->addToNormal(normal);
				tearingParticles.push_back(getParticle(x+1,y+1));
				tearingParticles.push_back(getParticle(x+1,y));
				tearingParticles.push_back(getParticle(x,y+1));
			}
		}

		for (int i=0; i<triangles.size(); i++)
		{
// 			triangles[i].setP1(tearingParticles[i*3]);
// 			triangles[i].setP2(tearingParticles[i*3+1]);
// 			triangles[i].setP3(tearingParticles[i*3+2]);

			triangles[i].update(tearingParticles[i*3],tearingParticles[i*3+1],tearingParticles[i*3+2]);
		}

// 		clothBuffer->Update(v,c,n);
// 		clothBuffer->LinkBufferToShaderWithNormal();

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