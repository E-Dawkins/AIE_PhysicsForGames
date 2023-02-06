#pragma once
#include "PhysicsScene.h"

class NewtonsLaws : public PhysicsScene
{
public:
    void Startup() override;
    void Update(float _dt) override;
};
