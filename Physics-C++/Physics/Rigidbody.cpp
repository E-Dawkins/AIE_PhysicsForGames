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
    m_lastOrientation = 0;
    m_lastPosition = _position;
    m_smoothedPosition = _position;

    m_linearDrag = 0.3f;
    m_angularDrag = 0.3f;

    m_isKinematic = false;
    m_isTrigger = false;
    m_moment = 1;

    CalculateAxes();
    CalculateSmoothedPosition(1);
}

void Rigidbody::FixedUpdate(const glm::vec2 _gravity, const float _timeStep)
{
    // Store local axes
    CalculateAxes();

    // This object is a trigger
    if (m_isTrigger)
    {
        // Check that every object in the objectsInside list
        // is still inside us this frame, if not remove them
        // from the list and call triggerExit
        for (auto it = m_objectsInside.begin(); it != m_objectsInside.end(); ++it)
        {
            if (std::find(m_objectsInsideThisFrame.begin(), m_objectsInsideThisFrame.end(), *it)
                == m_objectsInsideThisFrame.end())
            {
                if (triggerExit)
                    triggerExit(*it);

                it = m_objectsInside.erase(it);

                if (it == m_objectsInside.end())
                    break;
            }
        }
    }

    // Clear the list now for next frame
    m_objectsInsideThisFrame.clear();

    // If object is kinematic don't perform any
    // force or position calculations
    if (m_isKinematic)
    {
        m_velocity = glm::vec2(0);
        m_angularVelocity = 0;
        return;
    }
    
    m_lastOrientation = m_orientation;
    m_lastPosition = m_position;

    m_velocity -= m_velocity * m_linearDrag * _timeStep;
    m_angularVelocity -= m_angularVelocity * m_angularDrag * _timeStep;

    if (length(m_velocity) < MIN_LINEAR_THRESHOLD)
        m_velocity = glm::vec2(0);

    if (abs(m_angularVelocity) < MIN_ANGULAR_THRESHOLD)
        m_angularVelocity = 0;
    
    m_position += m_velocity * _timeStep;
    ApplyForce(_gravity * GetMass() * _timeStep, glm::vec2(0));

    m_orientation += m_angularVelocity * _timeStep;
}

void Rigidbody::ApplyForce(const glm::vec2 _force, glm::vec2 _pos)
{
    m_velocity += _force / GetMass();
    m_angularVelocity += (_force.y * _pos.x - _force.x * _pos.y) / GetMoment();
}

void Rigidbody::ResolveCollision(Rigidbody* _other, glm::vec2 _contact, glm::vec2* _collisionNormal, float _pen)
{
    // Register that these two objects have overlapped this frame
    m_objectsInsideThisFrame.push_back(_other);
    _other->m_objectsInsideThisFrame.push_back(this);
    
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
        float mass1 = 1.f / (1.f / GetMass() + (r1 * r1) / GetMoment());
        float mass2 = 1.f / (1.f / _other->GetMass() + (r2 * r2) / _other->GetMoment());

        float elasticity = (GetElasticity() + _other->GetElasticity()) * 0.5f;
        
        glm::vec2 force = (1.f + elasticity) * mass1 * mass2 /
                            (mass1 + mass2) * (v1 - v2) * normal;

        // Apply forces and run callbacks, if both objects aren't triggers ...
        if (!m_isTrigger && !_other->m_isTrigger)
        {
            // Apply equal and opposite force
            ApplyForce(-force, _contact - m_position);
            _other->ApplyForce(force, _contact - _other->m_position);

            // Collision callbacks, i.e. OnCollisionEnter
            if (collisionCallback != nullptr)
                collisionCallback(_other);

            if (_other->collisionCallback != nullptr)
                _other->collisionCallback(this);
        }
        else // ... else run trigger enter functions
        {
            TriggerEnter(_other);
            _other->TriggerEnter(this);
        }

        // Objects are penetrating, so apply contact
        // forces to separate the two objects
        if (_pen > 0)
            PhysicsScene::ApplyContactForces(this, _other, normal, _pen);
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

void Rigidbody::CalculateAxes()
{
    float sn = sinf(m_orientation);
    float cs = cosf(m_orientation);

    m_localX = glm::vec2(cs, sn);
    m_localY = glm::vec2(-sn, cs);
}

glm::vec2 Rigidbody::ToWorld(glm::vec2 _localPos)
{
    return m_position + (m_localX * _localPos.x + m_localY * _localPos.y);
}

glm::vec2 Rigidbody::ToWorldSmoothed(glm::vec2 _localPos)
{
    return m_smoothedPosition + (m_smoothedLocalX * _localPos.x + m_smoothedLocalY * _localPos.y);
}

void Rigidbody::TriggerEnter(PhysicsObject* _other)
{
    if (m_isTrigger && std::find(m_objectsInside.begin(), m_objectsInside.end(), _other) == m_objectsInside.end())
    {
        m_objectsInside.push_back(_other);

        if (triggerEnter != nullptr)
            triggerEnter(_other);
    }
}

float Rigidbody::GetKineticEnergy()
{
    return 0.5f * (GetMass() * dot(m_velocity, m_velocity) +
        GetMoment() * m_angularVelocity * m_angularVelocity);
}

float Rigidbody::GetPotentialEnergy()
{
    return -GetMass() * dot(PhysicsScene::GetGravity(), GetPosition());
}

float Rigidbody::GetEnergy()
{
    return GetKineticEnergy() + GetPotentialEnergy();
}
