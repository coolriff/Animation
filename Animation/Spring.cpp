#include "spring.h"  

Spring::Spring()  
{  
	//ctor  
}  

Spring::~Spring()  
{  
	//dtor  
}  

Spring::Spring(RigidBody* mass1, RigidBody* mass2,  float springConstant, float springLength,  float frictionConstant)  
{  
	this->springConstant = springConstant; 
	this->restLength = springLength;   
	this->frictionConstant = frictionConstant;   

	this->mass1 = mass1;  
	this->mass2 = mass2;  
}  

void Spring::solve()  
{  
	glm::vec3 springVector = mass1->GetPosition() - mass2->GetPosition();  
	float r = glm::length(springVector);  

	glm::vec3 force(0);  

	if(0 != r)  
	{  
		force += (springVector / r) * (r - restLength) * (-springConstant);  
	}  

	force += -(mass1->GetAngularMomentum() - mass2->GetAngularVelocity())*frictionConstant;  

	mass1->SetAngularMomentum(force); 
	mass2->SetAngularMomentum(-force);  
} 