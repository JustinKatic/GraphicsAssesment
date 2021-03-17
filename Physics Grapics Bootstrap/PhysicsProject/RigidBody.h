#pragma once
#include "PhysicsObject.h"
#include <list>
#include  <functional>
#include <iostream>
class RigidBody : public PhysicsObject
{
public:
	RigidBody(ShapeType a_shapeID, glm::vec2 a_position, glm::vec2 a_velocity, float a_rotation, float a_mass);
	~RigidBody() {};

	virtual void FixedUpdate(glm::vec2 a_gravity, float a_timeStep);
	virtual void Debug() {};

	void ApplyForce(glm::vec2 a_force, glm::vec2 a_pos);

	void ResolveCollision(RigidBody* a_otherActor, glm::vec2 a_contact, glm::vec2* a_collisionNormal = nullptr, float a_pen = 0);


	glm::vec2 GetPosition() const { return m_position; }
	glm::vec2 GetVelocity() { return m_velocity; }
	float GetMass() { return m_isKinematic ? INT_MAX : m_mass; }
	float GetRotation() { return m_rotation; }

	float GetMoment() { return m_isKinematic ? INT_MAX : m_moment; }
	float GetAngularVelcity() { return m_angularVelocity; }

	glm::vec2 SetPosition(glm::vec2 a_newPos) { return m_position = a_newPos; }
	glm::vec2 SetVelocity(glm::vec2 a_velocity) { return m_velocity = a_velocity; }
	float SetRotation(float a_rotate) { return m_rotation = a_rotate; }

	float GetLinearDrag() { return m_linearDrag; }
	float GetAngluarDrag() { return m_angularDrag; }

	float SetAngularVelocity(float a_angularVelocity) { return m_angularVelocity = a_angularVelocity; }

	glm::vec2 ToWorld(glm::vec2 a_localPos);

	std::function<void(PhysicsObject*)> m_collisionCallback;

	void TriggerEntered(PhysicsObject* a_otherActor);
	std::function<void(PhysicsObject*)> m_triggerEnter;
	std::function<void(PhysicsObject*)> m_triggerExit;
	bool IsTrigger() { return m_isTrigger; }
	bool SetTrigger(bool a_state) { return m_isTrigger = a_state; }

protected:
	bool m_isTrigger = false;
	std::list<PhysicsObject*> m_objectInside;
	std::list<PhysicsObject*> m_objectInsideThisFrame;



	glm::vec2 m_position = glm::vec2(0);
	glm::vec2 m_velocity = glm::vec2(0);

	float m_mass = 0.f;
	float m_rotation = 0.f;
	float m_angularVelocity = 0.f;
	float m_moment = 0.f;

	float m_linearDrag = 0.f;
	float m_angularDrag = 0.f;

	//these will store the local x and y axes of the Rigidbody based on its angle of rotation
	glm::vec2 m_localX = glm::vec2(0);
	glm::vec2 m_localY = glm::vec2(0);

};

