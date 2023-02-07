#pragma once
#include "../PhysicsScene.h"

class Simulating_Rockets : public PhysicsScene
{
public:
    void Startup() override;
    void Update(float _dt) override;
};
