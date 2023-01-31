#pragma once
#include <vector>
#include <glm/vec2.hpp>

#include "PhysicsObject.h"

class PhysicsObject;

class PhysicsScene
{
public:
    PhysicsScene();
    ~PhysicsScene();

    void AddActor(PhysicsObject* _actor);
    void RemoveActor(const PhysicsObject* _actor);
    
    void Update(float _dt);
    void Draw();

    void DebugScene();
    void CheckForCollisions();

    // Getters
    glm::vec2 GetGravity() const                { return m_gravity; }
    float GetTimeStep() const                   { return m_timeStep; }
    std::vector<PhysicsObject*>* GetActors()    { return &m_actors; }
    
    // Setters
    void SetGravity(const glm::vec2 _gravity)       { m_gravity = _gravity; }
    void SetTimeStep(const float _timeStep)         { m_timeStep = _timeStep; }
    void SetDoCollisions(const bool _doCollisions)  { m_doCollisions = _doCollisions; }

    // Collision functions
    static bool Circle2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Plane2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Circle2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Plane2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);

private:
    glm::vec2 m_gravity;
    float m_timeStep;
    std::vector<PhysicsObject*> m_actors;
    bool m_doCollisions = true;
};
