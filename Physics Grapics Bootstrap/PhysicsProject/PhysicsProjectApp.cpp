#include "PhysicsProjectApp.h"
#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "glm\ext.hpp"
#include <Gizmos.h>
#include "Sphere.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"


PhysicsProjectApp::PhysicsProjectApp()
{

}

PhysicsProjectApp::~PhysicsProjectApp()
{

}

bool PhysicsProjectApp::startup()
{
	//increases the 2D line count to maximise the number of objects we an draw
	aie::Gizmos::create(255U, 255U, 65535U, 65535U);

	CreateTextures();

	m_tableTexture = new aie::Texture("./textures/table.png");

	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("./font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();

	m_physicsScene->SetGravity(glm::vec2(0, 0));


	//lower the value, the more accurate the simulation will be
	// but it will increase the processing time required.
	//If it is to high it causes the sim to stutter and reduce stability
	m_physicsScene->SetTimeStep(0.01f);


	Createballs();
	CreateBoxBoarders();
	CreatePockets();

	return true;
}

void PhysicsProjectApp::shutdown()
{
	delete m_font;
	delete m_2dRenderer;
	delete m_tableTexture;
}

void PhysicsProjectApp::update(float deltaTime)
{
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();
	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	if (IsWhiteBallFirstShot)
	{
		ChooseWhiteBallLocation(input);
	}

	if (IsBallVelocity0())
	{
		ShootWhiteBall(input);
	}

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsProjectApp::draw()
{
	// wipe the screen to the background colour
	clearScreen();
	setBackgroundColour(.5f, .8f, 0.f, 1.f);

	// begin drawing sprites
	m_2dRenderer->begin();

	// If X-axis = -100 to 100, Y-axis = -56.25 to 56.25
	aie::Gizmos::draw2D((float)getWindowWidth(), (float)getWindowHeight());//glm::ortho<float>(-m_extents, m_extents, -m_extents / m_aspectRatio, m_extents / m_aspectRatio, -1.0f, 1.0f));

	// draw your stuff here!
	m_2dRenderer->drawSprite(m_tableTexture, (float)getWindowWidth() / 2.f, (float)getWindowHeight() / 2.f, (float)getWindowWidth(), (float)getWindowHeight(), 0.f);

	DrawBallTextures();

	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	char fps[32];
	sprintf_s(fps, 32, "FPS: %i", getFPS());
	m_2dRenderer->drawText(m_font, fps, 0, 720 - 32);
	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsProjectApp::ChooseWhiteBallLocation(aie::Input* a_input)
{
	if (a_input->isMouseButtonUp(0))
	{
		worldPos.x = a_input->getMouseX();
		worldPos.y = a_input->getMouseY();
		m_ListOfBalls[0]->SetPosition(glm::vec2(333, worldPos.y));
		if (m_ListOfBalls[0]->GetPosition().y >= 620)
			m_ListOfBalls[0]->SetPosition(glm::vec2(333, 620));
		else if (m_ListOfBalls[0]->GetPosition().y <= 80)
			m_ListOfBalls[0]->SetPosition(glm::vec2(333, 80));
	}
}

void PhysicsProjectApp::ShootWhiteBall(aie::Input* a_input)
{
	worldPos.x = a_input->getMouseX();
	worldPos.y = a_input->getMouseY();
	glm::vec2 dist = m_ListOfBalls[0]->GetPosition() - worldPos;
	if (glm::length(dist) > 200)
	{
		dist = glm::normalize(dist) * 200.0f;
	}

	if (a_input->isMouseButtonDown(0))
	{
		IsWhiteBallFirstShot = false;
		aie::Gizmos::add2DLine(m_ListOfBalls[0]->GetPosition(), m_ListOfBalls[0]->GetPosition() - dist, glm::vec4(1, 1, 1, 1));
	}
	if (a_input->wasMouseButtonReleased(0))
	{
		m_ListOfBalls[0]->ApplyForce(dist * extraForce, glm::vec2(0));
		hasABallBeenSunk = false;
	}
}

void PhysicsProjectApp::AddBallsToBallList(Sphere* a_ball)
{
	m_ListOfBalls.push_back(a_ball);
}

void PhysicsProjectApp::AddPocketsToPocketList(Sphere* a_pocket)
{
	m_ListOfPockets.push_back(a_pocket);
}

void PhysicsProjectApp::AddTexturesToList(aie::Texture* a_texture)
{
	m_ListOfTextures.push_back(a_texture);
}

void PhysicsProjectApp::AddStartPosToList(glm::vec2 a_pos)
{
	m_listOfStartPos.push_back(a_pos);
}

void PhysicsProjectApp::AddBoxBoardersToList(Box* a_box)
{
	m_listOfBoxes.push_back(a_box);
}

bool PhysicsProjectApp::IsBallVelocity0()
{
	for (auto pBall : m_ListOfBalls)
	{
		if (glm::sqrt(pBall->GetVelocity().x * pBall->GetVelocity().x + pBall->GetVelocity().y * pBall->GetVelocity().y) <= 10)
		{
			pBall->SetVelocity(glm::vec2(0));
		}
		else
		{
			return false;
		}
	}
	if (!hasABallBeenSunk)
	{
		ResetBallPosToStartPos();
	}
	return true;
}

void PhysicsProjectApp::Createballs()
{
	Sphere* whiteBall = new Sphere(glm::vec2(333, 353), glm::vec2(0, 0), 7.0f, 20, glm::vec4(1, 1, 1, 1));
	m_physicsScene->AddActor(whiteBall);

	//frontRow
	Sphere* ball1 = new Sphere(glm::vec2(900, 353), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball1);

	//secondRow
	Sphere* ball2 = new Sphere(glm::vec2(935, 373), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball2);
	Sphere* ball3 = new Sphere(glm::vec2(935, 330), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball3);

	//thirdRow
	Sphere* ball4 = new Sphere(glm::vec2(970, 393), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball4);
	Sphere* ball5 = new Sphere(glm::vec2(970, 350), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball5);
	Sphere* ball6 = new Sphere(glm::vec2(970, 307), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball6);

	//fourthRow
	Sphere* ball7 = new Sphere(glm::vec2(1005, 413), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball7);
	Sphere* ball8 = new Sphere(glm::vec2(1005, 370), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball8);
	Sphere* ball9 = new Sphere(glm::vec2(1005, 327), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball9);
	Sphere* ball10 = new Sphere(glm::vec2(1005, 284), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball10);

	//fifthRow
	Sphere* ball11 = new Sphere(glm::vec2(1040, 433), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball11);
	Sphere* ball12 = new Sphere(glm::vec2(1040, 390), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball12);
	Sphere* ball13 = new Sphere(glm::vec2(1040, 347), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball13);
	Sphere* ball14 = new Sphere(glm::vec2(1040, 304), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball14);
	Sphere* ball15 = new Sphere(glm::vec2(1040, 261), glm::vec2(0, 0), 3.0f, 20, glm::vec4(0, 1, 1, 1));
	m_physicsScene->AddActor(ball15);

	AddBallsToBallList(whiteBall);
	AddBallsToBallList(ball1);
	AddBallsToBallList(ball2);
	AddBallsToBallList(ball3);
	AddBallsToBallList(ball4);
	AddBallsToBallList(ball5);
	AddBallsToBallList(ball6);
	AddBallsToBallList(ball7);
	AddBallsToBallList(ball8);
	AddBallsToBallList(ball9);
	AddBallsToBallList(ball10);
	AddBallsToBallList(ball11);
	AddBallsToBallList(ball12);
	AddBallsToBallList(ball13);
	AddBallsToBallList(ball14);
	AddBallsToBallList(ball15);

	AddStartPosToList(whiteBall->GetPosition());
	AddStartPosToList(ball1->GetPosition());
	AddStartPosToList(ball2->GetPosition());
	AddStartPosToList(ball3->GetPosition());
	AddStartPosToList(ball4->GetPosition());
	AddStartPosToList(ball5->GetPosition());
	AddStartPosToList(ball6->GetPosition());
	AddStartPosToList(ball7->GetPosition());
	AddStartPosToList(ball8->GetPosition());
	AddStartPosToList(ball9->GetPosition());
	AddStartPosToList(ball10->GetPosition());
	AddStartPosToList(ball11->GetPosition());
	AddStartPosToList(ball12->GetPosition());
	AddStartPosToList(ball13->GetPosition());
	AddStartPosToList(ball14->GetPosition());
	AddStartPosToList(ball15->GetPosition());
}

void PhysicsProjectApp::CreatePockets()
{
	Sphere* pocket1 = new Sphere(glm::vec2(645, 35), glm::vec2(0, 0), 10, 45, glm::vec4(0, 0, 0, 1));
	Sphere* pocket2 = new Sphere(glm::vec2(645, 685), glm::vec2(0, 0), 10, 45, glm::vec4(0, 0, 0, 1));
	Sphere* pocket3 = new Sphere(glm::vec2(55, 670), glm::vec2(0, 0), 10, 55, glm::vec4(0, 0, 0, 1));
	Sphere* pocket4 = new Sphere(glm::vec2(1230, 670), glm::vec2(0, 0), 10, 55, glm::vec4(0, 0, 0, 1));
	Sphere* pocket5 = new Sphere(glm::vec2(1230, 55), glm::vec2(0, 0), 10, 55, glm::vec4(0, 0, 0, 1));
	Sphere* pocket6 = new Sphere(glm::vec2(55, 55), glm::vec2(0, 0), 10, 55, glm::vec4(0, 0, 0, 1));

	AddPocketsToPocketList(pocket1);
	AddPocketsToPocketList(pocket2);
	AddPocketsToPocketList(pocket3);
	AddPocketsToPocketList(pocket4);
	AddPocketsToPocketList(pocket5);
	AddPocketsToPocketList(pocket6);

	for (auto pocket : m_ListOfPockets)
	{
		pocket->SetKinematic(true);
		pocket->SetTrigger(true);
		m_physicsScene->AddActor(pocket);
		pocket->m_triggerEnter = [=](PhysicsObject* other)
		{
			if (other == m_ListOfBalls[0])
			{
				m_ListOfBalls[0]->SetVelocity(glm::vec2(0, 0));
				IsWhiteBallFirstShot = true;
			}
			else
			{
				dynamic_cast<RigidBody*>(other)->SetPosition(glm::vec2(ballCollectedXPos, 700));
				ballCollectedXPos += 45;
				other->SetKinematic(true);
				hasABallBeenSunk = true;
			}
		};
	}



}

void PhysicsProjectApp::CreateBoxBoarders()
{
	//Bottom pool table border
	Box* box1 = new Box(glm::vec2(360, 40), glm::vec2(0, 0), 0, 10, 240, 40, glm::vec4(0.6, .3, .3, 1));
	m_physicsScene->AddActor(box1);
	box1->SetKinematic(true);
	Box* box2 = new Box(glm::vec2(930, 40), glm::vec2(0, 0), 0, 10, 240, 40, glm::vec4(0.6, .3, .3, 1));
	m_physicsScene->AddActor(box2);
	box2->SetKinematic(true);
	//Top pool table border
	Box* box3 = new Box(glm::vec2(360, 680), glm::vec2(0, 0), 0, 10, 240, 40, glm::vec4(0.6, .3, .3, 1));
	m_physicsScene->AddActor(box3);
	box3->SetKinematic(true);
	Box* box4 = new Box(glm::vec2(930, 680), glm::vec2(0, 0), 0, 10, 240, 40, glm::vec4(0.6, .3, .3, 1));
	m_physicsScene->AddActor(box4);
	box4->SetKinematic(true);
	//Left pool table border
	Box* box5 = new Box(glm::vec2(40, 365), glm::vec2(0, 0), 0, 10, 40, 240, glm::vec4(0.6, .3, .3, 1));
	m_physicsScene->AddActor(box5);
	box5->SetKinematic(true);
	//Right pool table border
	Box* box6 = new Box(glm::vec2(1240, 365), glm::vec2(0, 0), 0, 10, 40, 240, glm::vec4(0.6, .3, .3, 1));
	m_physicsScene->AddActor(box6);
	box6->SetKinematic(true);

	AddBoxBoardersToList(box1);
	AddBoxBoardersToList(box2);
	AddBoxBoardersToList(box3);
	AddBoxBoardersToList(box4);
	AddBoxBoardersToList(box5);
	AddBoxBoardersToList(box6);
}

void PhysicsProjectApp::CreateTextures()
{
	aie::Texture* m_whiteBallTexture = new aie::Texture("./textures/whiteBall.png");
	aie::Texture* m_ball1Texture = new aie::Texture("./textures/ball1.png");
	aie::Texture* m_ball2Texture = new aie::Texture("./textures/ball2.png");
	aie::Texture* m_ball3Texture = new aie::Texture("./textures/ball3.png");
	aie::Texture* m_ball4Texture = new aie::Texture("./textures/ball4.png");
	aie::Texture* m_ball5Texture = new aie::Texture("./textures/ball5.png");
	aie::Texture* m_ball6Texture = new aie::Texture("./textures/ball6.png");
	aie::Texture* m_ball7Texture = new aie::Texture("./textures/ball7.png");
	aie::Texture* m_ball8Texture = new aie::Texture("./textures/ball8.png");
	aie::Texture* m_ball9Texture = new aie::Texture("./textures/ball9.png");
	aie::Texture* m_ball10Texture = new aie::Texture("./textures/ball10.png");
	aie::Texture* m_ball11Texture = new aie::Texture("./textures/ball11.png");
	aie::Texture* m_ball12Texture = new aie::Texture("./textures/ball12.png");
	aie::Texture* m_ball13Texture = new aie::Texture("./textures/ball13.png");
	aie::Texture* m_ball14Texture = new aie::Texture("./textures/ball14.png");
	aie::Texture* m_ball15Texture = new aie::Texture("./textures/ball15.png");

	AddTexturesToList(m_whiteBallTexture);
	AddTexturesToList(m_ball1Texture);
	AddTexturesToList(m_ball2Texture);
	AddTexturesToList(m_ball3Texture);
	AddTexturesToList(m_ball4Texture);
	AddTexturesToList(m_ball5Texture);
	AddTexturesToList(m_ball6Texture);
	AddTexturesToList(m_ball7Texture);
	AddTexturesToList(m_ball8Texture);
	AddTexturesToList(m_ball9Texture);
	AddTexturesToList(m_ball10Texture);
	AddTexturesToList(m_ball11Texture);
	AddTexturesToList(m_ball12Texture);
	AddTexturesToList(m_ball13Texture);
	AddTexturesToList(m_ball14Texture);
	AddTexturesToList(m_ball15Texture);
}

void PhysicsProjectApp::DrawBallTextures()
{
	for (int i = 0; i < m_ListOfBalls.size(); i++)
	{
		m_2dRenderer->drawSprite(m_ListOfTextures[i], m_ListOfBalls[i]->GetPosition().x, m_ListOfBalls[i]->GetPosition().y, m_ListOfBalls[i]->GetRadius() * 2, m_ListOfBalls[i]->GetRadius() * 2);
	}
}

void PhysicsProjectApp::ResetBallPosToStartPos()
{
	for (int i = 0; i < m_ListOfBalls.size(); i++)
	{
		m_ListOfBalls[i]->SetPosition(m_listOfStartPos[i]);
		m_ListOfBalls[i]->SetVelocity(glm::vec2(0, 0));
		m_ListOfBalls[i]->SetAngularVelocity(0);
		m_ListOfBalls[i]->SetKinematic(false);
		m_ListOfBalls[i]->SetRotation(0);
	}
}

//glm::vec2 PhysicsProjectApp::ScreenToWorld(glm::vec2 a_screenPos)
//{
//	glm::vec2 worldPos = a_screenPos;
//
//	// We will move the center of the screen to (0,0)
//	worldPos.x = getWindowWidth() / 2;
//	worldPos.y = getWindowHeight() / 2;
//
//	return worldPos;
//}

/*
void PhysicsProjectApp::DrawRect()
{
	m_physicsScene->AddActor(new Sphere(glm::vec2(20, 10), glm::vec2(-10, -17), 1, 3, glm::vec4(1, 0, 0, 1)));
	m_physicsScene->AddActor(new Plane(glm::vec2(-0.65, 0.75), -25));

	Box* box1 = new Box(glm::vec2(-20, 0), glm::vec2(16, -4), 1, 4, 8, 4, glm::vec4(1, 0, 0, 1));
	box1->SetRotation(0.5f);
	m_physicsScene->AddActor(box1);

	Box* box2 = new Box(glm::vec2(10, 0), glm::vec2(-4, 0), 1, 4, 8, 4, glm::vec4(0, 1, 0, 1));
	box2->SetRotation(0.5f);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);

	box1->ApplyForce(glm::vec2(30, 0), glm::vec2(0));
	box2->ApplyForce(glm::vec2(-15, 0), glm::vec2(0));

	Sphere* ball = new Sphere(glm::vec2(5, -10), glm::vec2(0), 1, 3, glm::vec4(0, 0, 1, 1));
	ball->SetRotation(0.5f);
	m_physicsScene->AddActor(ball);
	ball->SetKinematic(true);
}

void PhysicsProjectApp::SphereAndPlane()
{
	Sphere* ball1;
	ball1 = new Sphere(glm::vec2(-30, 10), glm::vec2(0), 3.0f, 4, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball1);

	Sphere* ball2;
	ball2 = new Sphere(glm::vec2(30, 10), glm::vec2(0), 3.0f, 4, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball2);


	ball1->ApplyForce(glm::vec2(30, 0), glm::vec2(0));
	ball2->ApplyForce(glm::vec2(-30, 0), glm::vec2(0));

	Plane* plane = new Plane();
	m_physicsScene->AddActor(plane);
}

void PhysicsProjectApp::SpringTest(int a_amount)
{
	Sphere* prev = nullptr;
	for (int i = 0; i < a_amount; i++)
	{
		//This will need to spawn the sphere at the bottom of the previous one, to make a pendulum that is effected by gravity
		Sphere* sphere = new Sphere(glm::vec2(i * 3, 30 - i * 5), glm::vec2(0), 10, 2, glm::vec4(0, 0, 1, 1));
		if (i == 0)
			sphere->SetKinematic(true);
		m_physicsScene->AddActor(sphere);

		if (prev)
			m_physicsScene->AddActor(new Spring(sphere, prev, 10, 500));
		prev = sphere;
	}
	Box* box = new Box(glm::vec2(0, -20), glm::vec2(0), 0.3f, 20, 8, 2);
	box->SetKinematic(true);
	m_physicsScene->AddActor(box);
}

void PhysicsProjectApp::TriggerTest()
{
	Sphere* ball1 = new Sphere(glm::vec2(10, 20), glm::vec2(0), 4, 4, glm::vec4(1, 0, 0, 1));
	Sphere* ball2 = new Sphere(glm::vec2(10, -5), glm::vec2(0), 4, 4, glm::vec4(0, 0.5, 0.5, 1));
	ball2->SetKinematic(true);
	ball2->SetTrigger(true);

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(new Plane(glm::vec2(0, 1), -30));
	m_physicsScene->AddActor(new Plane(glm::vec2(1, 0), -50));
	m_physicsScene->AddActor(new Plane(glm::vec2(-1, 0), -50));
	m_physicsScene->AddActor(new Box(glm::vec2(20, 10), glm::vec2(3, 0), 0.5, 4, 8, 4));
	m_physicsScene->AddActor(new Box(glm::vec2(-40, 10), glm::vec2(3, 0), 0.5, 4, 8, 4));

	ball2->m_triggerEnter = [=](PhysicsObject* other) {ball2->SetPosition(glm::vec2(-90, 50)); };
	ball2->m_triggerExit = [=](PhysicsObject* other) {std::cout << "EXITED: " << other << std::endl; };
}

*/


