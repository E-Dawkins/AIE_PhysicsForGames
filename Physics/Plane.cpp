#include "Plane.h"

#include <Gizmos.h>
#include <iostream>

#include "Rigidbody.h"
#include "PhysicsScene.h"

Plane::Plane() : PhysicsObject(PLANE)
{
    m_distToOrigin = 0;
    m_normal = glm::vec2(0, 1);
    m_color = glm::vec4(1);
    m_elasticity = 1;
}

Plane::Plane(const glm::vec2 _normal, const float _distance,
    const glm::vec4 _color, const float _elasticity): PhysicsObject(PLANE)
{
    m_normal = _normal;
    m_distToOrigin = _distance;
    m_color = _color;
    m_elasticity = _elasticity;
}

void Plane::Draw(float _alpha)
{
    m_color.a = _alpha;

    const float lineSegmentLength = 300;
    const glm::vec2 centerPoint = m_normal * m_distToOrigin;
    
    // Parallel is just the normal rotated 90 degrees through
    // its' z-axis to be in line (parallel) with the plane
    const glm::vec2 parallel = glm::vec2(m_normal.y, -m_normal.x);

    // Color fade is a zero-alpha copy of m_color
    glm::vec4 colourFade = m_color;
    colourFade.a = 0;

    const glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
    const glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
    
    aie::Gizmos::add2DTri(start, end, start - m_normal * 10.0f, m_color, m_color, colourFade);
    aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_color, colourFade, colourFade);
}

void Plane::ResolveCollision(Rigidbody* _other, glm::vec2 _contact, glm::vec2* _collisionNormal, float _pen)
{
    // The position at which we will apply the force
    // relative to the object's center of mass
    glm::vec2 localContact = _contact - _other->GetPosition();

    // The plane has no velocity, so the relative velocity
    // is just the other object's velocity at contact point
    glm::vec2 relativeVel = _other->GetVelocity() + _other->GetAngularVelocity()
                                * glm::vec2(-localContact.y, localContact.x);
    
    float velocityIntoPlane = dot(relativeVel, m_normal);
    float elasticity = (GetElasticity() + _other->GetElasticity()) * 0.5f;

    // Perpendicular distance we apply the force to relative
    // to the object's center of mass, i.e. Torque = F * r
    float r = dot(localContact, glm::vec2(m_normal.y, -m_normal.x));

    // "Effective mass" - combination of moment of inertia and mass,
    // tells us contact points' velocity change from the applied force
    float mass0 = 1.f / (1.f / _other->GetMass() + (r * r) / _other->GetMoment());
    
    float j = -(1 + elasticity) * velocityIntoPlane * mass0;

    glm::vec2 force = m_normal * j;
    
    _other->ApplyForce(force, _contact - _other->GetPosition());

    if (_other->collisionCallback)
        _other->collisionCallback(this);

    float penetration = dot(_contact, m_normal) - m_distToOrigin;
    PhysicsScene::ApplyContactForces(_other, nullptr, m_normal, penetration);
}
