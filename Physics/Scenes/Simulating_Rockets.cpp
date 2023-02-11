#include "Simulating_Rockets.h"

#include <Input.h>

void Simulating_Rockets::Startup(aie::Application* _app)
{
    // m_physicsScene->SetGravity(glm::vec2(0, -10));
    Circle* ball = new Circle(glm::vec2(0), glm::vec2(0), 1000.f, 5, glm::vec4(1, 0, 0, 1));
    ball->SetOrientation(90);
    AddActor(ball);
}

void Simulating_Rockets::Update(float _dt)
{
    PhysicsScene::Update(_dt);
    aie::Input* input = aie::Input::getInstance();

    static Circle* rocket = dynamic_cast<Circle*>(GetActors()->at(0));
    static float fuelStorage = rocket->GetMass() * 0.75f;
    static float rocketMass = rocket->GetMass() - fuelStorage;

    const float rads = DegreeToRadian(rocket->GetOrientation());
    const float actualFuelMass = fuelStorage >= 20.f ? 20.f : fuelStorage;
	
    const glm::vec2 forceDir = normalize(glm::vec2(cos(rads), sin(rads)));
    const glm::vec2 fuelPos = rocket->GetPosition() - forceDir * rocket->GetRadius();

    static float accumulatedTime = 0.0f;

    // Expel fuel and apply the force to the rocket to make it launch
    if (accumulatedTime >= 0.15f && fuelStorage > 0)
    {
        Circle* fuel = new Circle(fuelPos, glm::vec2(0), actualFuelMass, 0.35f, glm::vec4(0, 1, 0, 1));
        AddActor(fuel);

        fuelStorage -= actualFuelMass;

        rocket->ApplyForce(-forceDir * -30.f * actualFuelMass, glm::vec2(0));
        fuel->ApplyForce(forceDir * -30.f * actualFuelMass, glm::vec2(0));
        rocket->SetMass(rocketMass + fuelStorage);

        accumulatedTime = 0.0f;
    }

    // turn the rocket
    float addOrientation = 0.f;
    const float turnSpeed = 2.5f;
	
    if (input->isKeyDown(aie::INPUT_KEY_LEFT))
        addOrientation += turnSpeed;

    if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
        addOrientation -= turnSpeed;

    rocket->SetOrientation(rocket->GetOrientation() + addOrientation);
	
    accumulatedTime += _dt;
}
