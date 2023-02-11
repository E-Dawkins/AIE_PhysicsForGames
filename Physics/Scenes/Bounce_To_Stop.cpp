#include "Bounce_To_Stop.h"

void Bounce_To_Stop::Startup(aie::Application* _app)
{
    SetGravity(glm::vec2(0, -10));
	
    Plane* plane = new Plane(glm::vec2(0, 1), -40, glm::vec4(1), 0.3f);
    Box* box = new Box(glm::vec2(50, -30), glm::vec2(0), 1.f, glm::vec2(4), 0.f, glm::vec4(0, 1, 0, 1), 0.3f);
    Box* box1 = new Box(glm::vec2(49, 0), glm::vec2(0), 1.f, glm::vec2(4), 0.f, glm::vec4(0, 0, 1, 1), 0.3f);
    Circle* circle = new Circle(glm::vec2(-50, 10), glm::vec2(0), 1.f, 4, glm::vec4(1, 0, 0, 1), 0.3f);
	
    AddActor(plane);
    AddActor(box);
    AddActor(box1);
    AddActor(circle);
}
