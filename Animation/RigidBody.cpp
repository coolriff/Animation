#include "RigidBody.h"


RigidBody::RigidBody(glm::vec3 &p, glm::quat &o, glm::vec3 &s)
{
	m_position = p;
	m_orientation = o;
	m_scale = s;
	m_linearMomentum = glm::vec3();
	m_force = glm::vec3();
	m_angularMomentum = glm::vec3();
	m_torque = glm::vec3();
}

glm::mat4 RigidBody::GetTransformationMatrix()
{
	glm::mat4 T = glm::translate(m_position);
	glm::mat4 R = glm::toMat4(m_orientation);
	R= glm::inverse(R);
	glm::mat4 S = glm::scale(m_scale);

	return T * R * S;
}

void RigidBody::Update(float deltaTime)
{
	// linear velocity
	m_linearMomentum += m_force * deltaTime;
	m_position += (m_linearMomentum * m_massInverse)  * deltaTime;

	m_force = glm::vec3();

	// angular velocity
	glm::mat3 currentInverseI = GetInertiaInverse();

	m_angularMomentum += m_torque * deltaTime;
	m_orientation = glm::normalize(glm::toQuat(currentInverseI * glm::toMat3(glm::quat(m_angularMomentum * deltaTime))) * m_orientation);

	m_torque = glm::vec3();
}

glm::mat3 RigidBody::GetInertiaInverse() const
{
	return glm::toMat3(m_orientation) * m_inverseI * glm::transpose(glm::toMat3(m_orientation));
}

void RigidBody::SetPoints(const std::vector<glm::vec3> & vertices)
{
	m_points.clear();
	for(int i=0; i<vertices.size(); ++i)
	{
		bool flag = false;
		for(int j=0; j<m_points.size(); ++j)
		{
			if(vertices[i] == m_points[j])
			{
				flag = true;
				break;
			}
		}

		if(!flag)
		{
			m_points.push_back(vertices[i]); 
		}
	}
}

void RigidBody::ApplyForce(const glm::vec3 &point, const glm::vec3 &f)
{
	m_force += f;
	m_torque += glm::cross(point - m_position, f);
}
