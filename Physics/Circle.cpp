#include "Circle.h"

#include <Gizmos.h>

Circle::Circle() : Rigidbody(CIRCLE, glm::vec2(0), glm::vec2(0), 0, 0)
{
    m_radius = 0;
    m_color = glm::vec4(0);
    m_moment = 0.5f * GetMass() * m_radius * m_radius;
    m_elasticity = 1;
}

Circle::Circle(glm::vec2 _position, glm::vec2 _velocity,
    float _mass, float _radius, glm::vec4 _color, float _elasticity):
    Rigidbody(CIRCLE, _position, _velocity, 0, _mass)
{
    m_radius = _radius;
    m_color = _color;
    m_moment = 0.5f * GetMass() * m_radius * m_radius;
    m_elasticity = _elasticity;
}

Circle::~Circle() = default;

void Circle::Draw(float _alpha)
{
    CalculateSmoothedPosition(_alpha);
    
    aie::Gizmos::add2DCircle(m_smoothedPosition, m_radius, 12, m_color);
    
    // aie::Gizmos::add2DLine(m_smoothedPosition, m_smoothedPosition
    //                         + m_smoothedLocalX * m_radius, glm::vec4(1));
}
