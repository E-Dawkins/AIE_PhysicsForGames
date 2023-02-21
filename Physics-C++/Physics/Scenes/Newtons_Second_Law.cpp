#include "Newtons_Second_Law.h"

void Newtons_Second_Law::Startup(aie::Application* _app)
{
    SetGravity(glm::vec2(0, -15));
	
    Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
    AddActor(ball);
}
