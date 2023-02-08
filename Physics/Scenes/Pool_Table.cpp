#include "Pool_Table.h"

#include <Gizmos.h>
#include <Input.h>

void Pool_Table::Startup()
{
    // Cue ball
    m_cueBall = new Circle(glm::vec2(-60, 0), glm::vec2(0), 2.f, 3.5, glm::vec4(1), 0.8f);
    m_cueBall->SetLinearDrag(0.7f);
    AddActor(m_cueBall);

    // Pool table edges
    for(int i = 0; i < 4; i++)
    {
        glm::vec2 planeNormal = glm::vec2(i % 2, (1 + i) % 2) * (i % 3 == 0 ? -1.f : 1.f);
        float planeDist = planeNormal.x == 0 ? -50.f : -95.f;
        AddActor(new Plane(planeNormal, planeDist, glm::vec4(0, 0.75, 0, 1), 0.6f));
    }

    // First makes the triangle using recursion, then passes the made
    // triangle to the ColorTriangle function with the 2 teams colors
    vector<Circle*> triangle = MakeTriangle(glm::vec2(30, 0), 7.5f);
    ColorTriangle(triangle,glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
    
    for (auto ball : triangle) // sets drag & elasticity, then adds triangle to the scene
    {
        ball->SetLinearDrag(0.9f);
        ball->SetElasticity(0.8f);
        AddActor(ball);
    }
}

void Pool_Table::Update(float _dt)
{
    PhysicsScene::Update(_dt);

    aie::Input* input = aie::Input::getInstance();

    if (m_cueBall->GetVelocity() == glm::vec2(0))
    {
        float turnSpeed = 2.5f;
        float orientation = m_cueBall->GetOrientation();
	
        if (input->isKeyDown(aie::INPUT_KEY_LEFT))
            orientation += turnSpeed;

        if (input->isKeyDown(aie::INPUT_KEY_RIGHT))
            orientation -= turnSpeed;
	
        m_cueBall->SetOrientation(orientation);

        float rads = DegreeToRadian(m_cueBall->GetOrientation());
        const glm::vec2 forceDir = normalize(glm::vec2(cos(rads), sin(rads)));

        if (input->wasKeyPressed(aie::INPUT_KEY_SPACE))
        {
            static float forceAmount = 650.f;
            m_cueBall->ApplyForce(forceDir * forceAmount, glm::vec2(0));
        }
    }
}

void Pool_Table::Draw()
{
    PhysicsScene::Draw();

    if (m_cueBall->GetVelocity() == glm::vec2(0))
    {
        float rads = DegreeToRadian(m_cueBall->GetOrientation());
        const glm::vec2 forceDir = normalize(glm::vec2(cos(rads), sin(rads)));
        
        aie::Gizmos::add2DLine(m_cueBall->GetPosition(), m_cueBall->GetPosition()
                        + forceDir * m_cueBall->GetRadius(), glm::vec4(1, 0, 0, 1));
    }
}

vector<Circle*> Pool_Table::MakeTriangle(glm::vec2 _startPos, float _xDiff, int _rows)
{
    vector<Circle*> billiards;

    // Create _rows amount of rows
    for (int i = 0; i < _rows; i++)
    {
        float yDiff = abs((_xDiff * i * sinf(DegreeToRadian(30)))
                                / sinf(DegreeToRadian(60)));
        
        // Add billiards outwards, i.e. 1 up-right, 1 down-right
        glm::vec2 posDiff = glm::vec2(_xDiff * i, yDiff);
        billiards.push_back(new Circle(_startPos + posDiff, glm::vec2(0), 3.f, 4.f));

        posDiff = glm::vec2(posDiff.x, -posDiff.y);

        // Only adds second if position is not occupied
        if (_startPos + posDiff != billiards.back()->GetPosition())
            billiards.push_back(new Circle(_startPos + posDiff, glm::vec2(0), 3.f, 4.f));
    }

    if (_rows - 2 > 0) // if rows left is not 0 or negative, run recursion
    {
        vector<Circle*> extras = MakeTriangle(_startPos + glm::vec2(_xDiff * 2.f, 0), _xDiff, _rows - 2);
        billiards.insert(billiards.end(), extras.begin(), extras.end());
    }

    return billiards;
}

void Pool_Table::ColorTriangle(vector<Circle*>& _balls, glm::vec4 _color1, glm::vec4 _color2, glm::vec4 _8BallColor)
{
    int ballTotal = (int)_balls.size(), color1 = 0, color2 = 0;

    // Because of the recursion algorithm to make the triangle,
    // this is the only way to get the exact row amount
    int rows = (int)floor((-1 + sqrt(1 - 4 * 1 * (-2 * _balls.size()))) / 2);

    for (int i = 0; i < (int)_balls.size(); i++)
    {
        // Set random color, out of 2 passed in colors
        glm::vec4 ballColor = (rand() % 2 == 0) ? _color1 : _color2;

        // If too much of colorX, set it to colorY
        ballColor = color1 >= floor(ballTotal * 0.5f) ? _color2 : ballColor;
        ballColor = color2 >= floor(ballTotal * 0.5f) ? _color1 : ballColor;

        // 8-ball
        if (i == rows * 2 - 1) ballColor = _8BallColor;

        // Update totals
        color1 += ballColor == _color1;
        color2 += ballColor == _color2;

        _balls.at(i)->SetColor(ballColor);
    }
}
