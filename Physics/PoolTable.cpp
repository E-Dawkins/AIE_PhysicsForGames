#include "PoolTable.h"

#include <Gizmos.h>
#include <Input.h>

void PoolTable::Startup()
{
    // Cue ball
    AddActor(new Circle(glm::vec2(-60, 0), glm::vec2(0), 2.f, 3.5, glm::vec4(1), 0.8f));

    // Pool table edges
    for(int i = 0; i < 4; i++)
    {
        glm::vec2 planeNormal = glm::vec2(i % 2, (1 + i) % 2) * (i % 3 == 0 ? -1.f : 1.f);
        float planeDist = planeNormal.x == 0 ? -50.f : -95.f;
        AddActor(new Plane(planeNormal, planeDist, glm::vec4(0, 0.75, 0, 1), 0.6f));
    }

    // Billiards
    std::vector<Circle*> billiards;

    int billiardRows = 5;
    int ballTotal = ((billiardRows * (1 + billiardRows)) / 2) - 1;

    glm::vec2 billiardStart = glm::vec2(30, 0);

    glm::vec4 colors[] =
    {
        glm::vec4(1, 0, 0, 1),
        glm::vec4(1, 1, 0, 1),
        glm::vec4(0.15, 0.15, 0.15, 1)
    };

    // Delegate for adding billiard to the scene
    auto AddBilliard = [&colors, &ballTotal, &billiards, this](int _rowIndex, glm::vec2 _ballPos)
    {
        static int color1 = 0;
        static int color2 = 0;

        glm::vec4 billiardColor = colors[rand() % 2];

        // Too much of colorX, make it colorY
        billiardColor = (color1 >= floor(ballTotal * 0.5f)) ? colors[1] : billiardColor;
        billiardColor = (color2 >= floor(ballTotal * 0.5f)) ? colors[0] : billiardColor;

        // 8 ball
        billiardColor = (_rowIndex == 2 && _ballPos.y == 0) ? colors[2] : billiardColor;

        // Add to color totals
        color1 += billiardColor == colors[0];
        color2 += billiardColor == colors[1];

        // Add billiard to scene
        billiards.push_back(new Circle(_ballPos, glm::vec2(0), 3.f, 4, billiardColor, 0.8f));
        AddActor(billiards.back());
    };

    for(int i = 0; i < billiardRows; i++)
    {
        // If the first ball, then add it and continue to next loop
        if(billiards.empty())
        {
            AddBilliard(0, billiardStart);
            continue;
        }

        std::vector<Circle*> temp;

        // Add previous row of billiards into temp vector
        for(int j = 0; j < i; j++)
        {
            temp.push_back(billiards.at(billiards.size() - 1 - j));
        }

        // Loop through all billiards in temp, and add
        // new ones either side of it in the next row
        for(auto ball : temp)
        {
            glm::vec2 billiardPos = ball->GetPosition() + glm::vec2(7.5f, 4.5f);
            glm::vec2 billiardPos2 = ball->GetPosition() + glm::vec2(7.5f, -4.5f);

            bool result1 = billiards.end() == std::find_if(billiards.begin(), billiards.end(),
                                                           [billiardPos](Circle* _circle) { return _circle->GetPosition() == billiardPos; });

            bool result2 = billiards.end() == std::find_if(billiards.begin(), billiards.end(),
                                                           [billiardPos2](Circle* _circle) { return _circle->GetPosition() == billiardPos2; });

            // Adds the billiard if it has a unique position,
            // i.e. that position is not already occupied
            if(result1) AddBilliard(i, billiardPos);
            if(result2) AddBilliard(i, billiardPos2);
        }

        temp.clear();
    }
}

void PoolTable::Update(float _dt)
{
    PhysicsScene::Update(_dt);

    aie::Input* input = aie::Input::getInstance();

    static Circle* cueBall = dynamic_cast<Circle*>(GetActors()->at(0));

    if (cueBall->GetVelocity() == glm::vec2(0))
    {
        float turnSpeed = 1.f;
        float orientation = cueBall->GetOrientation();
	
        if (input->isKeyDown(aie::INPUT_KEY_LEFT))
            orientation += turnSpeed;

        if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
            orientation -= turnSpeed;
	
        cueBall->SetOrientation(orientation);

        float rads = DegreeToRadian(cueBall->GetOrientation());
        const glm::vec2 forceDir = normalize(glm::vec2(cos(rads), sin(rads)));

        if (input->wasKeyPressed(aie::INPUT_KEY_SPACE))
        {
            static float forceAmount = 650.f;
            cueBall->ApplyForce(forceDir * forceAmount, glm::vec2(0));
        }
    }
}

void PoolTable::Draw()
{
    PhysicsScene::Draw();

    static Circle* cueBall = dynamic_cast<Circle*>(GetActors()->at(0));

    if (cueBall->GetVelocity() == glm::vec2(0))
    {
        float rads = DegreeToRadian(cueBall->GetOrientation());
        const glm::vec2 forceDir = normalize(glm::vec2(cos(rads), sin(rads)));
		
        aie::Gizmos::add2DLine(cueBall->GetPosition(), cueBall->GetPosition()
                        + forceDir * cueBall->GetRadius(), glm::vec4(1, 0, 0, 1));
    }
}
