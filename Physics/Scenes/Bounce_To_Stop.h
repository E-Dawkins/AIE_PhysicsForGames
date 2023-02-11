#pragma once
#include "../PhysicsScene.h"

class Bounce_To_Stop : public PhysicsScene
{
public:
    void Startup(aie::Application* _app) override;
};
