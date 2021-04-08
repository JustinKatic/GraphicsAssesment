#include "Scene.h"

#include"Instance.h"

Scene::Scene(glm::vec2 a_windowSize, Light& a_light, glm::vec3 a_ambientLight)
	: m_windowSize(a_windowSize), m_light(a_light), m_ambientLight(a_ambientLight)
{
	m_pointLightColors[0] = glm::vec3(0);
	m_pointLightPositions[0] = glm::vec3(0);
	m_pointLightColors[1] = glm::vec3(0);
	m_pointLightPositions[1] = glm::vec3(0);
	m_pointLightColors[2] = glm::vec3(0);
	m_pointLightPositions[2] = glm::vec3(0);
	m_pointLightColors[3] = glm::vec3(0);
	m_pointLightPositions[3] = glm::vec3(0);
}

Scene::~Scene()
{
	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
		delete(*i);
}

void Scene::AddInstance(Instance* a_instance)
{
	m_instances.push_back(a_instance);
}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < m_pointLights.size(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].m_direction;
		m_pointLightColors[i] = m_pointLights[i].m_color;
	}

	for (auto i = m_instances.begin(); i != m_instances.end(); i++)
	{
		Instance* instance = *i;
		instance->Draw(this);
	}
}


