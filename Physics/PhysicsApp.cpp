#include "PhysicsApp.h"

#include <glm/ext.hpp>

#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Demos.h"

#include "PhysicsScene.h"

#include "Circle.h"
#include "Plane.h"

PhysicsApp::PhysicsApp() = default;
PhysicsApp::~PhysicsApp() = default;

bool PhysicsApp::startup() {
	// increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	// TODO: remember to change this when redistributing a build!
	// the following path would be used instead: "./font/consolas.ttf"
	m_font = new aie::Font("../bin/font/consolas.ttf", 32);

	m_physicsScene = new PhysicsScene();
	m_physicsScene->SetTimeStep(0.01f);

	DemoStartUp(1);
	
	return true;
}

void PhysicsApp::shutdown() {

	delete m_font;
	delete m_2dRenderer;
}

void PhysicsApp::update(float deltaTime) {

	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();

	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

	DemoUpdates(input, deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();

	// draw your stuff here!
	static float aspectRatio = 16.0f / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}

void PhysicsApp::DemoStartUp(int _num)
{
#ifdef NewtonsFirstLaw
	m_physicsScene->SetGravity(glm::vec2(0));
	
	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif
	
#ifdef NewtonsSecondLaw
	m_physicsScene->SetGravity(glm::vec2(0, -15));
	
	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif
	
#ifdef NewtonsThirdLaw
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(-4, 0), glm::vec2(0, 0), 0.1f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(4, 0), glm::vec2(0, 0), 0.1f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
#endif

#ifdef SimulatingCollisions
	m_physicsScene->SetGravity(glm::vec2(0, 0));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);

	ball1->ApplyForce(glm::vec2(30, 0));
	ball2->ApplyForce(glm::vec2(-15, 0));
#endif

#ifdef SimulatingRockets
	// m_physicsScene->SetGravity(glm::vec2(0, -10));
	m_physicsScene->SetDoCollisions(false);
	
	Circle* ball = new Circle(glm::vec2(0), glm::vec2(0), 1000.f, 5, glm::vec4(1, 0, 0, 1));
	ball->SetOrientation(90);
	m_physicsScene->AddActor(ball);
#endif

#ifdef CircleToCircle
	m_physicsScene->SetGravity(glm::vec2(0));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(10, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(0, 0), glm::vec2(-50, 0), 10.0f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
#endif

#ifdef CircleToPlane
	m_physicsScene->SetGravity(glm::vec2(0, -10));

	Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(10, 0), glm::vec2(0), 10.0f, 4, glm::vec4(0, 1, 0, 1));
	Plane* plane = new Plane(glm::vec2(0, 1), -40, glm::vec4(1, 1, 1, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(plane);
#endif
}

void PhysicsApp::DemoUpdates(aie::Input* _input, float _dt)
{
#ifdef NewtonsSecondLaw // Switches gravity to the corresponding arrow key direction
	if (_input->wasKeyPressed(aie::INPUT_KEY_LEFT))
		m_physicsScene->SetGravity(glm::vec2(-15, 0));
	
	else if (_input->wasKeyPressed(aie::INPUT_KEY_RIGHT))
		m_physicsScene->SetGravity(glm::vec2(15, 0));
	
	else if (_input->wasKeyPressed(aie::INPUT_KEY_DOWN))
		m_physicsScene->SetGravity(glm::vec2(0, -15));
	
	else if (_input->wasKeyPressed(aie::INPUT_KEY_UP))
		m_physicsScene->SetGravity(glm::vec2(0, 15));
#endif
	
#ifdef NewtonsThirdLaw // Mock 'collision' between 2 balls of different masses
	if (_input->wasKeyPressed(aie::INPUT_KEY_SPACE))
	{
		Circle* ball1 = dynamic_cast<Circle*>(m_physicsScene->GetActors()->at(0));
		Circle* ball2 = dynamic_cast<Circle*>(m_physicsScene->GetActors()->at(1));
		ball1->ApplyForceToActor(ball2, glm::vec2(2, 0));
	}
#endif

#ifdef SimulatingRockets
	static Circle* rocket = dynamic_cast<Circle*>(m_physicsScene->GetActors()->at(0));
	static float fuelStorage = rocket->GetMass() * 0.75f;
	static float rocketMass = rocket->GetMass() - fuelStorage;

	const float rads = DegreeToRadian(rocket->GetOrientation());
	const float actualFuelMass = fuelStorage >= 20.f ? 20.f : fuelStorage;
	
	const glm::vec2 forceDir = normalize(glm::vec2(cos(rads), sin(rads)));
	const glm::vec2 fuelPos = rocket->GetPosition() - forceDir * rocket->GetRadius();

	static float accumulatedTime = 0.0f;

	// Expel fuel and apply the force to the rocket to make it launch
	if (accumulatedTime >= 0.15f && fuelStorage > 0)
	{
		Circle* fuel = new Circle(fuelPos, glm::vec2(0), actualFuelMass, 0.35f, glm::vec4(0, 1, 0, 1));
		m_physicsScene->AddActor(fuel);

		fuelStorage -= actualFuelMass;
		
		rocket->ApplyForceToActor(fuel, forceDir * -30.f * actualFuelMass);
		rocket->SetMass(rocketMass + fuelStorage);

		accumulatedTime = 0.0f;
	}

	// turn the rocket
	float addOrientation = 0.f;
	const float turnSpeed = 2.5f;
	
	if (_input->isKeyDown(aie::INPUT_KEY_LEFT))
		addOrientation += turnSpeed;

	if (_input->isKeyDown(aie::INPUT_KEY_RIGHT))
		addOrientation -= turnSpeed;

	rocket->SetOrientation(rocket->GetOrientation() + addOrientation);
	
	accumulatedTime += _dt;
#endif
}

float PhysicsApp::DegreeToRadian(const float _degree) const
{
	return _degree * (Pi / 180.f);
}
