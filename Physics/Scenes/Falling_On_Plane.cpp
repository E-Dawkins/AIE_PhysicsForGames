#include "Falling_On_Plane.h"

void Falling_On_Plane::Startup()
{
    SetGravity(glm::vec2(0, -10));
	
    Plane* plane = new Plane(glm::vec2(0.707, 0.707), -40, glm::vec4(1));
    Plane* plane2 = new Plane(glm::vec2(-0.707, 0.707), -40, glm::vec4(1));
    Circle* circle = new Circle(glm::vec2(-50, 10), glm::vec2(0), 1.f, 1, glm::vec4(1, 0, 0, 1));
    Circle* circle1 = new Circle(glm::vec2(30, 40), glm::vec2(0), 2.f, 5, glm::vec4(1, 0, 1, 1));
    Circle* circle2 = new Circle(glm::vec2(-40, 20), glm::vec2(0), 3.f, 6, glm::vec4(0, 1, 1, 1));
    Box* box = new Box(glm::vec2(50, 10), glm::vec2(0), 1.f, glm::vec2(4), 0.f, glm::vec4(0, 0, 1, 1));
    Box* box1 = new Box(glm::vec2(20, 10), glm::vec2(0), 1.f, glm::vec2(4), 27.f, glm::vec4(0, 0, 1, 1));
    Box* box2 = new Box(glm::vec2(30, 20), glm::vec2(0), 1.f, glm::vec2(4), 54.f, glm::vec4(0, 0, 1, 1));
    Box* box3 = new Box(glm::vec2(10, 0), glm::vec2(0), 1.f, glm::vec2(4), 92.f, glm::vec4(0, 0, 1, 1));

    Box* box4 = new Box(glm::vec2(0, -20), glm::vec2(0), 1.f, glm::vec2(10), 0);
    box4->SetKinematic(true);
	
    AddActor(plane);
    AddActor(plane2);
    AddActor(circle);
    AddActor(circle1);
    AddActor(circle2);
    AddActor(box);
    AddActor(box1);
    AddActor(box2);
    AddActor(box3);
    AddActor(box4);
}
