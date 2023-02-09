#include "Pool_Table.h"

#include <Gizmos.h>
#include <Input.h>

void Pool_Table::Startup()
{
    // Cue ball
    m_cueBall = new Circle(glm::vec2(-60, 0), glm::vec2(0), 2.f, 3.f);
    m_cueBall->SetLinearDrag(0.9f);
    m_cueBall->SetLinearDrag(0.7f);
    AddActor(m_cueBall);

    // First makes the triangle using recursion, then passes the made
    // triangle to the ColorTriangle function with the 2 teams colors
    vector<Circle*> triangle = MakeTriangle(glm::vec2(30, 0), 6.f);
    ColorTriangle(triangle, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));

    MakePoolTable();
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
        const float radius = 3.25f;
        const float yDiff = abs((_xDiff * i * sinf(DegreeToRadian(30)))
                                / sinf(DegreeToRadian(60)));
        
        // Add billiards outwards, i.e. 1 up-right, 1 down-right
        glm::vec2 posDiff = glm::vec2(_xDiff * i, yDiff);
        billiards.push_back(new Circle(_startPos + posDiff, glm::vec2(0), 3.f, radius));

        posDiff = glm::vec2(posDiff.x, -posDiff.y);

        // Only adds second if position is not occupied
        if (_startPos + posDiff != billiards.back()->GetPosition())
            billiards.push_back(new Circle(_startPos + posDiff, glm::vec2(0), 3.f, radius));
    }

    if (_rows - 2 > 0) // if rows left is not 0 or negative, run recursion
    {
        vector<Circle*> extras = MakeTriangle(_startPos + glm::vec2(_xDiff * 2.f, 0), _xDiff, _rows - 2);
        billiards.insert(billiards.end(), extras.begin(), extras.end());
    }

    // Sets drag & elasticity, then adds billiards to the scene
    for (auto ball : billiards)
    {
        ball->SetLinearDrag(0.9f);
        ball->SetElasticity(0.8f);
        AddActor(ball);
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

void Pool_Table::MakePoolTable()
{
    // Boxes for the screen edges
    const glm::vec4 edgeColor = glm::vec4(0, 0.7, 0, 1);

    float cornerRadius = 14.5f;
    float centreRadius = 10.f;

    float edgeWidth = 5.f;

    float halfXSize = (m_windowExtents.x - cornerRadius - centreRadius) * 0.5f;
    float xPos = centreRadius + halfXSize;
    
    Box* top1 = new Box(glm::vec2(xPos, m_windowExtents.y), glm::vec2(0),
        1.f, glm::vec2(halfXSize, edgeWidth), 0.f, edgeColor);
    Box* top2 = new Box(glm::vec2(-xPos, m_windowExtents.y), glm::vec2(0),
        1.f, glm::vec2(halfXSize, edgeWidth), 0.f, edgeColor);

    Box* bot1 = new Box(glm::vec2(xPos, -m_windowExtents.y), glm::vec2(0),
        1.f, glm::vec2(halfXSize, edgeWidth), 0.f, edgeColor);
    Box* bot2 = new Box(glm::vec2(-xPos, -m_windowExtents.y), glm::vec2(0),
        1.f, glm::vec2(halfXSize, edgeWidth), 0.f, edgeColor);
    
    Box* left = new Box(glm::vec2(-m_windowExtents.x, 0), glm::vec2(0), 1.f,
        glm::vec2(edgeWidth, m_windowExtents.y - cornerRadius), 0.f, edgeColor);
    Box* right = new Box(glm::vec2(m_windowExtents.x, 0), glm::vec2(0), 1.f,
        glm::vec2(edgeWidth, m_windowExtents.y - cornerRadius), 0.f, edgeColor);

    top1->SetKinematic(true);
    top2->SetKinematic(true);
    bot1->SetKinematic(true);
    bot2->SetKinematic(true);
    right->SetKinematic(true);
    left->SetKinematic(true);
    
    AddActor(top1);
    AddActor(top2);
    AddActor(bot1);
    AddActor(bot2);
    AddActor(right);
    AddActor(left);

    // Makes the edges seem 'rounded', really just circles on the ends
    Circle* topRnd1 = new Circle(glm::vec2(xPos + halfXSize, m_windowExtents.y),
        glm::vec2(0), 1.f, edgeWidth, edgeColor);
    Circle* topRnd2 = new Circle(glm::vec2(xPos - halfXSize, m_windowExtents.y),
        glm::vec2(0), 1.f, edgeWidth, edgeColor);
    Circle* topRnd3 = new Circle(glm::vec2(-xPos + halfXSize, m_windowExtents.y),
        glm::vec2(0), 1.f, edgeWidth, edgeColor);
    Circle* topRnd4 = new Circle(glm::vec2(-xPos - halfXSize, m_windowExtents.y),
        glm::vec2(0), 1.f, edgeWidth, edgeColor);

    Circle* bottomRnd1 = new Circle(glm::vec2(xPos + halfXSize, -m_windowExtents.y),
        glm::vec2(0), 1.f, edgeWidth, edgeColor);
    Circle* bottomRnd2 = new Circle(glm::vec2(xPos - halfXSize, -m_windowExtents.y),
        glm::vec2(0), 1.f, edgeWidth, edgeColor);
    Circle* bottomRnd3 = new Circle(glm::vec2(-xPos + halfXSize, -m_windowExtents.y),
        glm::vec2(0), 1.f, edgeWidth, edgeColor);
    Circle* bottomRnd4 = new Circle(glm::vec2(-xPos - halfXSize, -m_windowExtents.y),
        glm::vec2(0), 1.f, edgeWidth, edgeColor);
    
    Circle* leftRnd1 = new Circle(glm::vec2(-m_windowExtents.x, m_windowExtents.y
            - cornerRadius), glm::vec2(0), 1.f, edgeWidth, edgeColor);
    Circle* leftRnd2 = new Circle(glm::vec2(-m_windowExtents.x, -m_windowExtents.y
            + cornerRadius), glm::vec2(0), 1.f, edgeWidth, edgeColor);
    Circle* rightRnd1 = new Circle(glm::vec2(m_windowExtents.x, m_windowExtents.y
            - cornerRadius), glm::vec2(0), 1.f, edgeWidth, edgeColor);
    Circle* rightRnd2 = new Circle(glm::vec2(m_windowExtents.x, -m_windowExtents.y
            + cornerRadius), glm::vec2(0), 1.f, edgeWidth, edgeColor);

    topRnd1->SetKinematic(true);
    topRnd2->SetKinematic(true);
    topRnd3->SetKinematic(true);
    topRnd4->SetKinematic(true);
    bottomRnd1->SetKinematic(true);
    bottomRnd2->SetKinematic(true);
    bottomRnd3->SetKinematic(true);
    bottomRnd4->SetKinematic(true);
    leftRnd1->SetKinematic(true);
    leftRnd2->SetKinematic(true);
    rightRnd1->SetKinematic(true);
    rightRnd2->SetKinematic(true);
    
    AddActor(topRnd1);
    AddActor(topRnd2);
    AddActor(topRnd3);
    AddActor(topRnd4);
    AddActor(bottomRnd1);
    AddActor(bottomRnd2);
    AddActor(bottomRnd3);
    AddActor(bottomRnd4);
    AddActor(leftRnd1);
    AddActor(leftRnd2);
    AddActor(rightRnd1);
    AddActor(rightRnd2);

    // Pockets
    glm::vec4 pocketColor = glm::vec4(1, 0, 0, 1);
    float pocketRadius = centreRadius - edgeWidth;

    glm::vec2 cornerPos = glm::vec2(m_windowExtents.x, m_windowExtents.y) - edgeWidth;
    
    Circle* topMid = new Circle(glm::vec2(0, m_windowExtents.y), glm::vec2(),
                        1.f, pocketRadius, pocketColor);
    Circle* topLeft = new Circle(glm::vec2(-cornerPos.x, cornerPos.y), glm::vec2(), 1.f, pocketRadius, pocketColor);
    Circle* topRight = new Circle(cornerPos, glm::vec2(), 1.f, pocketRadius, pocketColor);

    Circle* botMid = new Circle(glm::vec2(0, -m_windowExtents.y), glm::vec2(),
                        1.f, pocketRadius, pocketColor);
    Circle* botLeft = new Circle(glm::vec2(-cornerPos.x, -cornerPos.y), glm::vec2(), 1.f, pocketRadius, pocketColor);
    Circle* botRight = new Circle(glm::vec2(cornerPos.x, -cornerPos.y), glm::vec2(), 1.f, pocketRadius, pocketColor);

    topMid->SetTrigger(true);
    topLeft->SetTrigger(true);
    topRight->SetTrigger(true);
    botMid->SetTrigger(true);
    botLeft->SetTrigger(true);
    botRight->SetTrigger(true);
    
    AddActor(topMid);
    AddActor(topLeft);
    AddActor(topRight);
    AddActor(botMid);
    AddActor(botLeft);
    AddActor(botRight);
}
