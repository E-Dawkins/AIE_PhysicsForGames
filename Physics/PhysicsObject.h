#pragma once
#include <glm/vec2.hpp>

enum ShapeType
{
    PLANE = 0,
    CIRCLE = 1,
    BOX = 2
};

class PhysicsObject
{
protected:
    PhysicsObject(const ShapeType _shapeId) : m_shapeId(_shapeId) {}

public:
    virtual ~PhysicsObject() = default;
    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep) = 0;
    virtual void Draw(float _alpha) = 0;
    virtual void ResetPosition() {}

    // Getter
    ShapeType GetShapeId() const
        { return m_shapeId; }

protected:
    ShapeType m_shapeId;
};
