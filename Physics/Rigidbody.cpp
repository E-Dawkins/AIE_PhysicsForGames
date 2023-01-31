#include "Rigidbody.h"

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
