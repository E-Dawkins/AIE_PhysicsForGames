#include "Plane.h"

#include <Gizmos.h>
#include <iostream>
#include "Rigidbody.h"

Plane::Plane() : PhysicsObject(PLANE)
{
    m_distToOrigin = 0;
    m_normal = glm::vec2(0, 1);
    m_color = glm::vec4(1);
}

Plane::Plane(const glm::vec2 _normal, const float _distance, const glm::vec4 _color): PhysicsObject(PLANE)
{
    m_normal = _normal;
    m_distToOrigin = _distance;
    m_color = _color;
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
    
    aie::Gizmos::add2DTri(start, end, start - m_normal*10.0f, m_color, m_color, colourFade);
    aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_color, colourFade, colourFade);
}

void Plane::ResolveCollision(Rigidbody* _other)
{
    // if the objects are already moving apart, we don't need to do anything
    if (dot(m_normal, _other->GetVelocity()) >= 0)
        return;

    float elasticity = 1;
    float j = dot(-(1 + elasticity) * _other->GetVelocity(), m_normal) /
        (1 / _other->GetMass());

    glm::vec2 force = m_normal * j;

    float kePre = GetKineticEnergy() + _other->GetKineticEnergy();

    _other->ApplyForce(force);

    float kePost = GetKineticEnergy() + _other->GetKineticEnergy();

    float deltaKE = kePost - kePre;
    if (deltaKE > kePost * 0.01f)
        std::cout << "Kinetic energy discrepancy greater than 1% detected!!";
}
