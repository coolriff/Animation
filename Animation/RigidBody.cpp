#include "RigidBody.h"
#define MAX 36

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


void RigidBody::updateCurrentVertices(int verticesSize, std::vector<glm::vec3> & vertices)
{
	m_currentVerticesPosition.clear();
	for(int i = 0; i < verticesSize; i++)
	{
		glm::vec3 t = vertices.at(i);

		m_currentVerticesPosition.push_back(t * m_orientation + m_position);
	}
	SetPoints(m_currentVerticesPosition);
}

void RigidBody::CalculateCentreOfMess(int verticesSize)
{
	//Centroid of a 3D shell described by 3 vertex facets
	//http://paulbourke.net/geometry/polygonmesh/

	float area[MAX];

	glm::vec3 r[MAX];
	glm::vec3 up;
	glm::vec3 down;

	for (int i=0; i<MAX; i+=3)
	{
		glm::vec3 v1 = m_currentVerticesPosition[i];
		glm::vec3 v2 = m_currentVerticesPosition[i+1];
		glm::vec3 v3 = m_currentVerticesPosition[i+2];

		r[i] = (v1 + v2 + v3) / 3.0f;
		area[i] = glm::length(glm::cross(v2-v1,v3-v1));
		up += r[i] * area[i];
		down += area[i];
	}
	centre_of_mess = up/down;
}