#include "Trigger_Test.h"

#include <iostream>

void Trigger_Test::Startup()
{
    SetGravity(glm::vec2(0, -10));

    Circle* ball1 = new Circle(glm::vec2(-20, 0), glm::vec2(20, 0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
    Circle* ball2 = new Circle(glm::vec2(10, -20), glm::vec2(0), 4.0f, 4, glm::vec4(0, 1, 0, 1));
    ball2->SetKinematic(true);
    ball2->SetTrigger(true);

    AddActor(ball1);
    AddActor(ball2);
    AddActor(new Plane(glm::vec2(0, 1), -30));
    AddActor(new Plane(glm::vec2(1, 0), -50));
    AddActor(new Plane(glm::vec2(-1, 0), -50));
    AddActor(new Box(glm::vec2(20, 10), glm::vec2(30, 0), glm::vec2(2), 1.f, 0));
    AddActor(new Box(glm::vec2(-40, 10), glm::vec2(30, 0), glm::vec2(2), 1.f, 45));
    
    ball2->triggerEnter = [=](PhysicsObject* other) { std::cout << "Enter:" << other->GetShapeId() << std::endl; };
    ball2->triggerExit  = [=](PhysicsObject* other) { std::cout << "Exit:"  << other->GetShapeId() << std::endl; };
}
