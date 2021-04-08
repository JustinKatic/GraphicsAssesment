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

GraphicsProjectApp::GraphicsProjectApp() : m_dragonTransform(), m_projectionMatrix(), m_viewMatrix()
{
}

GraphicsProjectApp::~GraphicsProjectApp()
{
}

bool GraphicsProjectApp::startup()
{

	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	//create direction light
	Light light;
	light.m_color = { 1,1,1 };
	light.m_direction = { 1,-1,1 };

#pragma region White Leg Lerp Posistions
	//WHITE LEG lerp positions
	LegLerpPositions(0, m_whiteLegHipFrames, m_whiteLegKneeFrames, m_whiteLegAnkleFrames,
		glm::vec3(0, 5, 0), glm::vec3(1, 0, 0),			// hip pos, hip rot
		glm::vec3(0, -2.5f, 0), glm::vec3(1, 0, 0),		// knee pos, knee rot
		glm::vec3(0, -2.5f, 0), glm::vec3(-1, 0, 0));	// ankle pos, ankle rot

	LegLerpPositions(1, m_whiteLegHipFrames, m_whiteLegKneeFrames, m_whiteLegAnkleFrames,
		glm::vec3(0, 5, 0), glm::vec3(-1, 0, 0),		// hip pos, hip rot
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0),		// knee pos, knee rot
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0));	// ankle pos, ankle rot
#pragma endregion

#pragma region Blue Leg Lerp Posistions
	//BLUE LEG lerp positions
	LegLerpPositions(0, m_blueLegHipFrames, m_blueLegKneeFrames, m_blueLegAnkleFrames,
		glm::vec3(5, 5, 0), glm::vec3(-1, 0, 0),		// hip pos, hip rot
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0),		// knee pos, knee rot
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0));	// ankle pos, ankle rot

	LegLerpPositions(1, m_blueLegHipFrames, m_blueLegKneeFrames, m_blueLegAnkleFrames,
		glm::vec3(5, 5, 0), glm::vec3(1, 0, 0),			// hip pos, hip rot
		glm::vec3(0, -2.5f, 0), glm::vec3(1, 0, 0),		// knee pos, knee rot
		glm::vec3(0, -2.5f, 0), glm::vec3(-1, 0, 0));	// ankle pos, ankle rot
#pragma endregion

#pragma region Green Leg Lerp Posistions
	//LEFT LEG BACK lerp positions
	LegLerpPositions(0, m_greenLegHipFrames, m_greenLegKneeFrames, m_greenLegAnkleFrames,
		glm::vec3(0, 5, -10), glm::vec3(-1, 0, 0),		// hip pos, hip rot
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0),		// knee pos, knee rot
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0));	// ankle pos, ankle rot

	LegLerpPositions(1, m_greenLegHipFrames, m_greenLegKneeFrames, m_greenLegAnkleFrames,
		glm::vec3(0, 5, -10), glm::vec3(1, 0, 0),		// hip pos, hip rot
		glm::vec3(0, -2.5f, 0), glm::vec3(1, 0, 0),		// knee pos, knee rot
		glm::vec3(0, -2.5f, 0), glm::vec3(-1, 0, 0));	// ankle pos, ankle rot
#pragma endregion

#pragma region Red Leg Lerp Posistions
	//RIGHT LEG BACK lerp positions
	LegLerpPositions(0, m_redLegHipFrames, m_redLegKneeFrames, m_redLegAnkleFrames,
		glm::vec3(5, 5, -10), glm::vec3(1, 0, 0),		// hip pos, hip rot
		glm::vec3(0, -2.5f, 0), glm::vec3(1, 0, 0),		// knee pos, knee rot
		glm::vec3(0, -2.5f, 0), glm::vec3(-1, 0, 0));	// ankle pos, ankle rot

	LegLerpPositions(1, m_redLegHipFrames, m_redLegKneeFrames, m_redLegAnkleFrames,
		glm::vec3(5, 5, -10), glm::vec3(-1, 0, 0),		// hip pos, hip rot
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0),		// knee pos, knee rot
		glm::vec3(0, -2.5f, 0), glm::vec3(0, 0, 0));	// ankle pos, ankle rot
#pragma endregion

	//load shader mesh logic function
	return LoadShaderAndMeshLogic(light);
}

void GraphicsProjectApp::shutdown()
{

	Gizmos::destroy();
	delete m_scene;
}

void GraphicsProjectApp::update(float deltaTime)
{
	aie::Input* input = aie::Input::getInstance();

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draws a simple grid with gizmos
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
	//create ambient light
	m_scene->SetAmbientLight(m_ambientLight);

	//if current cam is not static allow cam to move
	if (m_scene->GetCurrentCamera()->m_isStatic == false)
		m_scene->GetCurrentCamera()->Update(deltaTime);

	//load imgui things
	IMGUI_Logic();

	//Change light direction over time
	float time = getTime();
	m_scene->GetLight().m_direction = glm::normalize(glm::vec3(glm::cos(time * m_directionalRotSpeed), glm::sin(time * m_directionalRotSpeed), 0)) + m_directionalLightPos;

	//gizmo for directional light
	Gizmos::addSphere(m_directionalOrbitRadius * m_scene->GetLight().m_direction, 0.25, 8, 8, glm::vec4(m_scene->GetLight().m_color, 1));

	//gizmo for point light 1
	Gizmos::addSphere(m_scene->GetPointLightPositions()[0], 0.25, 8, 8, glm::vec4(m_scene->GetPointLights()[0].m_color, 1));
	//gizmo for point light 2
	Gizmos::addSphere(m_scene->GetPointLightPositions()[1], 0.25, 8, 8, glm::vec4(m_scene->GetPointLights()[1].m_color, 1));

	//whiteLegLogic();
	AddLeg(m_whiteLegHipFrames, m_whiteLegKneeFrames, m_whiteLegAnkleFrames, m_whiteLegHipBone, m_whiteLegKneeBone, m_whiteLegAnkleBone);

	//blueLegLogic();
	AddLeg(m_blueLegHipFrames, m_blueLegKneeFrames, m_blueLegAnkleFrames, m_blueLegHipBone, m_blueLegKneeBone, m_blueLegAnkleBone);

	//greenLegLogic();
	AddLeg(m_greenLegHipFrames, m_greenLegKneeFrames, m_greenLegAnkleFrames, m_greenLegHipBone, m_greenLegKneeBone, m_greenLegAnkleBone);

	//redLegLogic();
	AddLeg(m_redLegHipFrames, m_redLegKneeFrames, m_redLegAnkleFrames, m_redLegHipBone, m_redLegKneeBone, m_redLegAnkleBone);

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

	//Draw white leg
	DrawLegs(m_whiteLegHipBone, m_whiteLegKneeBone, m_whiteLegAnkleBone, glm::vec4(1, 1, 1, 1));
	//Draw blue leg
	DrawLegs(m_blueLegHipBone, m_blueLegKneeBone, m_blueLegAnkleBone, glm::vec4(0, 0, 1, 1));
	//Draw green leg
	DrawLegs(m_greenLegHipBone, m_greenLegKneeBone, m_greenLegAnkleBone, glm::vec4(0, 1, 0, 1));
	//Draw red leg
	DrawLegs(m_redLegHipBone, m_redLegKneeBone, m_redLegAnkleBone, glm::vec4(1, 0, 0, 1));




	Gizmos::draw(projectionMatrix * viewMatrix);
}


void GraphicsProjectApp::DrawLegs(glm::mat4 a_hipBone, glm::mat4 a_kneeBone, glm::mat4 a_ankleBone, glm::vec4 a_color)
{
	//draw hip
	glm::vec3 hipPos = glm::vec3(a_hipBone[3].x, a_hipBone[3].y, a_hipBone[3].z);
	//draw knee
	glm::vec3 kneePos = glm::vec3(a_kneeBone[3].x, a_kneeBone[3].y, a_kneeBone[3].z);
	//draw ankle
	glm::vec3 anklePos = glm::vec3(a_ankleBone[3].x, a_ankleBone[3].y, a_ankleBone[3].z);

	//get centre pos
	glm::vec4 centre(0.5f);

	//draw hip
	Gizmos::addAABBFilled(hipPos, centre, a_color, &a_hipBone);
	//draw knee
	Gizmos::addAABBFilled(kneePos, centre, a_color, &a_kneeBone);
	//draw ankle
	Gizmos::addAABBFilled(anklePos, centre, a_color, &a_ankleBone);

	Gizmos::addAABBFilled(glm::vec3(5, 13, -10), glm::vec3(5, 2, 10), glm::vec4(1, 0, 0, 1));


	Gizmos::addLine(hipPos*2, kneePos*2, a_color);
	Gizmos::addLine(kneePos*2, anklePos*2, a_color);



}

void GraphicsProjectApp::LegLerpPositions(int a_Index, KeyFrame* a_hipFrame, KeyFrame* a_kneeFrame, KeyFrame* a_ankleFrame,
	glm::vec3 a_hipPos, glm::quat a_hipRot, glm::vec3 a_kneePos, glm::quat a_kneeRot, glm::vec3 a_anklePos, glm::quat a_ankleRot)
{
	a_hipFrame[a_Index].position = a_hipPos;
	a_hipFrame[a_Index].rotation = a_hipRot;

	a_kneeFrame[a_Index].position = a_kneePos;
	a_kneeFrame[a_Index].rotation = a_kneeRot;

	a_ankleFrame[a_Index].position = a_anklePos;
	a_ankleFrame[a_Index].rotation = a_ankleRot;
}



void GraphicsProjectApp::AddLeg(KeyFrame* a_hipFrame, KeyFrame* a_kneeFrame, KeyFrame* a_ankleFrame, glm::mat4& a_hipBone, glm::mat4& a_kneeBone, glm::mat4& a_ankleBone)
{
	// ANIMATE HIP
	float sHip = glm::cos(getTime()) * 0.5f + 0.5f;
	// linearly interpolate hip position
	glm::vec3 pHip = (1.0f - sHip) * a_hipFrame[0].position + sHip * a_hipFrame[1].position;
	// spherically interpolate hip rotation
	glm::quat rHip = glm::slerp(a_hipFrame[0].rotation, a_hipFrame[1].rotation, sHip);
	// update the hip bone
	a_hipBone = glm::translate(pHip) * glm::toMat4(rHip);

	// ANIMATE Knee
	float sKnee = glm::cos(getTime()) * 0.5f + 0.5f;
	// linearly interpolate hip position
	glm::vec3 pKnee = (1.0f - sKnee) * a_kneeFrame[0].position + sKnee * a_kneeFrame[1].position;
	// spherically interpolate hip rotation
	glm::quat rKnee = glm::slerp(a_kneeFrame[0].rotation, a_kneeFrame[1].rotation, sKnee);
	// update the hip bone
	a_kneeBone = a_hipBone * glm::translate(pKnee) * glm::toMat4(rKnee);

	// ANIMATE Ankle
	float sAnkle = glm::cos(getTime()) * 0.5f + 0.5f;
	// linearly interpolate hip position
	glm::vec3 pAnkle = (1.0f - sAnkle) * a_ankleFrame[0].position + sAnkle * a_ankleFrame[1].position;
	// spherically interpolate hip rotation
	glm::quat rAnkle = glm::slerp(a_ankleFrame[0].rotation, a_ankleFrame[1].rotation, sAnkle);
	// update the hip bone
	a_ankleBone = a_kneeBone * glm::translate(pAnkle) * glm::toMat4(rAnkle);
}

bool GraphicsProjectApp::LoadShaderAndMeshLogic(Light a_light)
{
	//load phne files
#pragma region phongShader
	m_phongShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/phong.vert");
	m_phongShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/phong.frag");

	if (m_phongShader.link() == false)
	{
		printf("Phong Shader had an error: %s\n", m_phongShader.getLastError());
		return false;
	}
#pragma endregion

	//load normal map files
#pragma region NormalMapShader
	m_normalMapShader.loadShader(aie::eShaderStage::VERTEX, "./shaders/normalMap.vert");
	m_normalMapShader.loadShader(aie::eShaderStage::FRAGMENT, "./shaders/normalMap.frag");

	if (m_normalMapShader.link() == false)
	{
		printf("Normal Map Shader had an error: %s\n", m_normalMapShader.getLastError());
		return false;
	}
#pragma endregion

	//load dragon files
#pragma region DragonLoad
	if (m_dragonMesh.load("./stanford/dragon.obj") == false)
	{
		printf("Dragon Mesh Failed!\n");
		return false;
	}
#pragma endregion

	//load spear files
#pragma region SoulSpearLoad
	if (m_soulSpearMesh.load("./soulspear/soulspear.obj", true, true) == false)
	{
		printf("Soulspear Mesh has had an error!\n");
		return false;
	}
#pragma endregion

	//load colt files
#pragma region ColtLoad
	if (m_coltMesh.load("./colt/source/colt.obj", true, true) == false)
	{
		printf("Saber Mesh has had an error!\n");
		return false;
	}
#pragma endregion

	//add instances of objects/lights to scene
#pragma region AddInstances

	//creat an instance of scene
	m_scene = new Scene(glm::vec2(getWindowWidth(), getWindowHeight()), a_light, glm::vec3(0.25f));

	//push back cameras
	m_scene->m_camera.push_back(new Camera(false, glm::vec3(0, 0, 0))); // FLY CAM
	m_scene->m_camera.push_back(new Camera(true, glm::vec3(30, 0, 0)));  //static 1 looking down
	m_scene->m_camera.push_back(new Camera(true, glm::vec3(0, 30, 0)));  //static 2 looking forward
	m_scene->m_camera.push_back(new Camera(true, glm::vec3(0, 0, 30))); //static 3 looking up





	//ADD COLT [0]
	m_scene->AddInstance(new Instance(glm::vec3(5, 0, 0), glm::vec3(0, 30, 0), glm::vec3(0.01), &m_coltMesh, &m_normalMapShader));
	//ADD DRAGON [1]
	m_scene->AddInstance(new Instance(glm::vec3(0, 0, 5), glm::vec3(0, 30, 0), glm::vec3(1), &m_dragonMesh, &m_phongShader));
	//ADDSPEAR [2]
	m_scene->AddInstance(new Instance(glm::vec3(0, 0, 0), glm::vec3(0, 30, 0), glm::vec3(1), &m_soulSpearMesh, &m_normalMapShader));


	//add a red light on the left side
	m_scene->GetPointLights().push_back(Light(vec3(5, 3, 0), vec3(1, 0, 0), 50));
	//add a green light on the right
	m_scene->GetPointLights().push_back(Light(vec3(-5, 3, 0), vec3(0, 1, 0), 50));


	return true;
}
#pragma endregion


void GraphicsProjectApp::IMGUI_Logic()
{

	//DIRECTIONAL LIGHT CONTROLS
#pragma region DirectionLightControls
	ImGui::Begin("Scene Directional Light Settings");
	ImGui::DragFloat3("Directional Light Color", &m_scene->GetLight().m_color[0], 0.1f, 0.0f, 2.0f);
	ImGui::DragFloat("Directional Light RotSpeed", &m_directionalRotSpeed, 0.1f, 0.0f, 20.0f);
	ImGui::DragFloat("directional Orbit Radius", &m_directionalOrbitRadius, 0.1f, 0.0f, 20.0f);
	ImGui::DragFloat3("Directional Light Pos", &m_directionalLightPos[0], 0.1f, -50.0f, 50.0f);
	ImGui::End();
#pragma endregion

	//POINT LIGHT CONTROLS
#pragma region PointLightControls
	ImGui::Begin("Scene Point Light Settings");
	ImGui::DragFloat3("Light1 Position", &m_scene->GetPointLights()[0].m_direction[0], 0.1, -50, 50.0f);
	ImGui::DragFloat3("Light1 Color", &m_scene->GetPointLights()[0].m_color[0], 0.1, 0.0f, 2.0f);
	ImGui::DragFloat3("Light2 Position", &m_scene->GetPointLights()[1].m_direction[0], 0.1, -50, 50.0f);
	ImGui::DragFloat3("Light2 Color", &m_scene->GetPointLights()[1].m_color[0], 0.1, 0.0f, 2.0f);
	ImGui::DragFloat3("Ambient Light Color", &m_ambientLight[0], 0.1, 0.0f, 2.0f);
	ImGui::End();
#pragma endregion

	//COLT TRANSFORM
#pragma region ColtTransform
	ImGui::Begin("Position Settings");
	ImGui::DragFloat3("Colt Position", &m_scene->m_instances[0]->m_pos[0], 0.01, -20.0f, 20.0f);
	ImGui::DragFloat3("Colt Rotation", &m_scene->m_instances[0]->m_rot[0], 0.05, -180.0f, 180.0f);
	ImGui::DragFloat3("Colt Scale", &m_scene->m_instances[0]->m_scale[0], 0.01, -0.01f, 3.0f);
#pragma endregion

	//DRAGON TRANSFORM
#pragma region DragonTransform
	ImGui::DragFloat3("Dragon Position", &m_scene->m_instances[1]->m_pos[0], 0.01, -20.0f, 20.0f);
	ImGui::DragFloat3("Dragon Rotation", &m_scene->m_instances[1]->m_rot[0], 0.05, -180.0f, 180.0f);
	ImGui::DragFloat3("Dragon Scale", &m_scene->m_instances[1]->m_scale[0], 0.01, 0.01f, 3.0f);
#pragma endregion

	//SPEAR TRANSFORM
#pragma region SpearTransform
	ImGui::DragFloat3("Spear Position", &m_scene->m_instances[2]->m_pos[0], 0.01, -20.0f, 20.0f);
	ImGui::DragFloat3("Spear Rotation", &m_scene->m_instances[2]->m_rot[0], 0.05, -180.0f, 180.0f);
	ImGui::DragFloat3("Spear Scale", &m_scene->m_instances[2]->m_scale[0], 0.01, 0.01f, 3.0f);
	ImGui::End();
#pragma endregion

	//CAM CONTROLS
#pragma region CamControls
	ImGui::Begin("Camera Select");
	ImGui::Text("CamMovement");
	ImGui::DragFloat("camMoveSpeed", &m_scene->GetCurrentCamera()->m_speed, 0.1, 0, 100);
	ImGui::DragFloat("camRotateSpeed", &m_scene->GetCurrentCamera()->m_rotateSpeed, 0.1, 0, 100);
	ImGui::Text("CamList");
	ImGui::TextColored(ImVec4(0, 1, 0, 1), ("Current Cam: " + std::to_string(m_scene->currentCam)).c_str());

	for (int i = 0; i < m_scene->m_camera.size(); i++)
	{
		if (ImGui::Button(("Select Cam: " + std::to_string(i)).c_str(), ImVec2(100, 20)))
		{
			m_scene->currentCam = i;
		}
	}
	ImGui::End();
#pragma endregion

	//recalculate the transform to its current pos 
	for (auto instance : m_scene->m_instances)
		instance->RecalculateTransform();
}


