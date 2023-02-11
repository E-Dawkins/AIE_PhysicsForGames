#include "Newtons_First_Law.h"

void Newtons_First_Law::Startup(aie::Application* _app)
{
    Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
    AddActor(ball);
}
