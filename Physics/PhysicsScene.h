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
    float GetTotalEnergy() const;

    // Getters
    static glm::vec2 GetGravity()               { return m_gravity; }
    float GetTimeStep() const                   { return m_timeStep; }
    std::vector<PhysicsObject*>* GetActors()    { return &m_actors; }
    
    // Setters
    void SetGravity(const glm::vec2 _gravity) const { m_gravity = _gravity; }
    void SetTimeStep(const float _timeStep)         { m_timeStep = _timeStep; }
    void SetDoCollisions(const bool _doCollisions)  { m_doCollisions = _doCollisions; }

    // Collision functions
    static bool Circle2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Circle2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Circle2Box(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Plane2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Plane2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Plane2Box(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Box2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Box2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2);
    static bool Box2Box(PhysicsObject* _obj1, PhysicsObject* _obj2);

protected:
    static glm::vec2 m_gravity;
    float m_timeStep;
    std::vector<PhysicsObject*> m_actors;
    bool m_doCollisions = true;
};