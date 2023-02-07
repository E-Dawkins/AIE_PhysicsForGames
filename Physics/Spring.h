#pragma once
#include "PhysicsObject.h"
#include "Rigidbody.h"

class Spring : public PhysicsObject
{
public:
    Spring(Rigidbody* _body1, Rigidbody* _body2, float _springCoefficient,
        float _damping, float _restLength = 0, glm::vec2 _contact1 = glm::vec2(0),
        glm::vec2 _contact2 = glm::vec2(0));
    
    void FixedUpdate(glm::vec2 _gravity, float _timeStep) override;
    void Draw(float _alpha) override;
    
    glm::vec2 GetContact1(float _alpha = 1.f);
    glm::vec2 GetContact2(float _alpha = 1.f);

protected:
    Rigidbody* m_body1;
    Rigidbody* m_body2;

    glm::vec2 m_contact1;
    glm::vec2 m_contact2;

    glm::vec4 m_color;

    float m_damping;
    float m_restLength;
    float m_springCoefficient;  // the restoring force
};
