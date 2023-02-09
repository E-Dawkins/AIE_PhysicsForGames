#include "Physics_Bucket.h"

void Physics_Bucket::Startup()
{
    SetGravity(glm::vec2(0, -15));

    Plane* plane = new Plane(glm::vec2(0.707, 0.707), -40, glm::vec4(1));
    Plane* plane1 = new Plane(glm::vec2(-0.707, 0.707), -40, glm::vec4(1));

    plane->SetElasticity(0.5f);
    plane1->SetElasticity(0.5f);
	
    Box* box = new Box(glm::vec2(0, 40), glm::vec2(0), 1.f, glm::vec2(3), 0);
    Box* box1 = new Box(glm::vec2(-10, 50), glm::vec2(0), 1.f, glm::vec2(5), 0);
    Box* box2 = new Box(glm::vec2(5, 60), glm::vec2(0), 1.f, glm::vec2(8), 0);

    box->SetLinearDrag(0.1f);
    box1->SetLinearDrag(1.f);
    box2->SetLinearDrag(0.4f);

    box->SetAngularDrag(0.5f);

    box->SetElasticity(0.6f);
    box1->SetElasticity(0.7f);
    box2->SetElasticity(0.8f);
	
    Circle* circle = new Circle(glm::vec2(0), glm::vec2(0), 1.f, 0.5f);
    Circle* circle1 = new Circle(glm::vec2(20, 0), glm::vec2(0), 1.f, 0.5f);
    Circle* circle2 = new Circle(glm::vec2(-20, 0), glm::vec2(0), 1.f, 0.5f);
    Circle* circle3 = new Circle(glm::vec2(5, -20), glm::vec2(0), 1.f, 0.5f);
    Circle* circle4 = new Circle(glm::vec2(-5, -20), glm::vec2(0), 1.f, 0.5f);
    Circle* circle5 = new Circle(glm::vec2(10, -30), glm::vec2(0), 1.f, 0.5f);
    Circle* circle6 = new Circle(glm::vec2(-10, -30), glm::vec2(0), 1.f, 0.5f);
	
    circle->SetKinematic(true);
    circle1->SetKinematic(true);
    circle2->SetKinematic(true);
    circle3->SetKinematic(true);
    circle4->SetKinematic(true);
    circle5->SetKinematic(true);
    circle6->SetKinematic(true);

    AddActor(plane);
    AddActor(plane1);
    AddActor(box);
    AddActor(box1);
    AddActor(box2);
    AddActor(circle);
    AddActor(circle1);
    AddActor(circle2);
    AddActor(circle3);
    AddActor(circle4);
    AddActor(circle5);
    AddActor(circle6);
}
