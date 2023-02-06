#include "NewtonsLaws.h"

#include <Input.h>

void NewtonsLaws::Startup()
{
#ifdef NewtonsFirstLaw
    Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
    AddActor(ball);
#endif
	
#ifdef NewtonsSecondLaw
    SetGravity(glm::vec2(0, -15));
	
    Circle* ball = new Circle(glm::vec2(-40, 0), glm::vec2(10, 30), 3.0f, 1, glm::vec4(1, 0, 0, 1));
    AddActor(ball);
#endif
	
#ifdef NewtonsThirdLaw
    Circle* ball1 = new Circle(glm::vec2(-4, 0), glm::vec2(0, 0), 0.1f, 4, glm::vec4(1, 0, 0, 1));
    Circle* ball2 = new Circle(glm::vec2(4, 0), glm::vec2(0, 0), 0.1f, 4, glm::vec4(0, 1, 0, 1));

    AddActor(ball1);
    AddActor(ball2);
#endif
}

void NewtonsLaws::Update(float _dt)
{
    PhysicsScene::Update(_dt);

    aie::Input* input = aie::Input::getInstance();

#ifdef NewtonsSecondLaw // Switches gravity to the corresponding arrow key direction
    if (input->wasKeyPressed(aie::INPUT_KEY_LEFT))
        SetGravity(glm::vec2(-15, 0));
	
    else if (input->wasKeyPressed(aie::INPUT_KEY_RIGHT))
        SetGravity(glm::vec2(15, 0));
	
    else if (input->wasKeyPressed(aie::INPUT_KEY_DOWN))
        SetGravity(glm::vec2(0, -15));
	
    else if (input->wasKeyPressed(aie::INPUT_KEY_UP))
        SetGravity(glm::vec2(0, 15));
#endif
	
#ifdef NewtonsThirdLaw // Mock 'collision' between 2 balls of different masses
    if (input->wasKeyPressed(aie::INPUT_KEY_SPACE))
    {
        Circle* ball1 = dynamic_cast<Circle*>(GetActors()->at(0));
        Circle* ball2 = dynamic_cast<Circle*>(GetActors()->at(1));
		
        ball1->ApplyForce(glm::vec2(2, 0), glm::vec2(0));
        ball2->ApplyForce(-glm::vec2(2, 0), glm::vec2(0));
    }
#endif
}
