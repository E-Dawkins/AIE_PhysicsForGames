#include "Circle_To_Plane.h"

void Circle_To_Plane::Startup()
{
    SetGravity(glm::vec2(0, -10));

    Circle* ball1 = new Circle(glm::vec2(-40, 50), glm::vec2(0), 4.0f, 4, glm::vec4(1, 0, 0, 1));
    Circle* ball2 = new Circle(glm::vec2(20, 55), glm::vec2(0), 10.0f, 6, glm::vec4(0, 1, 0, 1));
    Circle* ball3 = new Circle(glm::vec2(10, 35), glm::vec2(0), 7.0f, 5, glm::vec4(0, 0, 1, 1));
    Circle* ball4 = new Circle(glm::vec2(-15, 45), glm::vec2(0), 1.0f, 2, glm::vec4(1, 0, 1, 1));
    Plane* plane = new Plane(glm::vec2(0.866, 0.5), -20, glm::vec4(1));
    Plane* plane2 = new Plane(glm::vec2(-0.866, 0.5), -20, glm::vec4(1));

    AddActor(ball1);
    AddActor(ball2);
    AddActor(ball3);
    AddActor(ball4);
    AddActor(plane);
    AddActor(plane2);
}
