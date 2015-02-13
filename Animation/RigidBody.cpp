#include "RigidBody.h"
#define MAX 36

RigidBody::RigidBody(glm::vec3 &p, glm::quat &o, glm::vec3 &s)
{
	m_position = p;
	m_orientation = o;
	m_scale = s;
	m_linearMomentum = glm::vec3(0.0f);
	m_force = glm::vec3(0.0f);
	m_angularMomentum = glm::vec3(0.0f);
	m_torque = glm::vec3(0.0f);
	distanceFromCentreMessToPoint = NULL;
	maxX = 0.0f;
	maxY = 0.0f;
	maxZ = 0.0f;
	minX = 0.0f;
	minY = 0.0f;
	minZ = 0.0f;
	maxAABBx = 0.0f;
	maxAABBy = 0.0f;
	maxAABBz = 0.0f;
	AABBmax = glm::vec3(0.0f);
	AABBmin = glm::vec3(0.0f);
}

glm::mat4 RigidBody::GetTransformationMatrix()
{
	glm::mat4 T = glm::translate(m_position);
	glm::mat4 R = glm::toMat4(m_orientation);
	R = glm::inverse(R);
	glm::mat4 S = glm::scale(m_scale);

	return T * R * S;
}

glm::mat4 RigidBody::GetTransformationMatrix(glm::vec3 p, glm::vec3 s)
{
	glm::mat4 T = glm::translate(p);
	glm::mat4 R = glm::toMat4(m_orientation);
	glm::mat4 S = glm::scale(s);

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
	centre_of_mass = up/down;
}

void RigidBody::CalculateDistanceFromCentreOfMessToPoint(void)
{
	glm::vec3 boundingPoint = m_points.at(0);

	distanceFromCentreMessToPoint = glm::sqrt(
		((centre_of_mass.x - boundingPoint.x) * (centre_of_mass.x - boundingPoint.x)) + 
		((centre_of_mass.y - boundingPoint.y) * (centre_of_mass.y - boundingPoint.y)) + 
		((centre_of_mass.z - boundingPoint.z) * (centre_of_mass.z - boundingPoint.z))
		);

	distanceFromCentreMessToPoint = glm::abs(distanceFromCentreMessToPoint);
}

glm::vec3 RigidBody::getFurthestPoint(glm::vec3 &direction)
{
	int furthestPoint = 0;
	float max = FLT_MIN;

	glm::vec3 d = glm::normalize(glm::vec3(glm::toMat4(m_orientation) * glm::vec4(direction, 0.0f)));
	for(unsigned int i=0; i<m_points.size(); ++i)
	{
		glm::vec3 vertex = m_scale * m_points[i];
		float projection = glm::dot(vertex, d);

		if(projection > max)
		{
			furthestPoint = i;
			max = projection;
		}
	}

	printf("%f, %f, %f /n",m_points[furthestPoint].x,m_points[furthestPoint].y,m_points[furthestPoint].z);

// 	for(unsigned int i=0; i<m_points.size(); ++i)
// 	{
// 		glm::vec3 vertex = m_scale * m_points[i];
// 		float projection = glm::dot(vertex, direction);
// 
// 		if(projection > max)
// 		{
// 			furthestPoint = i;
// 			max = projection;
// 		}
// 	}
// 
// 	printf("%f, %f, %f",m_points[furthestPoint].x,m_points[furthestPoint].y,m_points[furthestPoint].z);

	return m_points[furthestPoint];
}

