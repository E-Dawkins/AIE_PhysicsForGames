#pragma once
#include "../PhysicsScene.h"

class Spring_Test : public PhysicsScene
{
public:
    void Startup(aie::Application* _app) override;

    void MakeRope(int _ropeSegments, glm::vec2 _ropeStart,
        glm::vec2 _segmentDiff, bool _anchorFirst, bool _anchorLast = false);
};
