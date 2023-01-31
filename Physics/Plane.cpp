#include "Plane.h"

#include <Gizmos.h>

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
    float lineSegmentLength = 300;
    glm::vec2 centerPoint = m_normal * m_distToOrigin;
    
    // easy to rotate normal through 90 degrees around z
    glm::vec2 parallel(m_normal.y, -m_normal.x);

    // Color fade is a zero-alpha copy of m_color
    glm::vec4 colourFade = m_color;
    colourFade.a = 0;
    
    glm::vec2 start = centerPoint + (parallel * lineSegmentLength);
    glm::vec2 end = centerPoint - (parallel * lineSegmentLength);
    
    //aie::Gizmos::add2DLine(start, end, colour);
    aie::Gizmos::add2DTri(start, end, start - m_normal*10.0f, m_color, m_color, colourFade);
    aie::Gizmos::add2DTri(end, end - m_normal * 10.0f, start - m_normal * 10.0f, m_color, colourFade, colourFade);
}
