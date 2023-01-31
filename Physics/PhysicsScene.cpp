#include "PhysicsScene.h"

#include "Circle.h"
#include "PhysicsObject.h"
#include "Plane.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
    PhysicsScene::Plane2Plane, PhysicsScene::Plane2Circle,
    PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle,
};

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

        if(m_doCollisions)
        {
            // check for collisions
            int actorCount = m_actors.size();
            
            // check for collisions against all objects except this one
            for (int outer = 0; outer < actorCount - 1; outer++)
            {
                for (int inner = outer + 1; inner < actorCount; inner++)
                {
                    PhysicsObject* object1 = m_actors[outer];
                    PhysicsObject* object2 = m_actors[inner];

                    int SHAPE_COUNT = 2;

                    int functionIdx = (object1->GetShapeId() * SHAPE_COUNT) + object2->GetShapeId();
                    fn collisionFunctionPtr = collisionFunctionArray[functionIdx];

                    if (collisionFunctionPtr != nullptr)
                    {
                        collisionFunctionPtr(object1, object2);
                    }
                    
                    Circle2Circle(object1, object2);
                }
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

    // Both objects are of type circle
    if (circle1 != nullptr && circle2 != nullptr)
    {
        float dist = distance(circle1->GetPosition(), circle2->GetPosition());

        // If the distance between the two spheres is less
        // than the radius sum, they are colliding
        if (dist < circle1->GetRadius() + circle2->GetRadius())
        {
            circle1->SetVelocity(glm::vec2(0));
            circle2->SetVelocity(glm::vec2(0));
            
            return true;
        }
    }

    return false;
}

bool PhysicsScene::Plane2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    Circle* circle = dynamic_cast<Circle*>(_obj1);
    Plane* plane = dynamic_cast<Plane*>(_obj2);

    // Both objects are of type plane
    if (circle != nullptr && plane != nullptr)
    {
        // Dot product represents which side of the plane the sphere is on
        // i.e. -1 is below, 1 is above and 0 is on the surface
        float sphereToPlane = dot(circle->GetPosition(), plane->GetNormal()) - plane->GetDistance();

        float intersection = circle->GetRadius() - sphereToPlane;
        float velocityOutOfPlane = dot(circle->GetVelocity(), plane->GetNormal());

        // Colliding if the sphere is intersecting the plane and
        // the velocity of the sphere is towards the plane's surface
        if (intersection > 0 && velocityOutOfPlane < 0)
        {
            circle->SetVelocity(glm::vec2(0));
            
            return true;
        }
    }
    return false;
}

bool PhysicsScene::Plane2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    return Circle2Plane(_obj2, _obj1);
}
