#include "Application2D.h"

#include <glm/ext.hpp>

#include "Input.h"
#include "Gizmos.h"

Application2D::Application2D()  = default;
Application2D::~Application2D() = default;

bool Application2D::startup() 
{
	srand((unsigned)time(nullptr));
	
	// increase the 2D line count to maximise the objects we can draw
	aie::Gizmos::create(225U, 225U, 65535U, 65535U);
	
	m_2dRenderer = new aie::Renderer2D();

	m_poolGame = new MainScene();

	m_poolGame->SetWindowExtents(glm::vec2(100, 100 / m_aspectRatio));
	m_poolGame->SetWindowPixelSize(glm::vec2(getWindowWidth(), getWindowHeight()));
	m_poolGame->SetRenderer(m_2dRenderer);
	
	m_poolGame->Startup(this);

	return true;
}

void Application2D::shutdown() 
{
	delete m_2dRenderer;
	delete m_poolGame;
}

void Application2D::update(float deltaTime) 
{
	// input example
	aie::Input* input = aie::Input::getInstance();

	aie::Gizmos::clear();
	
	m_poolGame->Update(deltaTime);

	// exit the application
	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();
}

void Application2D::draw() 
{
	// wipe the screen to the background colour
	clearScreen();

	// begin drawing sprites
	m_2dRenderer->begin();
	
	// done drawing sprites
	m_2dRenderer->end();
	
	m_poolGame->Draw();

	aie::Gizmos::draw2D(glm::ortho<float>(-100, 100,
		-100 / m_aspectRatio, 100 / m_aspectRatio, -1.f, 1.f));
}