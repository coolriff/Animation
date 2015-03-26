#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp> 
#include <glm/gtx/transform.hpp>

#define DAMPING 0.01f
#define TIME_STEPSIZE2 0.5f*0.5f

class Particle
{
public:

	Particle(glm::vec3 pos) : pos(pos), oldPosition(pos),acceleration(glm::vec3(0,0,0)), mass(1), movable(true), accumulated_normal(glm::vec3(0,0,0)) {}
	Particle(){}

	glm::vec3 pos;
	glm::vec3 oldPosition;
	glm::vec3 acceleration;
	glm::vec3 accumulated_normal;

	float mass;
	float inverseMass;
	bool movable;

	void timeStep()
	{
		if(movable)
		{
			glm::vec3 temp = pos;
			pos = pos + (pos-oldPosition) * (1.0f - DAMPING) + acceleration * TIME_STEPSIZE2;
			oldPosition = temp;
			acceleration = glm::vec3(0,0,0); 
		}
	}

	glm::vec3 getNormal() {return accumulated_normal;}
	glm::vec3 getPos() {return pos;} 

	glm::mat4 getTransform() 
	{
		glm::mat4 tPos = glm::mat4(1);
		tPos[3][0] = pos.x;
		tPos[3][1] = pos.y;
		tPos[3][2] = pos.z;

		return tPos;
	}

	void setMass(float mass) { this->mass = mass;}
	void addForce(glm::vec3 f) {acceleration += f/mass;}
	void resetNormal() {accumulated_normal = glm::vec3(0);}
	void resetAcceleration() {acceleration = glm::vec3(0);}
	void offsetPos(const glm::vec3 v) { if(movable){ pos += v;}}
	void makeUnmovable() {movable = false;}
	void makeMovable() {movable = true;}
	void addToNormal(glm::vec3 normal) {accumulated_normal += glm::normalize(normal);}
};

