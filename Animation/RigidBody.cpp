#include "RigidBody.h"


RigidBody::RigidBody(glm::vec3 &p, glm::quat &o, glm::vec3 &s)
{
	position = p;
	orientation = o;
	scale = s;
	linearMomentum = glm::vec3();
	force = glm::vec3();
	angularMomentum = glm::vec3();
	torque = glm::vec3();
}

glm::mat4 RigidBody::GetTransformationMatrix()
{
	glm::mat4 T = glm::translate(position);
	glm::mat4 R = glm::toMat4(orientation);
	glm::mat4 S = glm::scale(scale);

	return T * R * S;
}

void RigidBody::Update(float deltaTime)
{
	// linear velocity
	linearMomentum += force * deltaTime;
	position += (linearMomentum * massInverse)  * deltaTime;

	force = glm::vec3();

	// angular velocity
	glm::mat3 currentInverseI = GetInertiaInverse();

	angularMomentum += torque * deltaTime;
	orientation = glm::normalize(glm::toQuat(currentInverseI * glm::toMat3(glm::quat(angularMomentum * deltaTime))) * orientation);

	torque = glm::vec3();
}

glm::mat3 RigidBody::GetInertiaInverse() const
{
	return glm::toMat3(orientation) * inverseI * glm::transpose(glm::toMat3(orientation));
}

void RigidBody::SetPoints(const std::vector<glm::vec3> & vertices)
{
	points.clear();
	for(int i=0; i<vertices.size(); ++i)
	{
		bool flag = false;
		for(int j=0; j<points.size(); ++j)
		{
			if(vertices[i] == points[j])
			{
				flag = true;
				break;
			}
		}

		if(!flag)
		{
			points.push_back(vertices[i]); 
		}
	}
}

void RigidBody::ApplyForce(const glm::vec3 &point, const glm::vec3 &f)
{
	force += f;
	torque += glm::cross(point - position, f);
}
