#include "PhysicsScene.h"

typedef bool(*fn)(PhysicsObject*, PhysicsObject*);

static fn collisionFunctionArray[] =
{
    PhysicsScene::Plane2Plane, PhysicsScene::Plane2Circle, PhysicsScene::Plane2Box,
    PhysicsScene::Circle2Plane, PhysicsScene::Circle2Circle, PhysicsScene::Circle2Box,
    PhysicsScene::Box2Plane, PhysicsScene::Box2Circle, PhysicsScene::Box2Box,
};

glm::vec2 PhysicsScene::m_gravity = glm::vec2(0);

PhysicsScene::PhysicsScene()
{
    m_timeStep = 0.01f;
    m_windowExtents = glm::vec2(100, 56.25);
    m_windowPixelSize = glm::vec2(1280, 720);
    m_gravity = glm::vec2(0);
}

PhysicsScene::~PhysicsScene()
{
    for (auto pActor : m_actors)
    {
        delete pActor;
    }

    m_actors.clear();
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

        CheckForCollision();
    }
}

void PhysicsScene::Draw()
{
    for (auto pActor : m_actors)
    {
        pActor->Draw(1);
    }
}

void PhysicsScene::CheckForCollision()
{
    // check for collisions against all objects except this one
    for (int outer = 0; outer < (int)m_actors.size() - 1; outer++)
    {
        for (int inner = outer + 1; inner < (int)m_actors.size(); inner++)
        {
            PhysicsObject* object1 = m_actors[outer];
            PhysicsObject* object2 = m_actors[inner];

            // Ensures no joints are included in collision checks
            if (object1->GetShapeId() < 0 || object2->GetShapeId() < 0)
                continue;
                    
            int functionIdx = (object1->GetShapeId() * SHAPE_COUNT) + object2->GetShapeId();
            fn collisionFunctionPtr = collisionFunctionArray[functionIdx];

            if (collisionFunctionPtr != nullptr)
            {
                collisionFunctionPtr(object1, object2);
            }
        }
    }
}

float PhysicsScene::GetTotalEnergy() const
{
    float total = 0;

    for (const auto obj : m_actors)
    {
        total += obj->GetEnergy();
    }

    return total;
}

float PhysicsScene::DegreeToRadian(const float _degree) const
{
    return _degree * (Pi / 180.f);
}

glm::vec2 PhysicsScene::PixelToViewSpace(glm::vec2 _pixelPos) const
{
    const glm::vec2 newPos = glm::vec2((m_windowExtents.x * 2.f) * (_pixelPos.x / m_windowPixelSize.x),
                                       (m_windowExtents.y * 2.f) * (_pixelPos.y / m_windowPixelSize.y));
    return newPos - m_windowExtents;
}

bool PhysicsScene::Circle2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    Circle* circle1 = dynamic_cast<Circle*>(_obj1);
    Circle* circle2 = dynamic_cast<Circle*>(_obj2);

    // Both objects are of type circle
    if (circle1 != nullptr && circle2 != nullptr)
    {
        float dist = distance(circle1->GetPosition(), circle2->GetPosition());

        // If the radius sum minus the distance between the circles
        // is positive, then the circles are penetrating each other
        float penetration = circle1->GetRadius() + circle2->GetRadius() - dist;

        if (penetration > 0)
        {
            glm::vec2 contact = 0.5f * (circle1->GetPosition() + circle2->GetPosition());
            circle1->ResolveCollision(circle2, contact, nullptr, penetration);
            return true;
        }
    }

    return false;
}

bool PhysicsScene::Circle2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    Circle* circle = dynamic_cast<Circle*>(_obj1);
    Plane* plane = dynamic_cast<Plane*>(_obj2);

    // Objects are of type circle and plane
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
            glm::vec2 contact = circle->GetPosition() + (plane->GetNormal() * -circle->GetRadius());
            plane->ResolveCollision(circle, contact);
            
            return true;
        }
    }
    
    return false;
}

bool PhysicsScene::Circle2Box(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    // Pass the switched objects to Box2Circle function
    return Box2Circle(_obj2, _obj1);
}

bool PhysicsScene::Plane2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    // Pass the switched objects to Circle2Plane function
    return Circle2Plane(_obj2, _obj1);
}

bool PhysicsScene::Plane2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    // Planes are static, so they should never collide
    return false;
}

bool PhysicsScene::Plane2Box(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    Plane* plane = dynamic_cast<Plane*>(_obj1);
    Box* box = dynamic_cast<Box*>(_obj2);

    // Objects are of type plane and box
    if (plane != nullptr && box != nullptr)
    {
        int numContacts = 0;
        glm::vec2 contact = glm::vec2(0);
        float contactV = 0;

        // Get a reference point on the plane
        glm::vec2 planeOrigin = plane->GetNormal() * plane->GetDistance();

        // Check all 4 corners to see if we have hit the plane
        for (float x = -box->GetExtents().x; x < box->GetWidth(); x += box->GetWidth())
        {
            for (float y = -box->GetExtents().y; y < box->GetHeight(); y += box->GetHeight())
            {
                // Gets position of the corner in world space
                glm::vec2 p = box->GetPosition() + x * box->GetLocalX() + y * box->GetLocalY();
                float distFromPlane = dot(p - planeOrigin, plane->GetNormal());

                // Total velocity of point in world space
                glm::vec2 displacement = x * box->GetLocalX() + y * box->GetLocalY();
                glm::vec2 pointVelocity = box->GetVelocity() + box->GetAngularVelocity()
                                            * glm::vec2(-displacement.y, displacement.x);

                // Velocity of point towards plane
                float velocityToPlane = dot(pointVelocity, plane->GetNormal());

                // Colliding if the box is below the plane, and the
                // velocity of the box is going towards the plane
                if (distFromPlane < 0 && velocityToPlane <= 0)
                {
                    numContacts++;
                    contact += p;
                    contactV += velocityToPlane;
                }
            }
        }

        // At least 1 corner of the box has collided with the plane
        if (numContacts > 0)
        {
            plane->ResolveCollision(box, contact / (float)numContacts);
            return true;
        }
    }

    return false;
}

bool PhysicsScene::Box2Circle(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    Box* box = dynamic_cast<Box*>(_obj1);
    Circle* circle = dynamic_cast<Circle*>(_obj2);

    // Objects are of type box and circle
    if (box != nullptr && circle != nullptr)
    {
        // Transform circle into box's coordinate space
        glm::vec2 circlePosWorld = circle->GetPosition() - box->GetPosition();
        glm::vec2 circlePosBox = glm::vec2(dot(circlePosWorld, box->GetLocalX()),
                                            dot(circlePosWorld, box->GetLocalY()));

        // Find the closest point to the circle's center on the box,
        // by clamping the box-space coordinates to the box's extents
        glm::vec2 closestPointOnBoxBox = circlePosBox;
        glm::vec2 extents = box->GetExtents();

        if (closestPointOnBoxBox.x < -extents.x) closestPointOnBoxBox.x = -extents.x;
        if (closestPointOnBoxBox.x > extents.x) closestPointOnBoxBox.x = extents.x;
        if (closestPointOnBoxBox.y < -extents.y) closestPointOnBoxBox.y = -extents.y;
        if (closestPointOnBoxBox.y > extents.y) closestPointOnBoxBox.y = extents.y;

        // Convert back to world-space coordinates
        glm::vec2 closestPointOnBoxWorld = box->GetPosition() + closestPointOnBoxBox.x
                            * box->GetLocalX() + closestPointOnBoxBox.y * box->GetLocalY();
        glm::vec2 circleToBox = circle->GetPosition() - closestPointOnBoxWorld;

        float penetration = circle->GetRadius() - length(circleToBox);
        
        // If the closest point to the box is less than
        // the circle's radius, we have collided
        if (penetration > 0)
        {
            glm::vec2 direction = normalize(circleToBox);
            glm::vec2 contact = closestPointOnBoxWorld;
            box->ResolveCollision(circle, contact, &direction, penetration);
            return true;
        }
    }
    
    return false;
}

bool PhysicsScene::Box2Plane(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    // Pass the switched objects to Plane2Box function
    return Plane2Box(_obj2, _obj1);
}

bool PhysicsScene::Box2Box(PhysicsObject* _obj1, PhysicsObject* _obj2)
{
    Box* box1 = dynamic_cast<Box*>(_obj1);
    Box* box2 = dynamic_cast<Box*>(_obj2);

    // Both objects are of type box
    if (box1 != nullptr && box2 != nullptr)
    {
        glm::vec2 normal = glm::vec2(0);
        glm::vec2 contact = glm::vec2(0);
        float pen = 0;
        int numContacts = 0;

        box1->CheckBoxCorners(*box2, contact, numContacts, pen, normal);

        if (box2->CheckBoxCorners(*box1, contact, numContacts, pen, normal))
        {
            normal = -normal;
        }

        if (pen > 0)
        {
            box1->ResolveCollision(box2, contact / (float)numContacts, &normal, pen);
        }

        return true;
    }
    
    return false;
}

void PhysicsScene::ApplyContactForces(Rigidbody* _body1, Rigidbody* _body2, glm::vec2 _norm, float _pen)
{
    if ((_body1 && _body1->IsTrigger()) ||
        (_body2 && _body2->IsTrigger()))
        return;
    
    float body2Mass = _body2 ? _body2->GetMass() : INT_MAX;

    float body1Factor = body2Mass / (_body1->GetMass() + body2Mass);

    _body1->SetPosition(_body1->GetPosition() - body1Factor * _norm * _pen);

    if (_body2)
        _body2->SetPosition(_body2->GetPosition() + (1 - body1Factor) * _norm * _pen);
}