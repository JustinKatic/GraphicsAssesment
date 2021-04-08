#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "Camera.h"
#include <vector>
#include "Scene.h"

#define GLM_ENABLE_EXPERIMENTAL
#include<glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>


class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();


	void DrawLegs(glm::mat4 a_hipBone, glm::mat4 a_kneeBone, glm::mat4 a_ankleBone, glm::vec4 a_color);


protected:

	//Quaternion things
	glm::vec3 m_positions[2];
	glm::quat m_rotations[2];

	struct KeyFrame
	{
		glm::vec3 position;
		glm::quat rotation;
	};

	//White LEG FRONT
	KeyFrame  m_whiteLegHipFrames[2] = { glm::vec3(0),glm::vec3(0) };
	KeyFrame  m_whiteLegKneeFrames[2] = { glm::vec3(0),glm::vec3(0) };
	KeyFrame  m_whiteLegAnkleFrames[2] = { glm::vec3(0),glm::vec3(0) };
	glm::mat4 m_whiteLegHipBone = glm::mat4();
	glm::mat4 m_whiteLegKneeBone = glm::mat4();
	glm::mat4 m_whiteLegAnkleBone = glm::mat4();

	//Blue LEG FRONT
	KeyFrame  m_blueLegHipFrames[2] = { glm::vec3(0),glm::vec3(0) };
	KeyFrame  m_blueLegKneeFrames[2] = { glm::vec3(0),glm::vec3(0) };
	KeyFrame  m_blueLegAnkleFrames[2] = { glm::vec3(0),glm::vec3(0) };
	glm::mat4 m_blueLegHipBone = glm::mat4();
	glm::mat4 m_blueLegKneeBone = glm::mat4();
	glm::mat4 m_blueLegAnkleBone = glm::mat4();

	//Green LEG BACK
	KeyFrame  m_greenLegHipFrames[2] = { glm::vec3(0),glm::vec3(0) };
	KeyFrame  m_greenLegKneeFrames[2] = { glm::vec3(0),glm::vec3(0) };
	KeyFrame  m_greenLegAnkleFrames[2] = { glm::vec3(0),glm::vec3(0) };
	glm::mat4 m_greenLegHipBone = glm::mat4();
	glm::mat4 m_greenLegKneeBone = glm::mat4();
	glm::mat4 m_greenLegAnkleBone = glm::mat4();

	//RIGHT LEG BACK
	KeyFrame  m_redLegHipFrames[2] = { glm::vec3(0),glm::vec3(0) };
	KeyFrame  m_redLegKneeFrames[2] = { glm::vec3(0),glm::vec3(0) };
	KeyFrame  m_redLegAnkleFrames[2] = { glm::vec3(0),glm::vec3(0) };
	glm::mat4 m_redLegHipBone = glm::mat4();
	glm::mat4 m_redLegKneeBone = glm::mat4();
	glm::mat4 m_redLegAnkleBone = glm::mat4();

	void AddLeg(KeyFrame* a_hipFrame, KeyFrame* a_kneeFrame, KeyFrame* a_ankleFrame, glm::mat4& a_hipBone, glm::mat4& a_kneeBone, glm::mat4& a_ankleBone);

	void LegLerpPositions(int a_Index, KeyFrame* a_hipFrame, KeyFrame* a_kneeFrame, KeyFrame* a_ankleFrame,
		glm::vec3 a_hipPos, glm::quat a_hipRot, glm::vec3 a_kneePos, glm::quat a_kneeRot, glm::vec3 a_anklePos, glm::quat a_ankleRot);

	float m_directionalRotSpeed = 2;
	float m_directionalOrbitRadius = 1;
	glm::vec3 m_directionalLightPos = glm::vec3(0);

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	// SHADERs
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_normalMapShader;

	//Create a dragon with a flat color
	aie::OBJMesh m_dragonMesh;
	glm::mat4 m_dragonTransform;

	//Create a soulspear
	aie::OBJMesh m_soulSpearMesh;
	glm::mat4 m_soulSpearTransform;

	//Create a soulspear
	aie::OBJMesh m_coltMesh;
	glm::mat4 m_coltTransform;

	Scene* m_scene;

	glm::vec3	m_ambientLight = glm::vec3(0.25f);

public:
	bool LoadShaderAndMeshLogic(Light a_light);
	void IMGUI_Logic();
};