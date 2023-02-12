#include "Pool_Table.h"

#include <Gizmos.h>
#include <Input.h>

void Pool_Table::Startup(aie::Application* _app)
{
    _app->setBackgroundColour(0.f, 0.2f, 0.1f);
    
    // Cue ball
    m_cueBallStartPos = glm::vec2(-60, 0);
    m_cueBall = new Billiard(m_cueBallStartPos, glm::vec2(0), 2.f, 3.f, glm::vec4(1), 0.9f);
    m_cueBall->SetLinearDrag(0.7f);
    m_cueBall->collisionCallback = std::bind(&Pool_Table::CueBallCollision, this, std::placeholders::_1);
    AddActor(m_cueBall);

    // Prevents errors later on
    m_firstHit = m_cueBall;

    // First makes the triangle using recursion, then passes the made
    // triangle to the ColorTriangle function with the 2 teams colors
    vector<Billiard*> triangle = MakeTriangle(glm::vec2(40, 0), 6.f);
    ColorTriangle(triangle, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));

    MakePoolTable();
}

void Pool_Table::Update(float _dt)
{
    PhysicsScene::Update(_dt);

    aie::Input* input = aie::Input::getInstance();

    if (m_cueBall->GetVelocity() == glm::vec2(0))
    {
        // Didn't hit anything last time, extra turn
        if (m_firstHit == nullptr)
        {
            ExtraTurn();
            m_firstHit = m_cueBall;
        }
        
        if (m_turnAddCountdown == 0) // next players turn
        {
            m_playersTurn = (m_playersTurn + 1) % 2;
            m_turnAddCountdown = 1;
        }
        
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
            m_turnAddCountdown--;
            m_firstHit = nullptr;
            
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

    glm::vec4 color = m_playersTurn == 0 ? glm::vec4(1) : glm::vec4(1, 0, 0, 1);
    aie::Gizmos::add2DCircle(m_windowExtents, 5, 4, color);
}

vector<Billiard*> Pool_Table::MakeTriangle(glm::vec2 _startPos, float _xDiff, int _rows)
{
    vector<Billiard*> billiards;

    // Create _rows amount of rows
    for (int i = 0; i < _rows; i++)
    {
        const float radius = 3.25f;
        const float yDiff = abs((_xDiff * i * sinf(DegreeToRadian(30)))
                                / sinf(DegreeToRadian(60)));
        
        // Add billiards outwards, i.e. 1 up-right, 1 down-right
        glm::vec2 posDiff = glm::vec2(_xDiff * i, yDiff);
        billiards.push_back(new Billiard(_startPos + posDiff, glm::vec2(0), 3.f, radius));

        posDiff = glm::vec2(posDiff.x, -posDiff.y);

        // Only adds second if position is not occupied
        if (_startPos + posDiff != billiards.back()->GetPosition())
            billiards.push_back(new Billiard(_startPos + posDiff, glm::vec2(0), 3.f, radius));
    }

    if (_rows - 2 > 0) // if rows left is not 0 or negative, run recursion
    {
        vector<Billiard*> extras = MakeTriangle(_startPos + glm::vec2(_xDiff * 2.f, 0), _xDiff, _rows - 2);
        billiards.insert(billiards.end(), extras.begin(), extras.end());
    }

    // Sets drag & elasticity, then adds billiards to the scene
    for (auto ball : billiards)
    {
        // If a ball already exists at this position, there has been
        // a reference to the same ball added already, so skip it
        auto pred = [=] (PhysicsObject* _billiard)
        {
            Billiard* temp = dynamic_cast<Billiard*>(_billiard);
            return temp != nullptr && temp->GetPosition() == ball->GetPosition();
        };
        
        if (std::find_if(m_actors.begin(), m_actors.end(), pred) != m_actors.end())
        {
            continue;
        }
        
        ball->SetLinearDrag(0.9f);
        ball->SetElasticity(0.8f);
        AddActor(ball);
    }

    return billiards;
}

void Pool_Table::ColorTriangle(vector<Billiard*>& _balls, glm::vec4 _color1, glm::vec4 _color2, glm::vec4 _8BallColor)
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

        // Set billiard type, this HAS to be before the 8-ball logic
        _balls.at(i)->billiardType = ballColor == _color1 ? Billiard::ColorBall1 : Billiard::ColorBall2;
        
        // 8-ball
        if (i == rows * 2 - 1)
        {
            ballColor = _8BallColor;
            _balls.at(i)->billiardType = Billiard::EightBall;
        }

        // Update totals
        color1 += ballColor == _color1;
        color2 += ballColor == _color2;

        // Set billiard color
        _balls.at(i)->SetColor(ballColor);
    }
}

void Pool_Table::MakePoolTable()
{
    // Boxes for the screen edges
    const glm::vec4 edgeColor = glm::vec4(0, 0.7, 0, 1);

    float cornerRadius = 14.5f, centreRadius = 10.f, edgeWidth = 5.f;
    float halfXSize = (m_windowExtents.x - cornerRadius - centreRadius) * 0.5f;

    // Loop to make top and bottom edges
    for (int i = 0; i < 4; i++)
    {
        glm::vec2 boxPos = glm::vec2((centreRadius + halfXSize) * (i % 3 == 0 ? -1.f : 1.f),
                                    m_windowExtents.y * (i % 2 == 0 ? -1.f : 1.f));
        glm::vec2 boxSize = glm::vec2(halfXSize, edgeWidth);
        glm::vec2 circlePos = boxPos + glm::vec2(boxSize.x, 0) * (i % 2 == 0 ? -1.f : 1.f);

        AddActor(new Box(boxPos, glm::vec2(), 1, boxSize, 0, edgeColor));
        AddActor(new Circle(circlePos, glm::vec2(), 1, edgeWidth, edgeColor));
        AddActor(new Circle(glm::vec2(-circlePos.x, circlePos.y), glm::vec2(), 1, edgeWidth, edgeColor));
    }

    // Loop to make left and right edges
    for (int i = 0; i < 2; i++)
    {
        glm::vec2 boxPos = glm::vec2(m_windowExtents.x * (i % 2 == 0 ? -1.f : 1.f), 0);
        glm::vec2 boxSize = glm::vec2(edgeWidth, m_windowExtents.y - cornerRadius);
        glm::vec2 circlePos = boxPos + glm::vec2(0, boxSize.y) * (i % 2 == 0 ? -1.f : 1.f);

        AddActor(new Box(boxPos, glm::vec2(), 1, boxSize, 0, edgeColor));
        AddActor(new Circle(circlePos, glm::vec2(), 1, edgeWidth, edgeColor));
        AddActor(new Circle(glm::vec2(circlePos.x, -circlePos.y), glm::vec2(), 1, edgeWidth, edgeColor));
    }

    // Loop for setting edge kinematic and elasticity
    for (int i = 0; i < 18; i++)
    {
        dynamic_cast<Rigidbody*>(m_actors.at(m_actors.size() - 1 - i))->SetKinematic(true);
        m_actors.at(m_actors.size() - 1 - i)->SetElasticity(0.6f);
    }

    // Pockets
    glm::vec4 pocketColor = glm::vec4(0, 0, 0, 1);
    float pocketRadius = centreRadius - edgeWidth;
    
    glm::vec2 cornerPos = glm::vec2(m_windowExtents.x, m_windowExtents.y) - edgeWidth;

    // Loop for making the 3 pockets (left, mid, right) for top + bottom
    for (int i = 0; i < 2; i++)
    {
        float multi = i % 2 == 0 ? 1.f : -1.f;
        
        AddActor(new Circle(glm::vec2(-cornerPos.x, cornerPos.y) * multi, glm::vec2(), 1, pocketRadius, pocketColor));
        AddActor(new Circle(glm::vec2(0, m_windowExtents.y * multi), glm::vec2(), 1, pocketRadius, pocketColor));
        AddActor(new Circle(cornerPos * multi, glm::vec2(), 1, pocketRadius, pocketColor));
    }

    // Loop for setting the pockets to triggers and adding a callback
    for (int i = 0; i < 6; i++)
    {
        Rigidbody* rb = dynamic_cast<Rigidbody*>(m_actors.at(m_actors.size() - 1 - i));
        
        rb->SetTrigger(true);
        rb->triggerEnter = std::bind(&Pool_Table::PocketEnter, this, std::placeholders::_1);
    }
}

void Pool_Table::PocketEnter(PhysicsObject* _other)
{
    Billiard* billiard = dynamic_cast<Billiard*>(_other);
    
    if (billiard) // if object is of type 'Billiard'
    {
        Billiard::BilliardType type = billiard->billiardType;

        // Cue ball went into pocket
        if (type == Billiard::CueBall)
        {
            // Extra turn for the other team, reset the cue ball
            ExtraTurn();

            m_cueBall->SetPosition(m_cueBallStartPos);
            m_cueBall->SetVelocity(glm::vec2(0));
        }

        // Eight ball went into pocket
        if (type == Billiard::EightBall)
        {
            
        }

        // Team ball went into pocket
        if (type == Billiard::ColorBall1 || type == Billiard::ColorBall2)
        {
            // First team ball sunk, set team types and team counters
            if (m_team1Type == Billiard::Null || m_team2Type == Billiard::Null)
            {
                Billiard::BilliardType otherType = type == Billiard::ColorBall1 ? Billiard::ColorBall2 : Billiard::ColorBall1;

                m_team1Type = m_playersTurn == 0 ? type : otherType;
                m_team2Type = m_playersTurn == 1 ? type : otherType;
                
                // Set the billiard counters
                for (int i = 0; i < m_actors.size(); i++)
                {
                    Billiard* temp = dynamic_cast<Billiard*>(m_actors.at(i));
                    
                    if (temp != nullptr)
                    {
                        m_team1Counter += (temp->billiardType == m_team1Type);
                        m_team2Counter += (temp->billiardType == m_team2Type);
                    }
                }
            }
            
            // Decrement the right team counter
            m_team1Counter -= m_playersTurn == 0;
            m_team2Counter -= m_playersTurn == 1;

            // Remove the billiard from the scene
            RemoveActor(billiard);
        }
    }
}

void Pool_Table::CueBallCollision(PhysicsObject* _other)
{
    Billiard* first = dynamic_cast<Billiard*>(_other);
    
    if (first && m_firstHit == nullptr) // First billiard hit
    {
        m_firstHit = first;
        
        // First hit was the other teams ball
        if ((m_playersTurn == 0 && m_firstHit->billiardType == m_team1Type) ||
            (m_playersTurn == 1 && m_firstHit->billiardType == m_team2Type) ||
            m_firstHit->billiardType == Billiard::EightBall)
        {
            ExtraTurn();
        }
    }
}