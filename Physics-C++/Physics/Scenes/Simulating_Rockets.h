#pragma once
#include "../PhysicsScene.h"

class Simulating_Rockets : public PhysicsScene
{
public:
    void Startup(aie::Application* _app) override;
    void Update(float _dt) override;
};
