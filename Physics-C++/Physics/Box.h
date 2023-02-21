#pragma once
#include "Rigidbody.h"

class Box : public Rigidbody
{
public:
    Box();
    Box(glm::vec2 _position, glm::vec2 _velocity, float _mass, glm::vec2 _extents,
        float _orientation, glm::vec4 _color = glm::vec4(1), float _elasticity = 1);

    void Draw(float _alpha) override;
    bool CheckBoxCorners(const Box& _box, glm::vec2& _contact,
        int& _numContacts, float& _pen, glm::vec2& _edgeNormal);

    // Getters
    glm::vec2 GetExtents() const    { return m_extents; }
    glm::vec4 GetColor() const      { return m_color; }
    float GetWidth() const          { return m_extents.x * 2; }
    float GetHeight() const         { return m_extents.y * 2; }
    
    // Setters
    void SetExtents(const glm::vec2 _extents)   { m_extents = _extents; }
    void SetColor(const glm::vec4 _color)       { m_color = _color; }

protected:
    glm::vec2 m_extents;
    glm::vec4 m_color;
};
