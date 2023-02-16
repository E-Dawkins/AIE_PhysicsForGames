#pragma once

#include "Application.h"
#include "PoolGame.h"
#include "Renderer2D.h"

class Application2D : public aie::Application {
public:

	Application2D();
	virtual ~Application2D();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:

	aie::Renderer2D*	m_2dRenderer;
	PoolGame*			m_poolGame;
	float				m_aspectRatio = 16.f / 9.f;
};