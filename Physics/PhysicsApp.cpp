#include "PhysicsApp.h"

#include <glm/ext.hpp>

#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Demos.h"

// Physics Scenes
#include "NewtonsCradle.h"
#include "NewtonsLaws.h"
#include "PhysicsScene.h"
#include "Simulations.h"
#include "Collisions.h"
#include "PoolTable.h"

PhysicsApp::PhysicsApp() = default;
PhysicsApp::~PhysicsApp() = default;

bool PhysicsApp::startup() {
	srand((unsigned)time(nullptr));
	
	// increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();
	
	m_font = new aie::Font("./font/consolas.ttf", 32);

	#if defined NewtonsFirstLaw
		m_physicsScene = new NewtonsLaws();
	#elif defined NewtonsSecondLaw
		m_physicsScene = new NewtonsLaws();
	#elif defined NewtonsThirdLaw
		m_physicsScene = new NewtonsLaws();
	#elif defined SimulatingCollisions
		m_physicsScene = new Simulations();
	#elif defined SimulatingRockets
		m_physicsScene = new Simulations();
	#elif defined NewtonCradle
		m_physicsScene = new NewtonsCradle();
	#elif defined CircleToPlane
		m_physicsScene = new Collisions();
	#elif defined FallingOnPlane
		m_physicsScene = new Collisions();
	#elif defined BounceToStop
		m_physicsScene = new Collisions();
	#elif defined BouncePads
		m_physicsScene = new Collisions();
	#elif defined PhysicsBucket
		m_physicsScene = new Collisions();
	#elif defined TestPoolTable
		m_physicsScene = new PoolTable();
	#endif
	
	m_physicsScene->Startup();
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

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void PhysicsApp::draw() {

	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	m_physicsScene->Draw();
	
	// draw your stuff here!
	static float aspectRatio = 16.0f / 9.0f;
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / aspectRatio, 100 / aspectRatio, -1.f, 1.f));

	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}