#pragma once

#include "Application.h"
#include <glm/mat4x4.hpp>
#include "Mesh.h"
#include "Shader.h"
#include "OBJMesh.h"
#include "Camera.h"
#include <vector>


class GraphicsProjectApp : public aie::Application {
public:

	GraphicsProjectApp();
	virtual ~GraphicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

	void DrawPlanets();

protected:
	
	
	std::vector<Camera> m_camera;
	int m_currentCam = 0;

	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::Texture m_gridTexture;

	// SHADER
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_bunnyShader;
	aie::ShaderProgram m_phongShader;
	aie::ShaderProgram m_textureShader;
	aie::ShaderProgram m_normalMapShader;



	//
	//Basic Plane
	Mesh m_quadMesh;
	glm::mat4 m_quadTransform;
	

	//Create a bunny with a flat color
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;
	//glm::vec3 m_bunnyPosition{ 0,0,0 };

	//Create a dragon with a flat color
	aie::OBJMesh m_dragonMesh;
	glm::mat4 m_dragonTransform;
	//glm::vec3 m_dragonPosition{ 0,0,0 };


	//Create a lucy with a flat color
	aie::OBJMesh m_lucyMesh;
	glm::mat4 m_lucyTransform;
	//glm::vec3 m_lucyPosition{ 0,0,0 };


	//Create a buddha with a flat color
	aie::OBJMesh m_buddhaMesh;
	glm::mat4 m_buddhaTransform;
	//glm::vec3 m_buddhaPosition{ 0,0,0 };

	//Create a soulspear
	aie::OBJMesh m_soulSpearMesh;
	glm::mat4 m_soulSpearTransform;

	//Create a soulspear
	aie::OBJMesh m_coltMesh;
	glm::mat4 m_coltTransform;


	struct Light
	{
		glm::vec3 direction{2.f,4.2f,-10.f};
		glm::vec3 color{ 0.5f,0.5f,0.5f };
	};

	Light		m_light;
	Light		m_light1;
	glm::vec3	m_ambientLight;

public:
	bool LoadShaderAndMeshLogic();
	void DrawShaderAndMeshes(glm::mat4, glm::mat4);
	void IMGUI_Logic();
};