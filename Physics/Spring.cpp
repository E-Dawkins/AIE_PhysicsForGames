#include "Spring.h"

#include <Gizmos.h>

#include "Rigidbody.h"

Spring::Spring(Rigidbody* _body1, Rigidbody* _body2, float _springCoefficient,
    float _damping, float _restLength, glm::vec2 _contact1, glm::vec2 _contact2)
    : PhysicsObject(JOINT)
{
    m_body1 = _body1;
    m_body2 = _body2;
    m_springCoefficient = _springCoefficient;
    m_damping = _damping;
    m_restLength = _restLength;
    m_contact1 = _contact1;
    m_contact2 = _contact2;
    m_color = glm::vec4(1);
    
    if (m_restLength == 0)
    {
        if (m_body1) m_body1->CalculateAxes();
        if (m_body2) m_body2->CalculateAxes();
        m_restLength = distance(GetContact1(), GetContact2());
    }
}

void Spring::FixedUpdate(glm::vec2 _gravity, float _timeStep)
{
    // Get the world coordinates of the spring ends
    glm::vec2 p1 = GetContact1();
    glm::vec2 p2 = GetContact2();

    float length = distance(p1, p2);
    glm::vec2 direction = normalize(p2 - p1);

    // Apply damping
    glm::vec2 relativeVel = m_body2->GetVelocity() - m_body1->GetVelocity();

    // F = -kX - bv
    glm::vec2 force = direction * m_springCoefficient *
        (m_restLength - length) - m_damping * relativeVel;

    // Cap the spring force to 1000N to prevent numerical instability
    const float threshold = 1000.f;
    const float forceMag = glm::length(force);

    if (forceMag > threshold)
        force *= threshold / forceMag;

    m_body1->ApplyForce(-force * _timeStep, p1 - m_body1->GetPosition());
    m_body2->ApplyForce(force * _timeStep, p2 - m_body2->GetPosition());
}

void Spring::Draw(float _alpha)
{
    if (m_body1) m_body1->CalculateSmoothedPosition(_alpha);
    if (m_body2) m_body2->CalculateSmoothedPosition(_alpha);
    
    aie::Gizmos::add2DLine(GetContact1(), GetContact2(), m_color);
}

glm::vec2 Spring::GetContact1(float _alpha) // translates the rigidbody pos, to world-space
{
    return m_body1 ? m_body1->ToWorldSmoothed(m_contact1) : m_contact1;
}

glm::vec2 Spring::GetContact2(float _alpha) // translates the rigidbody pos, to world-space
{
    return m_body2 ? m_body2->ToWorldSmoothed(m_contact2) : m_contact2;
}
