#pragma once

#include <vector>

#include "Application.h"
#include "Renderer2D.h"
#include "Input.h"

class PhysicsScene;
class PhysicsObject;

class PhysicsApp : public aie::Application {
public:

	PhysicsApp();
	~PhysicsApp() override;

	bool startup() override;
	void shutdown() override;

	void update(float deltaTime) override;
	void draw() override;

protected:

	aie::Renderer2D*			m_2dRenderer;
	aie::Font*					m_font;
	
	PhysicsScene*				m_physicsScene;
	float						m_aspectRatio = 16.f / 9.f;
};