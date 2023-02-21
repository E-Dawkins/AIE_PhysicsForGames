#include "Simulating_Collisions.h"

void Simulating_Collisions::Startup(aie::Application* _app)
{
    SetGravity(glm::vec2(0, -10));
	
    Circle* ball1 = new Circle(glm::vec2(-50, 0), glm::vec2(30, 20), 1.0f, 4, glm::vec4(1, 0, 0, 1));
    Circle* ball2 = new Circle(glm::vec2(50, 0), glm::vec2(-30, 20), 1.0f, 4, glm::vec4(0, 1, 0, 1));

    AddActor(ball1);
    AddActor(ball2);
}
