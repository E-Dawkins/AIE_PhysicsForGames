#pragma once
#include "PhysicsObject.h"

#include <glm/glm.hpp>

class Plane : public PhysicsObject
{
public:
    Plane();
    Plane(glm::vec2 _normal, float _distance, glm::vec4 _color = glm::vec4(1), float _elasticity = 1);
    ~Plane() override = default;

    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep) override {}
    virtual void Draw(float _alpha) override;

    void ResolveCollision(Rigidbody* _other, glm::vec2 _contact, glm::vec2* _collisionNormal = nullptr, float _pen = 0) override;

    // Getters
    glm::vec2 GetNormal() const { return m_normal; }
    float GetDistance() const   { return m_distToOrigin; }
    glm::vec4 GetColor() const  { return m_color; }

    // Setters
    void SetColor(const glm::vec4 _color) { m_color = _color; }

protected:
    glm::vec2 m_normal;
    float m_distToOrigin;
    glm::vec4 m_color;
};
