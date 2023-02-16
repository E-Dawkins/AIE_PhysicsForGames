#pragma once
#include <Renderer2D.h>
#include <vector>
#include <glm/vec2.hpp>

#include "PhysicsObject.h"
#include "Rigidbody.h"
#include "Demos.h"
#include "Application.h"

#include "Circle.h"
#include "Plane.h"
#include "Box.h"
#include "Spring.h"
#include "SoftBody.h"

using std::vector;

class PhysicsScene
{
public:
    PhysicsScene();
    virtual ~PhysicsScene();

    void AddActor(PhysicsObject* _actor);
    void RemoveActor(const PhysicsObject* _actor);

    virtual void Startup(aie::Application* _app) {}
    virtual void Update(float _dt);
    virtual void Draw();

    void CheckForCollision();
    float GetTotalEnergy() const;
    float DegreeToRadian(float _degree) const;
    glm::vec2 PixelToViewSpacePos(glm::vec2 _pixelPos) const;
    glm::vec2 ViewToPixelSpacePos(glm::vec2 _viewPos) const;

    // Getters
    static glm::vec2 GetGravity()               { return m_gravity; }
    float GetTimeStep() const                   { return m_timeStep; }
    std::vector<PhysicsObject*>* GetActors()    { return &m_actors; }
    
    // Setters
    void SetGravity(const glm::vec2 _gravity) const { m_gravity = _gravity; }
    void SetTimeStep(const float _timeStep)         { m_timeStep = _timeStep; }
    void SetWindowExtents(const glm::vec2 _size)    { m_windowExtents = _size; }
    void SetWindowPixelSize(const glm::vec2 _size)  { m_windowPixelSize = _size; }
    void SetRenderer(aie::Renderer2D*& _renderer)   { m_renderer2D = _renderer; }
    
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

    static void ApplyContactForces(Rigidbody* _body1, Rigidbody* _body2, glm::vec2 _norm, float _pen);

protected:
    static glm::vec2 m_gravity;
    float m_timeStep;
    vector<PhysicsObject*> m_actors;
    
    glm::vec2 m_windowExtents;
    glm::vec2 m_windowPixelSize;
    aie::Renderer2D* m_renderer2D = nullptr;
};