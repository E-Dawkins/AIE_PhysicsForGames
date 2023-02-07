#pragma once
#include "../PhysicsScene.h"

class Pool_Table : public PhysicsScene
{
public:
    void Startup() override;
    void Update(float _dt) override;
    void Draw() override;

protected:
    Circle* m_cueBall = nullptr;
};
