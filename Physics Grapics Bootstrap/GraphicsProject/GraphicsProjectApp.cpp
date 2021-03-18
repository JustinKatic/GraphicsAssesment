#include "GraphicsProjectApp.h"
#include "Gizmos.h"
#include "Input.h"
#include <imgui.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <vector>
#include "Scene.h"
#include "Instance.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsProjectApp::GraphicsProjectApp() : m_ambientLight(), m_bunnyTransform(), m_dragonTransform(), m_lucyTransform(), m_buddhaTransform(), m_projectionMatrix(), m_quadTransform(), m_viewMatrix()
{
}

GraphicsProjectApp::~GraphicsProjectApp()
{
}

bool GraphicsProjectApp::startup() {

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	//m_camera.push_back(Camera());


	Light light;
	light.m_color = { 1,1,1 };
	light.m_direction = { 1,-1,1 };

	return LoadShaderAndMeshLogic(light);
}

void GraphicsProjectApp::shutdown() {

	Gizmos::destroy();
	delete m_scene;
}

void GraphicsProjectApp::update(float deltaTime)
{

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

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));

	//make all camera but 1st cam static
	if (m_scene->currentCam == 0)
		m_scene->GetCurrentCamera()->Update(deltaTime);

	IMGUI_Logic();


	//Change light direction over time
	float time = getTime();
	m_scene->GetLight().m_direction = glm::normalize(glm::vec3(glm::cos(time * 2), glm::sin(time * 2), 0));

	//Gizmos for direction light
	Gizmos::addSphere(m_scene->GetLight().m_direction, 0.25, 8, 8, glm::vec4(m_scene->GetLight().m_color, 1));

	//gizmo for point light 1
	Gizmos::addSphere(m_scene->GetPointLightPositions()[0], 0.25, 8, 8, glm::vec4(m_scene->GetPointLights()[0].m_color, 1));
	//gizmo for point light 2
	Gizmos::addSphere(m_scene->GetPointLightPositions()[1], 0.25, 8, 8, glm::vec4(m_scene->GetPointLights()[1].m_color, 1));


		if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
			quit();
}

void GraphicsProjectApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();

	glm::mat4 projectionMatrix = m_scene->GetCurrentCamera()->GetProjectionMatrix(getWindowWidth(), (float)getWindowHeight());

	glm::mat4 viewMatrix = m_scene->GetCurrentCamera()->GetViewMatrix();

	// DrawShaderAndMeshes(projectionMatrix, viewMatrix);
	m_scene->Draw();

	Gizmos::draw(projectionMatrix * viewMatrix);
}

bool GraphicsProjectApp::LoadShaderAndMeshLogic(Light a_light)
{
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

#pragma region DragonLoad
	if (m_dragonMesh.load("./stanford/dragon.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}
#pragma endregion

#pragma region SoulSpearLoad
	if (m_soulSpearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh has had an error!\n");
		return false;
	}
#pragma endregion

#pragma region ColtLoad
	if (m_coltMesh.load("./colt/source/colt.obj", true, true) == false)
	{
		printf("Saber Mesh has had an error!\n");
		return false;
	}

#pragma endregion

#pragma region AddInstances

	m_camera.push_back(new Camera(glm::vec3(-5, 0, 0))); // FLY CAM
	m_camera.push_back(new Camera(glm::vec3(0, 0, 0)));  //static 1
	m_camera.push_back(new Camera(glm::vec3(5, 0, 0)));  //static 2
	m_camera.push_back(new Camera(glm::vec3(0, 10, 0))); //static 3


	m_scene = new Scene(m_camera, glm::vec2(getWindowWidth(), getWindowHeight()), a_light, glm::vec3(0.25f));


	m_scene->AddInstance(new Instance(glm::vec3(5, 0, 0), glm::vec3(0, 30, 0), glm::vec3(0.01), &m_coltMesh, &m_normalMapShader));

	m_scene->AddInstance(new Instance(glm::vec3(0, 0, 5), glm::vec3(0, 30, 0), glm::vec3(1), &m_dragonMesh, &m_phongShader));

	m_scene->AddInstance(new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 30, 0), glm::vec3(1), &m_soulSpearMesh, &m_normalMapShader));


	//add a red light on the left side
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
	//add a green light on the right
	m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));


	return true;
#pragma endregion
}

void GraphicsProjectApp::IMGUI_Logic()
{
	ImGui::Begin("Scene Light Settings");
	ImGui::DragFloat3("Sunlight Direction", &m_scene->GetLight().m_direction[0], 0.1f, -20.0f, 20.0f);
	ImGui::DragFloat3("Sunlight Color", &m_scene->GetLight().m_color[0], 0.1f, 0.0f, 2.0f);
	ImGui::End();

	ImGui::Begin("Scene Point Light Settings");
	ImGui::DragFloat3("Light1 Position", &m_scene->GetPointLights()[0].m_direction[0], 0.1, -50, 50.0f);
	ImGui::DragFloat3("Light1 Color", &m_scene->GetPointLights()[0].m_color[0], 0.1, 0.0f, 2.0f);

	ImGui::DragFloat3("Light2 Position", &m_scene->GetPointLights()[1].m_direction[0], 0.1, -50, 50.0f);
	ImGui::DragFloat3("Light2 Color", &m_scene->GetPointLights()[1].m_color[0], 0.1, 0.0f, 2.0f);
	ImGui::End();


	ImGui::Begin("Position Settings");
	ImGui::DragFloat3("Colt Position", &m_scene->m_instances[0]->m_pos[0], 0.01, -20.0f, 20.0f);
	ImGui::DragFloat3("Colt Rotation", &m_scene->m_instances[0]->m_rot[0], 0.05, -180.0f, 180.0f);
	ImGui::DragFloat3("Colt Scale", &m_scene->m_instances[0]->m_scale[0], 0.01, -0.01f, 3.0f);

	ImGui::DragFloat3("Dragon Position", &m_scene->m_instances[1]->m_pos[0], 0.01, -20.0f, 20.0f);
	ImGui::DragFloat3("Dragon Rotation", &m_scene->m_instances[1]->m_rot[0], 0.05, -180.0f, 180.0f);
	ImGui::DragFloat3("Dragon Scale", &m_scene->m_instances[1]->m_scale[0], 0.01, 0.01f, 3.0f);

	ImGui::DragFloat3("Spear Position", &m_scene->m_instances[2]->m_pos[0], 0.01, -20.0f, 20.0f);
	ImGui::DragFloat3("Spear Rotation", &m_scene->m_instances[2]->m_rot[0], 0.05, -180.0f, 180.0f);
	ImGui::DragFloat3("Spear Scale", &m_scene->m_instances[2]->m_scale[0], 0.01, 0.01f, 3.0f);
	ImGui::End();

	ImGui::Begin("Camera Select");
	ImGui::Text("CamMovement");
	ImGui::DragFloat("camMoveSpeed", &m_scene->GetCurrentCamera()->m_speed, 0.1, 0, 100);
	ImGui::DragFloat("camRotateSpeed", &m_scene->GetCurrentCamera()->m_rotateSpeed, 0.1, 0, 100);
	ImGui::Text("CamList");
	ImGui::TextColored(ImVec4(0,1,0,1), ("Current Cam: " + std::to_string(m_scene->currentCam)).c_str());

	for (int i = 0; i < m_scene->m_camera.size(); i++)
	{
		if (ImGui::Button(("Select Cam: " + std::to_string(i)).c_str(), ImVec2(100, 20)))
		{
			m_scene->currentCam = i;
		}
	}
	ImGui::End();


	for (auto instance : m_scene->m_instances)
		instance->RecalculateTransform();
}


