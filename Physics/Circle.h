#pragma once
#include "Rigidbody.h"

class Circle : public Rigidbody
{
public:
    Circle();
    Circle(glm::vec2 _position, glm::vec2 _velocity,
            float _mass, float _radius, glm::vec4 _color);
    ~Circle() override;

    void Draw(float _alpha) override;

    // Getter
    float GetRadius() const { return m_radius; }
    glm::vec4 GetColor() const { return m_color; }
    
    // Setter
    void SetRadius(const float _radius) { m_radius = _radius; }
    void SetColor(const glm::vec4 _color) { m_color = _color; }
    
protected:
    float m_radius;
    glm::vec4 m_color;
};
