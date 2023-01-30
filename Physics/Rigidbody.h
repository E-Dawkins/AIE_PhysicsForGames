﻿#pragma once
#include "PhysicsObject.h"

#include <glm/glm.hpp>

class Rigidbody : public PhysicsObject
{
public:
    Rigidbody(ShapeType _shapeId, glm::vec2 _position,
        glm::vec2 _velocity, float _orientation, float _mass);
    virtual ~Rigidbody() = default;

    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep) override;
    void ApplyForce(glm::vec2 _force);
    void ApplyForceToActor(Rigidbody* _other, glm::vec2 _force);

    // Getters
    glm::vec2 GetPosition() const
    { return m_position; }

    glm::vec2 GetVelocity() const
    { return m_velocity; }

    float GetMass() const
    { return m_mass; }

    float GetOrientation() const
    { return m_orientation; }

    glm::vec4 GetColor() const
    { return m_color; }
    
    // Setters
    void SetPosition(const glm::vec2 _position)
        { m_position = _position; }

    void SetVelocity(const glm::vec2 _velocity)
        { m_velocity = _velocity; }

    void SetMass(const float _mass)
        { m_mass = _mass; }

    void SetOrientation(const float _orientation)
        { m_orientation = _orientation; }

    void SetColor(const glm::vec4 _color)
        { m_color = _color; }
    
protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    float m_orientation;

    glm::vec4 m_color;
};
