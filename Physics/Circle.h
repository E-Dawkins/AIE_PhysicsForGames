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
    float GetRadius() const
        { return m_radius; }
    
    // Setter
    void SetRadius(const float _radius)
        { m_radius = _radius; }
    
protected:
    float m_radius;
};
