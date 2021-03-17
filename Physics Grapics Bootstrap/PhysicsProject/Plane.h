#pragma once
#include "PhysicsObject.h"

class RigidBody;

class Plane : public PhysicsObject
{
	//this is a one-sided object that extends infinitly along both its edge and backwards from its normal direction.
public:
	Plane(glm::vec2 a_normal, float a_distance);
	Plane();
	~Plane();

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void Draw() {};
	virtual void Debug() {};
	virtual void ResetPosition() {};
	virtual void MakeGizmo();
	virtual bool IsInside(glm::vec2 a_point) { return false; }


	void ResolveCollision(RigidBody* a_otherActor, glm::vec2 a_contactPoint);


	glm::vec2 GetNormal() { return m_normal; }
	float GetDistance() { return m_distanceToOrigin; }
	glm::vec4 GetColor() { return m_color; }


protected:

	glm::vec2 m_normal;
	float m_distanceToOrigin;
	glm::vec4 m_color;


};

