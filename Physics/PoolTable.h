#pragma once
#include "PhysicsScene.h"

class PoolTable : public PhysicsScene
{
public:
    void Startup() override;
    void Update(float _dt) override;
    void Draw() override;
};
