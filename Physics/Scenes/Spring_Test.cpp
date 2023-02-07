#include "Spring_Test.h"

void Spring_Test::Startup()
{
    SetGravity(glm::vec2(0, -15));
    
    MakeRope(10, glm::vec2(0, 30), glm::vec2(5, 0), true, true);
    MakeRope(13, glm::vec2(30, 40), glm::vec2(-5, 0), true);

    // A box for the rope to dangle over
    Box* box = new Box(glm::vec2(0, -20), glm::vec2(0), glm::vec2(10.f), 1.f, 20);
    box->SetKinematic(true);
    AddActor(box);

    // A box falling onto the rope
    Box* box1 = new Box(glm::vec2(25, 100), glm::vec2(0), glm::vec2(5), 10.f, 0);
    AddActor(box1);

    Box* box2 = new Box(glm::vec2(30, 80), glm::vec2(0), glm::vec2(5), 10.f, 0);
    AddActor(box2);

    Box* box3 = new Box(glm::vec2(10, 150), glm::vec2(0), glm::vec2(5), 10.f, 0);
    AddActor(box3);
}

void Spring_Test::MakeRope(const int _ropeSegments, const glm::vec2 _ropeStart,
    const glm::vec2 _segmentDiff, const bool _anchorFirst, const bool _anchorLast)
{
    Circle* prev = nullptr;

    for (int i = 0; i < _ropeSegments; i++)
    {
        const glm::vec2 circlePos = _ropeStart + _segmentDiff * (float)i;
        
        Circle* circle = new Circle(circlePos, glm::vec2(0), 10, 2.f);
        circle->SetKinematic((i == 0 && _anchorFirst) ||
                            (i == _ropeSegments - 1 && _anchorLast));

        circle->SetElasticity(0.2f);

        AddActor(circle);

        if (prev)
            AddActor(new Spring(circle, prev, 250, 10));

        prev = circle;
    }
}
