#include "Box.h"

#include <Gizmos.h>

Box::Box(): Rigidbody(BOX, glm::vec2(0), glm::vec2(0), 0, 0)
{
    m_extents = glm::vec2(0);
    m_color = glm::vec4(0);
    m_moment = 1.f / 12.f * m_mass * (m_extents.x * 2.f) * (m_extents.y * 2.f);
}

Box::Box(glm::vec2 _position, glm::vec2 _velocity, glm::vec2 _extents,
    float _mass, float _orientation, glm::vec4 _color):
    Rigidbody(BOX, _position, _velocity, _orientation, _mass)
{
    m_extents = _extents;
    m_color = _color;
    m_moment = 1.f / 12.f * m_mass * (m_extents.x * 2.f) * (m_extents.y * 2.f);
}

void Box::Draw(float _alpha)
{
    CalculateSmoothedPosition(_alpha);

    glm::vec2 p1 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
                    - m_smoothedLocalY * m_extents.y;
    glm::vec2 p2 = m_smoothedPosition + m_smoothedLocalX * m_extents.x
                    - m_smoothedLocalY * m_extents.y;
    glm::vec2 p3 = m_smoothedPosition - m_smoothedLocalX * m_extents.x
                    + m_smoothedLocalY * m_extents.y;
    glm::vec2 p4 = m_smoothedPosition + m_smoothedLocalX * m_extents.x
                    + m_smoothedLocalY * m_extents.y;

    aie::Gizmos::add2DTri(p1, p2, p4, m_color);
    aie::Gizmos::add2DTri(p1, p4, p3, m_color);
}

bool Box::CheckBoxCorners(const Box& _box, glm::vec2& _contact, int& _numContacts, float& _pen, glm::vec2& _edgeNormal)
{
    float minX, maxX, minY, maxY;
    float boxW = _box.GetWidth();
    float boxH = _box.GetHeight();
    int numLocalContacts = 0;
    glm::vec2 localContact = glm::vec2(0);
    bool first = true;

    // Loop over all corners of the other box
    for (float x = -_box.GetExtents().x; x < boxW; x += boxW)
    {
        for (float y = -_box.GetExtents().y; y < boxH; y += boxH)
        {
            // Get point position in world space
            glm::vec2 p = _box.GetPosition() + x * _box.m_localX + y * _box.m_localY;

            // Get position in this box's space
            glm::vec2 p0 = glm::vec2(dot(p - m_position, m_localX),
                                        dot(p - m_position, m_localY));

            // Update extents in each cardinal direction in
            // this box's space, i.e. along the separating axes
            if (first || p0.x < minX) minX = p0.x;
            if (first || p0.x > maxX) maxX = p0.x;
            if (first || p0.y < minY) minY = p0.y;
            if (first || p0.y > maxY) maxY = p0.y;

            // If this corner is inside the box,
            // add it the the list of contact points
            if (p0.x >= -m_extents.x && p0.x <= m_extents.x &&
                p0.y >= -m_extents.y && p0.y <= m_extents.y)
            {
                numLocalContacts++;
                localContact += p0;
            }

            first = false;
        }
    }

    // If we lie entirely to one side of the box along one
    // axis, we've found a separating axis and can exit
    if (maxX <= -m_extents.x || minX >= m_extents.x ||
        maxY <= -m_extents.y || minY >= m_extents.y)
            return false;

    // No contact point, exit
    if (numLocalContacts == 0)
        return false;

    bool res = false;
    _contact += m_position + (localContact.x * m_localX + localContact.y * m_localY) /
                    (float)numLocalContacts;
    _numContacts++;

    // Find the minimum penetration vector as a penetration amount and normal
    float pen0 = m_extents.x - minX;
    if (pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _edgeNormal = m_localX;
        _pen = pen0;
        res = true;
    }

    pen0 = maxX + m_extents.x;
    if (pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _edgeNormal = -m_localX;
        _pen = pen0;
        res = true;
    }

    pen0 = m_extents.y - minY;
    if (pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _edgeNormal = m_localY;
        _pen = pen0;
        res = true;
    }

    pen0 = maxY + m_extents.y;
    if (pen0 > 0 && (pen0 < _pen || _pen == 0))
    {
        _edgeNormal = -m_localY;
        _pen = pen0;
        res = true;
    }

    return res;
}
