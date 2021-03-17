#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp() : m_ambientLight(), m_bunnyTransform(), m_dragonTransform(), m_lucyTransform(), m_buddhaTransform(), m_light(), m_projectionMatrix(), m_quadTransform(), m_viewMatrix()
{

}

GraphicsProjectApp::~GraphicsProjectApp() {

}

bool GraphicsProjectApp::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	//m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	m_camera.push_back(Camera());

	m_ambientLight = { 0.25f,0.25f,0.25f };

	return LoadShaderAndMeshLogic();
}

void GraphicsProjectApp::shutdown() {

	Gizmos::destroy();
}

void GraphicsProjectApp::update(float deltaTime) {

	aie::Input* input = aie::Input::getInstance();


	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
			vec3(-10 + i, 0, -10),
			i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
			vec3(-10, 0, -10 + i),
			i == 10 ? white : black);
	}

	//m_bunnyTransform = glm::translate(m_bunnyTransform, m_bunnyPosition);
	//m_dragonTransform = glm::translate(m_dragonTransform, m_dragonPosition);
	//m_lucyTransform = glm::translate(m_lucyTransform, m_lucyPosition);
	//m_buddhaTransform = glm::translate(m_buddhaTransform, m_buddhaPosition);



	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));


	if (input->wasKeyPressed(aie::INPUT_KEY_UP))
		if (m_currentCam < m_camera.size() - 1)
			m_currentCam++;
		else
			m_currentCam = 0;

	if (input->wasKeyPressed(aie::INPUT_KEY_DOWN))
		if (m_currentCam > 0)
			m_currentCam--;
		else
			m_currentCam = m_camera.size() - 1;

	m_camera[m_currentCam].Update(deltaTime);

	IMGUI_Logic();


	//Change light direction over time
	 float time = getTime();
	 m_light.direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));


	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void GraphicsProjectApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	glm::mat4 projectionMatrix = m_camera[m_currentCam].GetProjectionMatrix(getWindowWidth(), (float)getWindowHeight());

	glm::mat4 viewMatrix = m_camera[m_currentCam].GetViewMatrix();

	// update perspective based on screen size
	//m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, getWindowWidth() / (float)getWindowHeight(), 0.1f, 1000.0f);

	DrawShaderAndMeshes(projectionMatrix, viewMatrix);

	Gizmos::draw(projectionMatrix * viewMatrix);
}

void GraphicsProjectApp::DrawPlanets()
{
	glm::vec3 sun = glm::vec3(0, 0, 0);
	glm::vec3 mercury = glm::vec3(5);

	float mercuryRing = 0.f;

	mercuryRing = glm::distance(sun, mercury);


	Gizmos::addSphere(sun, 1, 16, 16, glm::vec4(0.4f, 0.5f, 0, 1));


	Gizmos::addSphere(glm::vec3(cos(0.5f * getTime()), 0, sin(0.5f * getTime())) + mercury, 0.1f, 10, 10, glm::vec4(0.5f, 0, 0, 1));

}

bool GraphicsProjectApp::LoadShaderAndMeshLogic()
{
#pragma region LoadShadersFromFile
#pragma region TextureShader
	m_textureShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/textured.vert");
	m_textureShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/textured.frag");
	if (m_textureShader.link() == false)
	{
		printf("Textured Shader had an error: %s\n", m_textureShader.getLastError());
		return false;
	}


#pragma endregion

#pragma region QuadShader

	// Load the Vertex shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	//Load the fragment shader from a file
	m_simpleShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_simpleShader.link())
	{
		printf("Simple Shader had an error: %s\n", m_simpleShader.getLastError());
		return false;
	}


	//m_quadTransform = glm::rotate(m_quadTransform, 3.14f / 2, glm::vec3(1, 0, 0));

#pragma endregion

#pragma region BunnyShader
	// Load the Vertex shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/simple.vert");

	//Load the fragment shader from a file
	m_bunnyShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/simple.frag");

	if (!m_bunnyShader.link())
	{
		printf("Bunny Shader had an error: %s\n", m_bunnyShader.getLastError());
		return false;
	}
#pragma endregion

#pragma region phongShader
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	if (m_phongShader.link() == false)
	{
		printf("Phong Shader had an error: %s\n", m_phongShader.getLastError());
		return false;
	}
#pragma endregion

#pragma region NormalMapShader
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");

	if (m_normalMapShader.link() == false)
	{
		printf("Normal Map Shader had an error: %s\n", m_normalMapShader.getLastError());
		return false;
	}
#pragma endregion


#pragma endregion

#pragma region createObjects

#pragma region BunnyLogic
//	if (m_bunnyMesh.load("./stanford/bunny.obj") == false)
//	{
//		printf("Bunny Mesh Failed!\n");
//		return false;
//	}
//
//	m_bunnyTransform = {
//		0.5f,0,0,0,
//		0,0.5f,0,0,
//		0,0,0.5f,0,
//		-5,0,0,1
//	};
//
//#pragma endregion
//
//#pragma region QuadLogic
//	Mesh::Vertex vertices[4];
//	vertices[0].position = { -0.5f, 0.f, 0.5f, 1.f };
//	vertices[1].position = { 0.5f, 0.f, 0.5f, 1.f };
//	vertices[2].position = { -0.5f, 0.f, -0.5f, 1.f };
//	vertices[3].position = { 0.5f, 0.f, -0.5f, 1.f };
//
//	unsigned int indices[6] = { 0,1,2,2,1,3 };
//
//	m_quadMesh.InitialiseQuad();
//	//m_quadMesh.Initialise(4, vertices, 6, indices);
//
//	//We will make the quad 10 units by 10 units
//	m_quadTransform = {
//		10,0,0,0,
//		0,10,0,0,
//		0,0,10,0,
//		0,0,0,1
//	};
//
#pragma endregion
//
#pragma region DragonLogic
//	if (m_dragonMesh.load("./stanford/dragon.obj") == false)
//	{
//		printf("Dragon Mesh Failed!\n");
//		return false;
//	}
//
//	m_dragonTransform = {
//		0.5f,0,0,0,
//		0,0.5f,0,0,
//		0,0,0.5f,0,
//		5,0,0,1
//	};
#pragma endregion
//
#pragma region LucyLogic
//	if (m_lucyMesh.load("./stanford/lucy.obj") == false)
//	{
//		printf("lucy Mesh Failed!\n");
//		return false;
//	}
//
//	m_lucyTransform = {
//		0.5f,0,0,0,
//		0,0.5f,0,0,
//		0,0,0.5f,0,
//		0,0,-5,1
//	};
#pragma endregion
//
#pragma region DragonLogic
//	if (m_buddhaMesh.load("./stanford/buddha.obj") == false)
//	{
//		printf("Buddha Mesh Failed!\n");
//		return false;
//	}
//
//	m_buddhaTransform = {
//		0.5f,0,0,0,
//		0,0.5f,0,0,
//		0,0,0.5f,0,
//		0,0,5,1
//	};
#pragma endregion
//
#pragma region SoulSpear
	if (m_soulSpearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh has had an error!\n");
		return false;
	}
	m_soulSpearTransform = {
	1.0f,	0,		0,		0,
	0,		1.0f,	0,		0,
	0,		0,		1.0f,	0,
	0,		0,		0,		1
	};

#pragma endregion

#pragma region Colt
	if (m_coltMesh.load("./colt/source/colt.obj", true, true) == false)
	{
		printf("Saber Mesh has had an error!\n");
		return false;
	}
	m_coltTransform = {
	0.01f,	0,		0,		0,
	0,		0.01f,	0,		0,
	0,		0,		0.01f,	0,
	0,		0,		0,		1
	};
#pragma endregion

#pragma region GridLogic
	if (m_gridTexture.load("./textures/numbered_grid.tga") == false)
	{
		printf("Failed to load: numbered_grid.tga\n");
		return false;
	}
#pragma endregion



#pragma endregion

	return true;
}

void GraphicsProjectApp::DrawShaderAndMeshes(glm::mat4 a_projectionMatrix, glm::mat4 a_viewMatrix)
{
	auto pvm = a_projectionMatrix * a_viewMatrix * glm::mat4(0);
#pragma region Quad
	//Bind the shader
	m_textureShader.bind();

	//Bind the transforms of the mesh

	pvm = a_projectionMatrix * a_viewMatrix * m_quadTransform; //PVM = Projection View Matrix
	m_textureShader.bindUniform("ProjectionViewModel", pvm);

	//Bind the texture to a location on your choice (0)
	m_textureShader.bindUniform("diffuseTexture", 0);

	//bind the texture to the specific location
	m_gridTexture.bind(0);

	//Draw the quad...
	m_quadMesh.Draw();
#pragma endregion


//#pragma region Phong
//	//Bind the shader
//	m_phongShader.bind();
//
//	//Bind the camera position
//	m_phongShader.bindUniform("CameraPosition", vec3(glm::inverse(a_viewMatrix)[3]));
//
//	//bind the light
//	m_phongShader.bindUniform("AmbientColor", m_ambientLight);
//	m_phongShader.bindUniform("LightColor", m_light.color);
//	m_phongShader.bindUniform("LightDirection", m_light.direction);
//
//#pragma endregion
//
//#pragma region DrawBunny
//	//Bind the PVM
//	pvm = a_projectionMatrix * a_viewMatrix * m_bunnyTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//	//Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_bunnyTransform);
//
//	m_bunnyMesh.draw();
//#pragma endregion
//
//#pragma region DrawDragon
//	//Bind the PVM
//	pvm = a_projectionMatrix * a_viewMatrix * m_dragonTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//	//Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_dragonTransform);
//
//	m_dragonMesh.draw();
//#pragma endregion
//
//#pragma region DrawLucy
//	//Bind the PVM
//	pvm = a_projectionMatrix * a_viewMatrix * m_lucyTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//	//Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_lucyTransform);
//
//	m_lucyMesh.draw();
//#pragma endregion
//
//#pragma region DrawBuddha
//	//Bind the PVM
//	pvm = a_projectionMatrix * a_viewMatrix * m_buddhaTransform;
//	m_phongShader.bindUniform("ProjectionViewModel", pvm);
//	//Bind the lighting transforms
//	m_phongShader.bindUniform("ModelMatrix", m_buddhaTransform);
//
//	m_buddhaMesh.draw();
//#pragma endregion

#pragma region Soulspear
	m_normalMapShader.bind();

	//bind the transform
	pvm = a_projectionMatrix * a_viewMatrix * m_soulSpearTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("CameraPosition", m_camera[m_currentCam].GetPosition());
	m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
	m_normalMapShader.bindUniform("LightColor", m_light.color);
	m_normalMapShader.bindUniform("LightDirection", m_light.direction);
	m_normalMapShader.bindUniform("LightColor1", m_light1.color);
	m_normalMapShader.bindUniform("LightDirection1", m_light1.direction);
	m_normalMapShader.bindUniform("ModelMatrix", m_soulSpearTransform);

	//Draw the mesh
	m_soulSpearMesh.draw();

#pragma endregion

#pragma region Colt
	m_normalMapShader.bind();

	//bind the transform
	pvm = a_projectionMatrix * a_viewMatrix * m_coltTransform;
	m_normalMapShader.bindUniform("ProjectionViewModel", pvm);
	m_normalMapShader.bindUniform("CameraPosition", m_camera[m_currentCam].GetPosition());
	m_normalMapShader.bindUniform("AmbientColor", m_ambientLight);
	m_normalMapShader.bindUniform("LightColor", m_light.color);
	m_normalMapShader.bindUniform("LightDirection", m_light.direction);
	m_normalMapShader.bindUniform("LightColor1", m_light1.color);
	m_normalMapShader.bindUniform("LightDirection1", m_light1.direction);
	m_normalMapShader.bindUniform("ModelMatrix", m_coltTransform);

	//Draw the mesh
	m_coltMesh.draw();

#pragma endregion



}

void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_light.direction[0], 0.1f, -20.0f, 20.0f);
	ImGui::DragFloat3("Sunlight Color", &m_light.color[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();

	ImGui::Begin("Scene Light1 Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_light1.direction[0], 0.1f, -20.0f, 20.0f);
	ImGui::DragFloat3("Sunlight Color", &m_light1.color[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();

	ImGui::Begin("Position Settings");
	ImGui::DragFloat3("SoulSpear", &m_soulSpearTransform[3][0], 0.01, -20.0f, 20.0f);
	ImGui::DragFloat3("ColtPosition", &m_coltTransform[3][0], 0.01, -20.0f, 20.0f);

	ImGui::End();

	ImGui::Begin("Camera Select");

	ImGui::Text("CamMovement");
	ImGui::DragFloat("camMoveSpeed", &m_camera[m_currentCam].m_speed, 0.1, 0, 100);
	ImGui::DragFloat("camRotateSpeed", &m_camera[m_currentCam].m_rotateSpeed, 0.1, 0, 100);



	ImGui::Text("AddCam");
	if (ImGui::Button("Add Cam", ImVec2(100, 20)))
	{
		m_camera.push_back(Camera());
	}

	ImGui::Text("CamSelect");
	for (int i = 0; i < m_camera.size(); i++)
	{
		if (ImGui::Button(std::to_string(i).c_str(), ImVec2(50, 20)))
		{
			m_currentCam = i;
		}
	}

	ImGui::End();

}
