﻿#pragma once
#include "PhysicsObject.h"

#include <glm/glm.hpp>

class Rigidbody : public PhysicsObject
{
public:
    Rigidbody(ShapeType _shapeId, glm::vec2 _position,
        glm::vec2 _velocity, float _orientation, float _mass);
    ~Rigidbody() override = default;

    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep) override;

    void ApplyForce(glm::vec2 _force, glm::vec2 _pos);
    void ResolveCollision(Rigidbody* _other, glm::vec2 _contact, glm::vec2* _collisionNormal = nullptr) override;
    void CalculateSmoothedPosition(float _alpha);
    
    // Diagnostics
    float GetKineticEnergy() override;
    float GetPotentialEnergy() override;
    float GetEnergy() override;

    // Getters
    glm::vec2 GetPosition() const   { return m_position; }
    glm::vec2 GetVelocity() const   { return m_velocity; }
    float GetMass()         const   { return m_mass; }
    float GetOrientation()  const   { return m_orientation; }
    float GetMoment() const         { return m_moment; }
    
    // Setters
    void SetPosition(const glm::vec2 _position)     { m_position = _position; }
    void SetVelocity(const glm::vec2 _velocity)     { m_velocity = _velocity; }
    void SetMass(const float _mass)                 { m_mass = _mass; }
    void SetOrientation(const float _orientation)   { m_orientation = _orientation; }
    
protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    
    float m_orientation;
    float m_lastOrientation;
    float m_angularVelocity;
    float m_moment;

    glm::vec2 m_lastPosition;
    glm::vec2 m_smoothedPosition;
    glm::vec2 m_smoothedLocalX;
    glm::vec2 m_smoothedLocalY;
};
