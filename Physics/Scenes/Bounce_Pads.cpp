#include "Bounce_Pads.h"

void Bounce_Pads::Startup()
{
    SetGravity(glm::vec2(0, -10));
	
    Box* box = new Box(glm::vec2(0, -30), glm::vec2(0), 1.f, glm::vec2(10, 2), 0);
    Box* box1 = new Box(glm::vec2(-50, -30), glm::vec2(0), 1.f, glm::vec2(10, 2), 0);
    Box* box2 = new Box(glm::vec2(50, -30), glm::vec2(0), 1.f, glm::vec2(10, 2), 0);

    Circle* circle = new Circle(glm::vec2(0), glm::vec2(0), 4.f, 4.f);
    Circle* circle1 = new Circle(glm::vec2(-50, 0), glm::vec2(0), 4.f, 4.f);
    Circle* circle2 = new Circle(glm::vec2(50, 0), glm::vec2(0), 4.f, 4.f);
	
    box->SetKinematic(true);
    box1->SetKinematic(true);
    box2->SetKinematic(true);
	
    AddActor(box);
    AddActor(box1);
    AddActor(box2);
    AddActor(circle);
    AddActor(circle1);
    AddActor(circle2);
}
