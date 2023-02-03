#include "Rigidbody.h"

#include <iostream>

#include "PhysicsScene.h"

Rigidbody::Rigidbody(const ShapeType _shapeId, const glm::vec2 _position, const glm::vec2 _velocity, const float _orientation, const float _mass):
    PhysicsObject(_shapeId)
{
    m_position = _position;
    m_velocity = _velocity;
    m_orientation = _orientation;
    m_mass = _mass;
}

void Rigidbody::FixedUpdate(const glm::vec2 _gravity, const float _timeStep)
{
    m_position += m_velocity * _timeStep;
    ApplyForce(_gravity * m_mass * _timeStep);
}

void Rigidbody::ApplyForce(const glm::vec2 _force)
{
    m_velocity += _force / m_mass;
}

void Rigidbody::ApplyForceToActor(Rigidbody* _other, glm::vec2 _force)
{
    _other->ApplyForce(_force);
    ApplyForce(-_force);
}

void Rigidbody::ResolveCollision(Rigidbody* _other)
{
    glm::vec2 normal = normalize(_other->GetPosition() - m_position);
    glm::vec2 relativeVelocity = _other->GetVelocity() - m_velocity;

    // if the objects are already moving apart, we don't need to do anything
    if (dot(normal, relativeVelocity) >= 0)
        return;

    float elasticity = 1;
    float j = dot(-(1 + elasticity) * relativeVelocity, normal) /
        ((1 / m_mass) + (1 / _other->GetMass()));

    glm::vec2 force = normal * j;

    float kePre = GetKineticEnergy() + _other->GetKineticEnergy();

    ApplyForceToActor(_other, force);

    float kePost = GetKineticEnergy() + _other->GetKineticEnergy();

    float deltaKE = kePost - kePre;
    if (deltaKE > kePost * 0.01f)
        std::cout << "Kinetic energy discrepancy greater than 1% detected!!";
}

float Rigidbody::GetKineticEnergy()
{
    return m_mass * length(m_velocity) * length(m_velocity) * 0.5f;
}

float Rigidbody::GetPotentialEnergy()
{
    return -GetMass() * dot(PhysicsScene::GetGravity(), GetPosition());
}

float Rigidbody::GetEnergy()
{
    return GetKineticEnergy() + GetPotentialEnergy();
}
