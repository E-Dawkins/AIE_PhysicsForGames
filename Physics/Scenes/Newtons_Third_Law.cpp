#include "Newtons_Third_Law.h"

#include <Input.h>

void Newtons_Third_Law::Startup(aie::Application* _app)
{
    Circle* ball1 = new Circle(glm::vec2(-4, 0), glm::vec2(0, 0), 0.1f, 4, glm::vec4(1, 0, 0, 1));
    Circle* ball2 = new Circle(glm::vec2(4, 0), glm::vec2(0, 0), 0.1f, 4, glm::vec4(0, 1, 0, 1));

    AddActor(ball1);
    AddActor(ball2);
}

void Newtons_Third_Law::Update(float _dt)
{
    PhysicsScene::Update(_dt);
    aie::Input* input = aie::Input::getInstance();

    if (input->wasKeyPressed(aie::INPUT_KEY_SPACE))
    {
        Circle* ball1 = dynamic_cast<Circle*>(GetActors()->at(0));
        Circle* ball2 = dynamic_cast<Circle*>(GetActors()->at(1));
		
        ball1->ApplyForce(glm::vec2(2, 0), glm::vec2(0));
        ball2->ApplyForce(-glm::vec2(2, 0), glm::vec2(0));
    }
}
