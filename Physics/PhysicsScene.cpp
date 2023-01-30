#include "PhysicsScene.h"

#include "Circle.h"
#include "PhysicsObject.h"

PhysicsScene::PhysicsScene()
{
    m_timeStep = 0.01f;
    m_gravity = glm::vec2(0);
}

PhysicsScene::~PhysicsScene()
{
    for (auto pActor : m_actors)
    {
        delete pActor;
    }
}

void PhysicsScene::AddActor(PhysicsObject* _actor)
{
    m_actors.push_back(_actor);
}

void PhysicsScene::RemoveActor(const PhysicsObject* _actor)
{
    const std::vector<PhysicsObject*>::iterator iter =
        std::find(m_actors.begin(), m_actors.end(), _actor);
    
    if (*iter != nullptr)
        m_actors.erase(iter);
}

void PhysicsScene::Update(float _dt)
{
    static float accumulatedTime = 0.0f;
    accumulatedTime += _dt;

    while (accumulatedTime >= m_timeStep)
    {
        for (auto pActor : m_actors)
        {
            pActor->FixedUpdate(m_gravity, m_timeStep);
        }

        accumulatedTime -= m_timeStep;

        // check for collisions
        int actorCount = m_actors.size();

        // check for collisions against all objects except this one
        for (int outer = 0; outer < actorCount - 1; outer++)
        {
            for (int inner = outer + 1; inner < actorCount; inner++)
            {
                PhysicsObject* object1 = m_actors[outer];
                PhysicsObject* object2 = m_actors[inner];

                Circle2Circle(object1, object2);
            }
        }
    }
}

void PhysicsScene::Draw()
{
    for (auto pActor : m_actors)
    {
        pActor->Draw(1);
    }
}

bool PhysicsScene::Circle2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    Circle* circle1 = dynamic_cast<Circle*>(_obj1);
    Circle* circle2 = dynamic_cast<Circle*>(_obj2);

    // both objects are of type circle
    if (circle1 != nullptr && circle2 != nullptr)
    {
        float dist = distance(circle1->GetPosition(), circle2->GetPosition());

        return dist < (circle1->GetRadius() + circle2->GetRadius());
    }

    return false;
}
