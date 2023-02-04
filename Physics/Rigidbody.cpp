#include "Rigidbody.h"

#include "PhysicsScene.h"

Rigidbody::Rigidbody(const ShapeType _shapeId, const glm::vec2 _position, const glm::vec2 _velocity, const float _orientation, const float _mass):
    PhysicsObject(_shapeId)
{
    m_position = _position;
    m_velocity = _velocity;
    m_orientation = _orientation;
    m_mass = _mass;

    m_angularVelocity = 0;
    m_moment = 0;
    m_lastOrientation = 0;
    m_lastPosition = _position;
    m_smoothedPosition = _position;
}

void Rigidbody::FixedUpdate(const glm::vec2 _gravity, const float _timeStep)
{
    m_lastOrientation = m_orientation;
    m_lastPosition = m_position;
    
    m_position += m_velocity * _timeStep;
    ApplyForce(_gravity * m_mass * _timeStep, glm::vec2(0));

    m_orientation += m_angularVelocity * _timeStep;
}

void Rigidbody::ApplyForce(const glm::vec2 _force, glm::vec2 _pos)
{
    m_velocity += _force / GetMass();
    m_angularVelocity += (_force.y * _pos.x - _force.x * _pos.y) / GetMoment();
}

void Rigidbody::ResolveCollision(Rigidbody* _other, glm::vec2 _contact, glm::vec2* _collisionNormal)
{
    // Calculate vector between the two centres, or use the provided normal
    glm::vec2 normal = normalize(_collisionNormal ? *_collisionNormal :
                                    _other->m_position - m_position);
    // Vector perpendicular to the normal
    glm::vec2 perp = glm::vec2(normal.y, -normal.x);

    // Determine total velocity of the contact points, both linear and rotational
    // 'r' is the radius from axis to application of force
    float r1 = dot(_contact - m_position, -perp);
    float r2 = dot(_contact - _other->m_position, perp);

    // Velocity of contact point on this object
    float v1 = dot(m_velocity, normal) - r1 * m_angularVelocity;
    // Velocity of contact point on other object
    float v2 = dot(_other->m_velocity, normal) + r2 * _other->m_angularVelocity;

    if (v1 > v2) // moving closer to each other
    {
        // Effective mass at the contact point for each object
        // i.e. how far the point will move due to the applied force
        float mass1 = 1.f / (1.f / m_mass + (r1 * r1) / m_moment);
        float mass2 = 1.f / (1.f / _other->m_mass + (r2 * r2) / _other->m_moment);

        float elasticity = 1;

        glm::vec2 force = (1.f + elasticity) * mass1 * mass2 /
                            (mass1 + mass2) * (v1 - v2) * normal;

        // Apply equal and opposite force
        ApplyForce(-force, _contact - m_position);
        _other->ApplyForce(force, _contact - _other->m_position);
    }
}

void Rigidbody::CalculateSmoothedPosition(float _alpha)
{
    m_smoothedPosition = _alpha * m_position + (1 - _alpha) * m_lastPosition;
    float smoothedOrientation = _alpha * m_orientation + (1 - _alpha) * m_lastOrientation;
    
    float sn = sinf(smoothedOrientation);
    float cs = cosf(smoothedOrientation);
    
    m_smoothedLocalX = glm::vec2(cs, sn);
    m_smoothedLocalY = glm::vec2(-sn, cs);
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
