#pragma once

#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"

#include "PhysicsScene.h"
#include "Sphere.h"
#include"Box.h"
#include <vector>


class PhysicsProjectApp : public aie::Application
{
public:

	PhysicsProjectApp();
	virtual ~PhysicsProjectApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	glm::vec2 worldPos = glm::vec2(0);

	aie::Renderer2D* m_2dRenderer = nullptr;
	aie::Font* m_font = nullptr;

	float extraForce = 60;
	float ballCollectedXPos = 130;
	bool hasABallBeenSunk = false;

	PhysicsScene* m_physicsScene = nullptr;



	void ChooseWhiteBallLocation(aie::Input* a_input);
	void ShootWhiteBall(aie::Input* a_input);
	bool IsWhiteBallFirstShot = false;
	bool HasBallsStopped = false;

	void AddBallsToBallList(Sphere* a_ball);
	std::vector<Sphere*> m_ListOfBalls;

	void AddPocketsToPocketList(Sphere* a_pocket);
	std::vector<Sphere*> m_ListOfPockets;

	void AddTexturesToList(aie::Texture* a_texture);
	std::vector<aie::Texture*> m_ListOfTextures;

	std::vector<glm::vec2> m_listOfStartPos;
	void AddStartPosToList(glm::vec2 a_pos);

	void AddBoxBoardersToList(Box* a_box);
	std::vector<Box*> m_listOfBoxes;

	bool IsBallVelocity0();

	void Createballs();

	void CreatePockets();

	void CreateBoxBoarders();

	void CreateTextures();

	void DrawBallTextures();

	void ResetBallPosToStartPos();

	aie::Texture* m_tableTexture = nullptr;

public:
	//void DrawRect();
	//void SphereAndPlane();
	//void SpringTest(int a_amount);
	//void TriggerTest();

};