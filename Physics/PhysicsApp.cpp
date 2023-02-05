#include "PhysicsApp.h"

#include <glm/ext.hpp>

#include "Box.h"
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

	DemoUpdates(input, deltaTime);
	DemoDraw();
	
	m_physicsScene->Update(deltaTime);
	m_physicsScene->Draw();

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
	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif
	
#ifdef NewtonsSecondLaw
	m_physicsScene->SetGravity(glm::vec2(0, -15));
	
	Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
	m_physicsScene->AddActor(ball);
#endif
	
#ifdef NewtonsThirdLaw
	Circle* ball1 = new Circle(glm::vec2(-4, 0), glm::vec2(0, 0), 0.1f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(4, 0), glm::vec2(0, 0), 0.1f, 4, glm::vec4(0, 1, 0, 1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
#endif

#ifdef SimulatingCollisions
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

#ifdef CircleToPlane
	m_physicsScene->SetGravity(glm::vec2(0, -10));

	Circle* ball1 = new Circle(glm::vec2(-40, 50), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(20, 55), glm::vec2(0), 10.0f, 6, glm::vec4(0, 1, 0, 1));
	Circle* ball3 = new Circle(glm::vec2(10, 35), glm::vec2(0), 7.0f, 5, glm::vec4(0, 0, 1, 1));
	Circle* ball4 = new Circle(glm::vec2(-15, 45), glm::vec2(0), 1.0f, 2, glm::vec4(1, 0, 1, 1));
	Plane* plane = new Plane(glm::vec2(0.866, 0.5), -20, glm::vec4(1));
	Plane* plane2 = new Plane(glm::vec2(-0.866, 0.5), -20, glm::vec4(1));

	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(ball3);
	m_physicsScene->AddActor(ball4);
	m_physicsScene->AddActor(plane);
	m_physicsScene->AddActor(plane2);
#endif
	
#ifdef NewtonsCradle1
	Circle* ball1 = new Circle(glm::vec2(20, 0), glm::vec2(0), 1.0f, 4, glm::vec4(1, 0, 0, 1));
	Circle* ball2 = new Circle(glm::vec2(-20, 0), glm::vec2(20, 0), 1.0f, 4, glm::vec4(0, 1, 0, 1));

	Plane* plane = new Plane(glm::vec2(1, 0), -40, glm::vec4(1));
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -40, glm::vec4(1));
	
	m_physicsScene->AddActor(ball1);
	m_physicsScene->AddActor(ball2);
	m_physicsScene->AddActor(plane);
	m_physicsScene->AddActor(plane2);
#endif

#if defined NewtonsCradle2 || defined NewtonsCradle3
	Plane* plane = new Plane(glm::vec2(1, 0), -60, glm::vec4(1));
	Plane* plane2 = new Plane(glm::vec2(-1, 0), -60, glm::vec4(1));
	
	Circle* circle = new Circle(glm::vec2(-50, 0), glm::vec2(50, 0), 1.f, 4, glm::vec4(1));
	Circle* circle1 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 1.f, 4, glm::vec4(1));
	Circle* circle2 = new Circle(glm::vec2(0), glm::vec2(0), 1.f, 4, glm::vec4(1));
	Circle* circle3 = new Circle(glm::vec2(20, 0), glm::vec2(0), 1.f, 4, glm::vec4(1));
	Circle* circle4 = new Circle(glm::vec2(40, 0), glm::vec2(0), 1.f, 4, glm::vec4(1));

#if defined NewtonsCradle3
	circle1->SetMass(3.f);
	circle1->SetRadius(6);
	circle1->SetColor(glm::vec4(1, 0, 0, 1));
#endif
	
	m_physicsScene->AddActor(plane);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(circle);
	m_physicsScene->AddActor(circle1);
	m_physicsScene->AddActor(circle2);
	m_physicsScene->AddActor(circle3);
	m_physicsScene->AddActor(circle4);
#endif

#ifdef FallingOnPlane
	m_physicsScene->SetGravity(glm::vec2(0, -20));
	
	Plane* plane = new Plane(glm::vec2(0.707, 0.707), -40, glm::vec4(1));
	Plane* plane2 = new Plane(glm::vec2(-0.707, 0.707), -40, glm::vec4(1));
	Circle* circle = new Circle(glm::vec2(-50, 10), glm::vec2(0), 1.f, 4, glm::vec4(1, 0, 0, 1));
	Circle* circle1 = new Circle(glm::vec2(30, 40), glm::vec2(0), 2.f, 5, glm::vec4(1, 0, 1, 1));
	Circle* circle2 = new Circle(glm::vec2(-40, 20), glm::vec2(0), 3.f, 6, glm::vec4(0, 1, 1, 1));
	Box* box = new Box(glm::vec2(50, 10), glm::vec2(0), glm::vec2(4), 1.f, 0.f, glm::vec4(0, 0, 1, 1));
	Box* box1 = new Box(glm::vec2(20, 10), glm::vec2(0), glm::vec2(4), 1.f, 27.f, glm::vec4(0, 0, 1, 1));
	Box* box2 = new Box(glm::vec2(30, 20), glm::vec2(0), glm::vec2(4), 1.f, 54.f, glm::vec4(0, 0, 1, 1));
	Box* box3 = new Box(glm::vec2(10, 0), glm::vec2(0), glm::vec2(4), 1.f, 92.f, glm::vec4(0, 0, 1, 1));

	m_physicsScene->AddActor(plane);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(circle);
	m_physicsScene->AddActor(circle1);
	m_physicsScene->AddActor(circle2);
	m_physicsScene->AddActor(box);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(box2);
	m_physicsScene->AddActor(box3);
#endif

#ifdef BounceToStop
	m_physicsScene->SetGravity(glm::vec2(0, -10));
	
	Plane* plane = new Plane(glm::vec2(0, 1), -40, glm::vec4(1));
	Box* box = new Box(glm::vec2(50, 10), glm::vec2(0), glm::vec2(4), 1.f, 0.f, glm::vec4(0, 0, 1, 1));
	Box* box1 = new Box(glm::vec2(45, 30), glm::vec2(0), glm::vec2(4), 1.f, 37.f, glm::vec4(0, 0, 1, 1));
	Circle* circle = new Circle(glm::vec2(-50, 10), glm::vec2(0), 1.f, 4, glm::vec4(1, 0, 0, 1));

	box->SetElasticity(0.3f);
	box1->SetElasticity(0.3f);
	circle->SetElasticity(0.3f);
	
	m_physicsScene->AddActor(plane);
	m_physicsScene->AddActor(box);
	m_physicsScene->AddActor(box1);
	m_physicsScene->AddActor(circle);
#endif

#ifdef TestPoolTable
	Circle* cueBall = new Circle(glm::vec2(-60, 0), glm::vec2(0), 2.f, 3.5, glm::vec4(1));
	
	Plane* plane = new Plane(glm::vec2(0, 1), -50, glm::vec4(0, 0.75, 0, 1));
	Plane* plane1 = new Plane(glm::vec2(0, -1), -50, glm::vec4(0, 0.75, 0, 1));
	Plane* plane2 = new Plane(glm::vec2(1, 0), -95, glm::vec4(0, 0.75, 0, 1));
	Plane* plane3 = new Plane(glm::vec2(-1, 0), -95, glm::vec4(0, 0.75, 0, 1));

	// 1
	Circle* billiard = new Circle(glm::vec2(30, 0), glm::vec2(0), 3.f, 4, glm::vec4(1, 0, 0, 1));
	// 2
	Circle* billiard1 = new Circle(glm::vec2(37.5, 4.5), glm::vec2(0), 3.f, 4, glm::vec4(1, 1, 0, 1));
	Circle* billiard2 = new Circle(glm::vec2(37.5, -4.5), glm::vec2(0), 3.f, 4, glm::vec4(1, 0, 0, 1));
	// 3
	Circle* billiard3 = new Circle(glm::vec2(45, 9), glm::vec2(0), 3.f, 4, glm::vec4(1, 0, 0, 1));
	Circle* billiard4 = new Circle(glm::vec2(45, 0), glm::vec2(0), 3.f, 4, glm::vec4(0.15, 0.15, 0.15, 1));
	Circle* billiard5 = new Circle(glm::vec2(45, -9), glm::vec2(0), 3.f, 4, glm::vec4(1, 1, 0, 1));
	// 4
	Circle* billiard6 = new Circle(glm::vec2(52.5, 13.5), glm::vec2(0), 3.f, 4, glm::vec4(1 ,0, 0, 1));
	Circle* billiard7 = new Circle(glm::vec2(52.5, 4.5), glm::vec2(0), 3.f, 4, glm::vec4(1, 1, 0, 1));
	Circle* billiard8 = new Circle(glm::vec2(52.5, -4.5), glm::vec2(0), 3.f, 4, glm::vec4(1, 1, 0, 1));
	Circle* billiard9 = new Circle(glm::vec2(52.5, -13.5), glm::vec2(0), 3.f, 4, glm::vec4(1, 0, 0, 1));
	// 5
	Circle* billiard10 = new Circle(glm::vec2(60, 18), glm::vec2(0), 3.f, 4, glm::vec4(1, 1, 0, 1));
	Circle* billiard11 = new Circle(glm::vec2(60, 9), glm::vec2(0), 3.f, 4, glm::vec4(1, 0, 0, 1));
	Circle* billiard12 = new Circle(glm::vec2(60, 0), glm::vec2(0), 3.f, 4, glm::vec4(1, 1, 0, 1));
	Circle* billiard13 = new Circle(glm::vec2(60, -9), glm::vec2(0), 3.f, 4, glm::vec4(1, 0, 0, 1));
	Circle* billiard14 = new Circle(glm::vec2(60, -18), glm::vec2(0), 3.f, 4, glm::vec4(1, 1, 0, 1));
	
	m_physicsScene->AddActor(cueBall);
	
	m_physicsScene->AddActor(plane);
	m_physicsScene->AddActor(plane1);
	m_physicsScene->AddActor(plane2);
	m_physicsScene->AddActor(plane3);
	
	m_physicsScene->AddActor(billiard);
	m_physicsScene->AddActor(billiard1);
	m_physicsScene->AddActor(billiard2);
	m_physicsScene->AddActor(billiard3);
	m_physicsScene->AddActor(billiard4);
	m_physicsScene->AddActor(billiard5);
	m_physicsScene->AddActor(billiard6);
	m_physicsScene->AddActor(billiard7);
	m_physicsScene->AddActor(billiard8);
	m_physicsScene->AddActor(billiard9);
	m_physicsScene->AddActor(billiard10);
	m_physicsScene->AddActor(billiard11);
	m_physicsScene->AddActor(billiard12);
	m_physicsScene->AddActor(billiard13);
	m_physicsScene->AddActor(billiard14);
	
	for (int i = 0; i < m_physicsScene->GetActors()->size(); i++)
	{
		PhysicsObject* obj = m_physicsScene->GetActors()->at(i);
		
		if (dynamic_cast<Circle*>(obj))
			obj->SetElasticity(0.8f);

		else obj->SetElasticity(0.6f);
	}
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

#ifdef TestPoolTable
	static Circle* cueBall = dynamic_cast<Circle*>(m_physicsScene->GetActors()->at(0));
	float turnSpeed = 1.f;

	float orientation = cueBall->GetOrientation();
	
	if (_input->isKeyDown(aie::INPUT_KEY_LEFT))
		orientation += turnSpeed;

	if (_input->isKeyDown(aie::INPUT_KEY_RIGHT))
		orientation -= turnSpeed;
	
	cueBall->SetOrientation(orientation);

	float rads = DegreeToRadian(cueBall->GetOrientation());
	const glm::vec2 forceDir = normalize(glm::vec2(cos(rads), sin(rads)));

	if (_input->wasKeyPressed(aie::INPUT_KEY_SPACE) && cueBall->GetVelocity() == glm::vec2(0))
	{
		static float forceAmount = 650.f;
		cueBall->ApplyForce(forceDir * forceAmount, glm::vec2(0));
	}
#endif
}

void PhysicsApp::DemoDraw()
{
#ifdef TestPoolTable
	static Circle* cueBall = dynamic_cast<Circle*>(m_physicsScene->GetActors()->at(0));
	
	float rads = DegreeToRadian(cueBall->GetOrientation());
	const glm::vec2 forceDir = normalize(glm::vec2(cos(rads), sin(rads)));

	glm::vec4 dirColor = cueBall->GetVelocity() == glm::vec2(0)
							? glm::vec4(1, 0, 0, 1) : glm::vec4(1);

	aie::Gizmos::add2DLine(cueBall->GetPosition(), cueBall->GetPosition()
							+ forceDir * cueBall->GetRadius(), dirColor);
#endif
}

float PhysicsApp::DegreeToRadian(const float _degree) const
{
	return _degree * (Pi / 180.f);
}
