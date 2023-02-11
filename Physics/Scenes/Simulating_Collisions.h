#pragma once
#include "../PhysicsScene.h"

class Simulating_Collisions : public PhysicsScene
{
public:
    void Startup(aie::Application* _app) override;
};
