﻿#pragma once
#include <glm/glm.hpp>

class Rigidbody;

enum ShapeType
{
    PLANE,
    CIRCLE,
    BOX,
    SHAPE_COUNT
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
    virtual void ResolveCollision(Rigidbody* _other, glm::vec2 _contact, glm::vec2* _collisionNormal = nullptr) {}

    // Diagnostics
    virtual float GetKineticEnergy() { return 0; }
    virtual float GetPotentialEnergy() { return 0; }
    virtual float GetEnergy() { return 0; }

    // Getter
    ShapeType GetShapeId() const { return m_shapeId; }

protected:
    ShapeType m_shapeId;
};
