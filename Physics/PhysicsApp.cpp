#include "PhysicsApp.h"

#include <glm/ext.hpp>

#include "Texture.h"
#include "Font.h"
#include "Input.h"
#include "Gizmos.h"
#include "Demos.h"
#include "PhysicsScene.h"

// Physics Scenes
#include "Scenes/Newtons_First_Law.h"
#include "Scenes/Newtons_Second_Law.h"
#include "Scenes/Newtons_Third_Law.h"
#include "Scenes/Simulating_Collisions.h"
#include "Scenes/Simulating_Rockets.h"
#include "Scenes/Newtons_Cradle.h"
#include "Scenes/Circle_To_Plane.h"
#include "Scenes/Falling_On_Plane.h"
#include "Scenes/Bounce_To_Stop.h"
#include "Scenes/Bounce_Pads.h"
#include "Scenes/Physics_Bucket.h"
#include "Scenes/Pool_Table.h"
#include "Scenes/Spring_Test.h"
#include "Scenes/SoftBody_Test.h"
#include "Scenes/Trigger_Test.h"

PhysicsApp::PhysicsApp() = default;
PhysicsApp::~PhysicsApp() = default;

bool PhysicsApp::startup() {
	srand((unsigned)time(nullptr));
	
	// increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();
	
	m_font = new aie::Font("./font/consolas.ttf", 32);

	#if defined NewtonsFirstLaw
		m_physicsScene = new Newtons_First_Law();
	#elif defined NewtonsSecondLaw
		m_physicsScene = new Newtons_Second_Law();
	#elif defined NewtonsThirdLaw
		m_physicsScene = new Newtons_Third_Law();
	#elif defined SimulatingCollisions
		m_physicsScene = new Simulating_Collisions();
	#elif defined SimulatingRockets
		m_physicsScene = new Simulating_Rockets();
	#elif defined NewtonCradle
		m_physicsScene = new Newtons_Cradle();
	#elif defined CircleToPlane
		m_physicsScene = new Circle_To_Plane();
	#elif defined FallingOnPlane
		m_physicsScene = new Falling_On_Plane();
	#elif defined BounceToStop
		m_physicsScene = new Bounce_To_Stop();
	#elif defined BouncePads
		m_physicsScene = new Bounce_Pads();
	#elif defined PhysicsBucket
		m_physicsScene = new Physics_Bucket();
	#elif defined PoolTable
		m_physicsScene = new Pool_Table();
	#elif defined SpringTest
		m_physicsScene = new Spring_Test();
	#elif defined SoftbodyTest
		m_physicsScene = new SoftBody_Test();
	#elif defined TriggerTest
		m_physicsScene = new Trigger_Test();
	#endif

	m_physicsScene->SetWindowExtents(glm::vec2(100, 100 / m_aspectRatio));
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
	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / m_aspectRatio, 100 / m_aspectRatio, -1.f, 1.f));
	
	// output some text, uses the last used colour
	m_2dRenderer->drawText(m_font, "Press ESC to quit", 0, 0);

	// done drawing sprites
	m_2dRenderer->end();
}