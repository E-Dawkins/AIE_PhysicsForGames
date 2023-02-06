#pragma once
#include "PhysicsScene.h"

class Simulations : public PhysicsScene
{
public:
    void Startup() override;
    void Update(float _dt) override;
};
