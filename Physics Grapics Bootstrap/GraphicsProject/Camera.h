#pragma once
#include <glm/glm.hpp>
class Camera
{
public:
	Camera(bool a_isStatic, glm::vec3 a_camPos);
	~Camera() {};

	void Update(float a_deltaTime);
	glm::vec3 GetPosition() { return m_position; }

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float a_width, float a_height);

	bool m_isStatic = true;
	float m_speed = 20;
	float m_rotateSpeed = 20;
private:
	float m_theta;			//In degrees
	float m_phi;			//In degrees
	glm::vec3 m_position;

	float m_lastMouseX;
	float m_lastMouseY;



};

