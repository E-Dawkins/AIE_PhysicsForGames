#include "Pool_Table.h"

#include <Gizmos.h>
#include <Input.h>

void Pool_Table::Startup(aie::Application* _app)
{
    _app->setBackgroundColour(0, 0, 0);

    glm::vec2 tableOffset = glm::vec2(0, -5);
    glm::vec2 tableExtents = m_windowExtents * 0.75f;

    Box* background = new Box(tableOffset, glm::vec2(0), 1.f, tableExtents, 0, glm::vec4(0, 0.2f, 0.1f, 1));
    background->SetTrigger(true);
    AddActor(background);
    
    MakePoolTable(tableOffset, tableExtents);
    
    // Cue ball
    m_cueBallStartPos = tableOffset + glm::vec2(-50, 0);
    m_cueBall = new Billiard(m_cueBallStartPos, glm::vec2(0), 2.f, 2.5f, glm::vec4(1), 0.9f);
    m_cueBall->SetLinearDrag(0.8f);
    m_cueBall->collisionCallback = std::bind(&Pool_Table::CueBallCollision, this, std::placeholders::_1);
    AddActor(m_cueBall);
    
    // Prevents errors later on
    m_firstHit = m_cueBall;

    // First makes the triangle using recursion, then passes the made
    // triangle to the ColorTriangle function with the 2 teams colors
    vector<Billiard*> triangle = MakeTriangle(tableOffset + glm::vec2(30, 0), 5.f);
    ColorTriangle(triangle, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));
}

void Pool_Table::Update(float _dt)
{
    PhysicsScene::Update(_dt);
    
    aie::Input* input = aie::Input::getInstance();

    if (m_cueBall->GetVelocity() == glm::vec2(0))
    {
        // Check if all balls have stopped moving
        for (auto obj : m_actors)
        {
            Rigidbody* rb = dynamic_cast<Rigidbody*>(obj);

            if (rb != nullptr && rb->GetVelocity() != glm::vec2(0))
                return;
        }
        
        m_cueBall->SetColor(glm::vec4(1));
        
        // Did eight ball go in?
        if (m_runEndgame)
        {
            // Did the cue ball go in after?
            if (m_eightBallFirst)
                EndGame(1 - m_playersTurn);

            // If not, this player won
            else EndGame(m_playersTurn);

            return;
        }
        
        // Didn't hit anything last time, extra turn
        if (m_firstHit == nullptr)
        {
            ExtraTurn();
            m_firstHit = m_cueBall;
        }
        
        if (m_turnAddCountdown == 0) // next players turn
        {
            m_playersTurn = 1 - m_playersTurn;
            m_turnAddCountdown = 1;
        }

        // -- Aim logic --
        if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_LEFT))
        {
            m_dragStartPos = glm::vec2(input->getMouseX(), input->getMouseY());
            m_dragging = true;
        }

        m_dragVector = m_dragStartPos - glm::vec2(input->getMouseX(), input->getMouseY());
        
        if (m_dragging) // currently dragging
        {
            float rotation = atan2f(m_dragVector.y, m_dragVector.x) / (2.f * Pi);
            m_cueBall->SetOrientation(rotation * 360.f);

            if(input->wasMouseButtonReleased(aie::INPUT_MOUSE_BUTTON_LEFT))
            {
                // Drag ended, do some logic
                m_turnAddCountdown--;
                m_firstHit = nullptr;
                m_dragging = false;
                m_potted = false;

                float maxForce = sqrtf(m_windowPixelSize.x * m_windowPixelSize.x +
                                    m_windowPixelSize.y * m_windowPixelSize.y) * 0.65f;

                if (length(m_dragVector) > maxForce)
                    m_dragVector = normalize(m_dragVector) * maxForce;
                
                m_cueBall->ApplyForce(m_dragVector, glm::vec2(0));
                m_cueBall->SetColor(glm::vec4(0.75f));
            }
        }
    }
}

void Pool_Table::Draw()
{
    PhysicsScene::Draw();

    if (m_dragging) // Draw aim line
    {
        glm::vec2 dragStart = PixelToViewSpace(m_dragStartPos);
        glm::vec2 mousePos = PixelToViewSpace(m_dragStartPos - m_dragVector);

        float maxForce = sqrtf(m_windowPixelSize.x * m_windowPixelSize.x +
                                m_windowPixelSize.y * m_windowPixelSize.y) * 0.65f;

        float forcePercent = length(m_dragVector) / maxForce;

        glm::vec4 color1 = glm::vec4(0, 0, 1, 1);
        glm::vec4 color2 = glm::vec4(1, 0, 0, 1);
            
        glm::vec4 finalColor = glm::vec4
        {
            color1.r + (color2.r - color1.r) * forcePercent,
            color1.g + (color2.g - color1.b) * forcePercent,
            color1.b + (color2.g - color1.b) * forcePercent,
            1
        };

        glm::vec2 dragVector = mousePos - dragStart;
            
        aie::Gizmos::add2DLine(dragStart, mousePos, finalColor);
        aie::Gizmos::add2DLine(m_cueBall->GetPosition(), m_cueBall->GetPosition() - dragVector, glm::vec4(1));
    }

    // Draw UI
    // Team 1 counter
    auto GetTeamCol = [this] (Billiard::BilliardType _type)
    {
        for (int i = 0; i < m_actors.size(); i++)
        {
            Billiard* ball = dynamic_cast<Billiard*>(m_actors.at(i));

            if (ball != nullptr && ball->billiardType == _type)
                return ball->GetColor();
        }

        return glm::vec4(0);
    };
    
    glm::vec4 team1Col = GetTeamCol(m_team1Type);
    glm::vec4 team2Col = GetTeamCol(m_team2Type);

    glm::vec2 counterPos = glm::vec2(80, 50);
    float radius = 3.5f;
    
    for (int i = 0; i < m_colorTotal - m_team1Counter; i++)
    {
        aie::Gizmos::add2DCircle(glm::vec2(-counterPos.x, counterPos.y) +
            glm::vec2(radius * i * 2, 0), radius, 12, team1Col, false);
    }

    for (int i = 0; i < m_colorTotal - m_team2Counter; i++)
    {
        aie::Gizmos::add2DCircle(counterPos - glm::vec2(radius * i * 2, 0),
                                    radius, 12, team2Col, false);
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
        const float radius = 2.75f;
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
    m_colorTotal = (int)floor(ballTotal * 0.5f);

    // Because of the recursion algorithm to make the triangle,
    // this is the only way to get the exact row amount
    int rows = (int)floor((-1 + sqrt(1 - 4 * 1 * (-2 * _balls.size()))) / 2);

    for (int i = 0; i < (int)_balls.size(); i++)
    {
        // Set random color, out of 2 passed in colors
        glm::vec4 ballColor = (rand() % 2 == 0) ? _color1 : _color2;

        // If too much of colorX, set it to colorY
        ballColor = color1 >= m_colorTotal ? _color2 : ballColor;
        ballColor = color2 >= m_colorTotal ? _color1 : ballColor;

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

void Pool_Table::MakePoolTable(glm::vec2 _tableCenterOffset, glm::vec2 _tableExtents, bool _showTriggers)
{
    // Boxes for the inner table edges
    const glm::vec4 innerEdgeColor = glm::vec4(0, 0.7, 0, 1);

    float cornerRadius = 13.5f, centreRadius = 9.f, edgeWidth = 5.f;
    float halfXSize = (_tableExtents.x - cornerRadius - centreRadius) * 0.5f;

    // Loop to make top and bottom edges
    for (int i = 0; i < 4; i++)
    {
        glm::vec2 boxPos = glm::vec2((centreRadius + halfXSize) * (i % 3 == 0 ? -1.f : 1.f),
                                    _tableExtents.y * (i % 2 == 0 ? -1.f : 1.f));
        glm::vec2 boxSize = glm::vec2(halfXSize, edgeWidth);
        glm::vec2 circlePos = boxPos + glm::vec2(boxSize.x, 0) * (i % 2 == 0 ? -1.f : 1.f);

        AddActor(new Box(_tableCenterOffset + boxPos, glm::vec2(), 1, boxSize, 0, innerEdgeColor));
        AddActor(new Circle(_tableCenterOffset + circlePos, glm::vec2(), 1, edgeWidth, innerEdgeColor));
        AddActor(new Circle(_tableCenterOffset + glm::vec2(-circlePos.x, circlePos.y), glm::vec2(), 1, edgeWidth, innerEdgeColor));
    }

    // Loop to make left and right edges
    for (int i = 0; i < 2; i++)
    {
        glm::vec2 boxPos = glm::vec2(_tableExtents.x * (i % 2 == 0 ? -1.f : 1.f), 0);
        glm::vec2 boxSize = glm::vec2(edgeWidth, _tableExtents.y - cornerRadius);
        glm::vec2 circlePos = boxPos + glm::vec2(0, boxSize.y) * (i % 2 == 0 ? -1.f : 1.f);

        AddActor(new Box(_tableCenterOffset + boxPos, glm::vec2(), 1, boxSize, 0, innerEdgeColor));
        AddActor(new Circle(_tableCenterOffset + circlePos, glm::vec2(), 1, edgeWidth, innerEdgeColor));
        AddActor(new Circle(_tableCenterOffset + glm::vec2(circlePos.x, -circlePos.y), glm::vec2(), 1, edgeWidth, innerEdgeColor));
    }

    // Loop for setting edge kinematic and elasticity
    for (int i = 0; i < 18; i++)
    {
        dynamic_cast<Rigidbody*>(m_actors.at(m_actors.size() - 1 - i))->SetKinematic(true);
        m_actors.at(m_actors.size() - 1 - i)->SetElasticity(0.6f);
    }

    // Boxes and circles for the outer table edges
    const glm::vec4 outerEdgeColor = glm::vec4(0.36, 0.17, 0.15, 1);
    float outerEdgeWidth = 3.5f;
    float halfOuterWidth = outerEdgeWidth * 0.5f;
    
    AddActor(new Box(_tableCenterOffset + glm::vec2(0, _tableExtents.y + halfOuterWidth), glm::vec2(0), 1.f, glm::vec2(_tableExtents.x, outerEdgeWidth), 0, outerEdgeColor));
    AddActor(new Box(_tableCenterOffset + glm::vec2(0, -_tableExtents.y - halfOuterWidth), glm::vec2(0), 1.f, glm::vec2(_tableExtents.x, outerEdgeWidth), 0, outerEdgeColor));
    AddActor(new Box(_tableCenterOffset + glm::vec2(_tableExtents.x + halfOuterWidth, 0), glm::vec2(0), 1.f, glm::vec2(outerEdgeWidth, _tableExtents.y), 0, outerEdgeColor));
    AddActor(new Box(_tableCenterOffset + glm::vec2(-_tableExtents.x - halfOuterWidth, 0), glm::vec2(0), 1.f, glm::vec2(outerEdgeWidth, _tableExtents.y), 0, outerEdgeColor));

    AddActor(new Circle(_tableCenterOffset + glm::vec2(-_tableExtents.x, _tableExtents.y), glm::vec2(0), 1.f, 1.5f * outerEdgeWidth, outerEdgeColor));
    AddActor(new Circle(_tableCenterOffset + glm::vec2(_tableExtents.x, _tableExtents.y), glm::vec2(0), 1.f, 1.5f * outerEdgeWidth, outerEdgeColor));
    AddActor(new Circle(_tableCenterOffset + glm::vec2(-_tableExtents.x, -_tableExtents.y), glm::vec2(0), 1.f, 1.5f * outerEdgeWidth, outerEdgeColor));
    AddActor(new Circle(_tableCenterOffset + glm::vec2(_tableExtents.x, -_tableExtents.y), glm::vec2(0), 1.f, 1.5f * outerEdgeWidth, outerEdgeColor));

    for (int i = 0; i < 8; i++) // set outer edges to triggers
    {
        Rigidbody* rb = dynamic_cast<Rigidbody*>(m_actors.at(m_actors.size() - 1 - i));
        
        rb->SetTrigger(true);
    }
    
    // Pockets
    glm::vec4 pocketColor = glm::vec4(0.2f, 0.2f, 0.2f, 1);
    float pocketRadius = centreRadius - edgeWidth;
    
    glm::vec4 triggerColor = glm::vec4(1, 0, 0, _showTriggers);
    float triggerRadius = pocketRadius * 1.5f;
    
    glm::vec2 cornerPos = glm::vec2(_tableExtents.x, _tableExtents.y) - edgeWidth;

    // Loop for making the 3 pockets (left, mid, right) for top + bottom
    for (int i = 0; i < 2; i++)
    {
        float multi = i % 2 == 0 ? 1.f : -1.f;
        
        AddActor(new Circle(_tableCenterOffset + glm::vec2(-cornerPos.x, cornerPos.y) * multi, glm::vec2(), 1, pocketRadius, pocketColor));
        AddActor(new Circle(_tableCenterOffset + glm::vec2(_tableCenterOffset.x, _tableExtents.y * multi), glm::vec2(), 1, pocketRadius, pocketColor));
        AddActor(new Circle(_tableCenterOffset + cornerPos * multi, glm::vec2(), 1, pocketRadius, pocketColor));
    }

    for (int i = 0; i < 2; i++)
    {
        float multi = i % 2 == 0 ? 1.f : -1.f;
        
        AddActor(new Circle(_tableCenterOffset + glm::vec2(-_tableExtents.x, _tableExtents.y) * multi, glm::vec2(), 1, triggerRadius, triggerColor));
        AddActor(new Circle(_tableCenterOffset + glm::vec2(0, (_tableExtents.y + edgeWidth) * multi), glm::vec2(), 1, triggerRadius, triggerColor));
        AddActor(new Circle(_tableCenterOffset + _tableExtents * multi, glm::vec2(), 1, triggerRadius, triggerColor));
    }

    // Loop for setting the pockets to triggers and adding a callback
    for (int i = 0; i < 12; i++)
    {
        Rigidbody* rb = dynamic_cast<Rigidbody*>(m_actors.at(m_actors.size() - 1 - i));
        
        rb->SetTrigger(true);

        if (i < 6) rb->triggerEnter = std::bind(&Pool_Table::PocketEnter, this, std::placeholders::_1);
    }
}

void Pool_Table::PocketEnter(PhysicsObject* _other)
{
    Billiard* billiard = dynamic_cast<Billiard*>(_other);
    
    if (billiard) // if object is of type 'Billiard'
    {
        Billiard::BilliardType type = billiard->billiardType;

        // Eight ball went into pocket
        if (type == Billiard::EightBall)
        {
            m_runEndgame = true;

            // Cue ball was NOT reset, so eight ball went in first
            if (m_cueBall->GetPosition() != m_cueBallStartPos)
                m_eightBallFirst = true;
        }
        
        // Cue ball went into pocket
        if (type == Billiard::CueBall)
        {
            // Extra turn for the other team, reset the cue ball
            ExtraTurn();

            m_cueBall->SetPosition(m_cueBallStartPos);
            m_cueBall->SetVelocity(glm::vec2(0));
            m_firstHit = m_cueBall;
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
                        m_team1Counter += temp->billiardType == m_team1Type;
                        m_team2Counter += temp->billiardType == m_team2Type;
                    }
                }
            }
            
            // Decrement the right team counter
            m_team1Counter -= type == m_team1Type;
            m_team2Counter -= type == m_team2Type;
            
            // If the player got one of their own in, keep it their turn
            if (!m_potted)
            {
                if ((m_playersTurn == 0 && type == m_team1Type) ||
                (m_playersTurn == 1 && type == m_team2Type))
                {
                    m_turnAddCountdown = 1;
                    m_potted = true;
                }
            }

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
        if ((m_playersTurn == 0 && m_firstHit->billiardType == m_team2Type) ||
            (m_playersTurn == 1 && m_firstHit->billiardType == m_team1Type))
        {
            ExtraTurn();
        }

        // The first hit was of type eight ball, and team still has balls left
        else if (m_firstHit->billiardType == Billiard::EightBall)
        {
            if ((m_playersTurn == 0 && m_team1Counter != 0) ||
                (m_playersTurn == 1 && m_team2Counter != 0))
            {
                ExtraTurn();
            }
        }
    }
}

void Pool_Table::EndGame(int _winningTeam)
{
    // Remove all billiards from the scene
    for (int i = m_actors.size() - 1; i >= 0; i--)
    {
        if (dynamic_cast<Billiard*>(m_actors.at(i)))
        {
            RemoveActor(m_actors.at(i));
        }
    }
}