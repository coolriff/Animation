#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/transform.hpp"
#include <algorithm>
#include <map>
#include <vector>

class RigidBody
{
public:
	RigidBody(glm::vec3 &p = glm::vec3(), glm::quat &o = glm::quat(), glm::vec3 &s = glm::vec3(1,1,1));

	glm::vec3 m_position;
	glm::quat m_orientation;
	glm::vec3 m_scale;
	glm::vec3 m_linearMomentum;
	glm::vec3 m_angularMomentum;
	glm::vec3 m_force;
	glm::vec3 m_torque;
	glm::mat3 m_inverseI;
	glm::vec3 centre_of_mess;
	std::vector<glm::vec3> m_currentVerticesPosition;
	std::vector<glm::vec3> m_points;

	float m_massInverse;
	float distanceFromCentreMessToPoint;

	float maxX,maxY,maxZ,minX,minY,minZ;
	float maxAABBx;
	float maxAABBy;
	float maxAABBz;
	glm::vec3 AABBmax,AABBmin;

	void Update(float deltaTime);

	glm::mat4 GetTransformationMatrix();
	glm::mat4 GetTransformationMatrix(glm::vec3 p, glm::vec3 s);

	void SetPoints(const std::vector<glm::vec3> & vertices);

	glm::vec3 GetPosition() const { return m_position; }
	glm::quat GetOrientation() const { return m_orientation; }
	glm::vec3 GetScale() const { return m_scale; }
	float GetMassInverse() const { return m_massInverse; } 
	glm::mat3 GetInertiaInverse() const;
	float GetMass() const { return (m_massInverse != 0.0f) ? 1.0f/m_massInverse : FLT_MAX; }
	glm::vec3 GetLinearMomentum() const { return m_linearMomentum; }
	glm::vec3 GetLinearVelocity() const { return m_linearMomentum * m_massInverse; }
	glm::vec3 GetAngularMomentum() const { return m_angularMomentum; }
	glm::vec3 GetAngularVelocity() const { return GetInertiaInverse() * m_angularMomentum; }
	glm::vec3 GetForce() const { return m_force; }
	glm::vec3 GetTorque() const { return m_torque; }

	void SetPosition(const glm::vec3 &p) { m_position = p; }
	void SetOrientation(const glm::quat &o) { m_orientation = o; }
	void SetScale(const glm::vec3 &s) { m_scale = s; }
	void SetLinearMomentum(const glm::vec3 &lm) { m_linearMomentum = lm; }
	void SetAngularMomentum(const glm::vec3 &am) { m_angularMomentum = am; }
	void SetForce(const glm::vec3 &f) { m_force = f; }
	void SetTorque(const glm::vec3 &t) { m_torque = t; }

	void ApplyGravity(const float g) { if(m_massInverse > 0) m_force += glm::vec3(0, 1.0f/m_massInverse, 0) * g; }
	void ApplyForce(const glm::vec3 &point, const glm::vec3 &f);
	void updateCurrentVertices(int verticesSize, std::vector<glm::vec3> & vertices);
	void CalculateCentreOfMess(int verticesSize);
	void CalculateDistanceFromCentreOfMessToPoint(void);
};

#endif