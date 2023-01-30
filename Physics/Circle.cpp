#include "Circle.h"

#include <Gizmos.h>

Circle::Circle() : Rigidbody(CIRCLE, glm::vec2(0), glm::vec2(0), 0, 0)
{
    m_radius = 0;
}

Circle::Circle(glm::vec2 _position, glm::vec2 _velocity, float _mass, float _radius, glm::vec4 _color):
    Rigidbody(CIRCLE, _position, _velocity, 0, _mass)
{
    m_radius = _radius;
    m_color = _color;
}

Circle::~Circle() = default;

void Circle::Draw(float _alpha)
{
    aie::Gizmos::add2DCircle(m_position, m_radius, 12, m_color);
}
