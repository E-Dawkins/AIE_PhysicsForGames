#pragma once
#include <functional>
#include <list>

#include "PhysicsObject.h"

#include <glm/glm.hpp>

const float MIN_LINEAR_THRESHOLD = 0.09f;
const float MIN_ANGULAR_THRESHOLD = 0.01f;

class Rigidbody : public PhysicsObject
{
public:
    Rigidbody(ShapeType _shapeId, glm::vec2 _position,
        glm::vec2 _velocity, float _orientation, float _mass);
    ~Rigidbody() override = default;

    virtual void FixedUpdate(glm::vec2 _gravity, float _timeStep) override;

    void ApplyForce(glm::vec2 _force, glm::vec2 _pos);
    void ResolveCollision(Rigidbody* _other, glm::vec2 _contact, glm::vec2* _collisionNormal = nullptr, float _pen = 0) override;
    void CalculateSmoothedPosition(float _alpha);
    void CalculateAxes();
    glm::vec2 ToWorld(glm::vec2 _point);
    glm::vec2 ToWorldSmoothed(glm::vec2 _localPos);
    
    std::function<void(PhysicsObject*)> collisionCallback;
    std::function<void(PhysicsObject*)> triggerEnter;
    std::function<void(PhysicsObject*)> triggerExit;
    void TriggerEnter(PhysicsObject* _other);
    
    // Diagnostics
    float GetKineticEnergy() override;
    float GetPotentialEnergy() override;
    float GetEnergy() override;

    // Getters
    glm::vec2 GetPosition() const       { return m_position; }
    glm::vec2 GetVelocity() const       { return m_velocity; }
    float GetMass()         const       { return m_isKinematic ? INT_MAX : m_mass; }
    float GetOrientation()  const       { return m_orientation; }
    float GetMoment() const             { return m_isKinematic ? INT_MAX : m_moment; }
    glm::vec2 GetLocalX() const         { return m_localX; }
    glm::vec2 GetLocalY() const         { return m_localY; }
    float GetAngularVelocity() const    { return m_angularVelocity; }
    float GetLinearDrag() const         { return m_linearDrag; }
    float GetAngularDrag() const        { return m_angularDrag; }
    bool IsKinematic() const            { return m_isKinematic; }
    bool IsTrigger() const              { return m_isTrigger; }
    
    // Setters
    void SetPosition(const glm::vec2 _position)     { m_position = _position; }
    void SetVelocity(const glm::vec2 _velocity)     { m_velocity = _velocity; }
    void SetMass(const float _mass)                 { m_mass = _mass; }
    void SetOrientation(const float _orientation)   { m_orientation = _orientation; }
    void SetLinearDrag(const float _linearDrag)     { m_linearDrag = _linearDrag; }
    void SetAngularDrag(const float _angularDrag)   { m_angularDrag = _angularDrag; }
    void SetKinematic(const bool _state)            { m_isKinematic = _state;}
    void SetTrigger(const bool _state)              { m_isTrigger = _state; }
    
protected:
    glm::vec2 m_position;
    glm::vec2 m_velocity;
    float m_mass;
    
    float m_orientation;
    float m_lastOrientation;
    float m_angularVelocity;
    float m_moment;

    glm::vec2 m_lastPosition;
    glm::vec2 m_smoothedPosition;
    glm::vec2 m_smoothedLocalX;
    glm::vec2 m_smoothedLocalY;

    glm::vec2 m_localX;
    glm::vec2 m_localY;

    float m_linearDrag;
    float m_angularDrag;

    bool m_isKinematic;

    bool m_isTrigger;
    std::list<PhysicsObject*> m_objectsInside;
    std::list<PhysicsObject*> m_objectsInsideThisFrame;
};
