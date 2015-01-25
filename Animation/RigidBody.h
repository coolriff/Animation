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

	glm::vec3 position;
	glm::quat orientation;
	glm::vec3 scale;
	glm::vec3 linearMomentum;
	glm::vec3 angularMomentum;
	glm::vec3 force;
	glm::vec3 torque;
	glm::mat3 inverseI;
	std::vector<glm::vec3> points;

	float massInverse;

	void Update(float deltaTime);

	glm::mat4 GetTransformationMatrix();

	void SetPoints(const std::vector<glm::vec3> & vertices);

	glm::vec3 GetPosition() const { return position; }
	glm::quat GetOrientation() const { return orientation; }
	glm::vec3 GetScale() const { return scale; }
	float GetMassInverse() const { return massInverse; } 
	glm::mat3 GetInertiaInverse() const;
	float GetMass() const { return (massInverse != 0.0f) ? 1.0f/massInverse : FLT_MAX; }
	glm::vec3 GetLinearMomentum() const { return linearMomentum; }
	glm::vec3 GetLinearVelocity() const { return linearMomentum * massInverse; }
	glm::vec3 GetAngularMomentum() const { return angularMomentum; }
	glm::vec3 GetAngularVelocity() const { return GetInertiaInverse() * angularMomentum; }
	glm::vec3 GetForce() const { return force; }
	glm::vec3 GetTorque() const { return torque; }

	void SetPosition(const glm::vec3 &p) { position = p; }
	void SetOrientation(const glm::quat &o) { orientation = o; }
	void SetScale(const glm::vec3 &s) { scale = s; }
	void SetLinearMomentum(const glm::vec3 &lm) { linearMomentum = lm; }
	void SetAngularMomentum(const glm::vec3 &am) { angularMomentum = am; }
	void SetForce(const glm::vec3 &f) { force = f; }
	void SetTorque(const glm::vec3 &t) { torque = t; }

	void ApplyGravity(const float g) { if(massInverse > 0) force += glm::vec3(0, 1.0f/massInverse, 0) * g; }
	void ApplyForce(const glm::vec3 &point, const glm::vec3 &f);
};

#endif