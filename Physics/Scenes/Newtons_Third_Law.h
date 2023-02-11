#pragma once
#include "../PhysicsScene.h"

class Newtons_Third_Law : public PhysicsScene
{
public:
    void Startup(aie::Application* _app) override;
    void Update(float _dt) override;
};
