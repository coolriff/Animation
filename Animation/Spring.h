#ifndef SPRING_H  
#define SPRING_H  
#include "RigidBody.h"  
#include <iostream> 

class Spring  
{  
public:  
	Spring();  
	Spring(RigidBody* mass1, RigidBody* mass2,  
		float springConstant, float springLength,  
		float frictionConstant);  
	~Spring();  

	void solve();  

	RigidBody* mass1;  
	RigidBody* mass2;  

	float springConstant;  
	float restLength;  
	float frictionConstant;  

};  

#endif // SPRING_H 