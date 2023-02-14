#include "Pool_Table.h"

#include <Gizmos.h>
#include <Input.h>
#include <Renderer2D.h>
#include <glm/ext.hpp>

void Pool_Table::Startup(aie::Application* _app)
{
    glm::vec2 tableOffset = glm::vec2(0, -5);
    glm::vec2 tableExtents = m_windowExtents * 0.75f;
    
    // Load assets
    m_font = new aie::Font("./font/consolas.ttf", 32);
    aie::Texture* tempTable = new aie::Texture("./textures/table.png");
    m_table = new Sprite(tempTable, ViewToPixelSpacePos(tableOffset), ViewToPixelSpacePos(tableExtents));

    // Pool table colliders and pocket triggers
    MakePoolTable(tableOffset, tableExtents);
    
    // Cue ball
    m_cueBallStartPos = tableOffset + glm::vec2(-47.5f, 0);
    m_cueBall = new Billiard(m_cueBallStartPos, glm::vec2(0), 2.f, 1.85f, glm::vec4(1), 0.9f);
    m_cueBall->SetLinearDrag(0.8f);
    m_cueBall->collisionCallback = std::bind(&Pool_Table::CueBallCollision, this, std::placeholders::_1);
    AddActor(m_cueBall);
    
    // Prevents errors later on
    m_firstHit = m_cueBall;

    // Makes triangle of billiards
    MakeTriangle(tableOffset + glm::vec2(32.f, 0), 4.5f);
}

void Pool_Table::Update(float _dt)
{
    PhysicsScene::Update(_dt);
    
    aie::Input* input = aie::Input::getInstance();

    // Check if any of the balls are off the screen and count it as being pocketed
    for (int i = m_actors.size() - 1; i >= 0; i--)
    {
        Billiard* ball = dynamic_cast<Billiard*>(m_actors.at(i));

        if (ball != nullptr)
        {
            if (ball->GetPosition().x >= m_windowExtents.x || ball->GetPosition().x <= -m_windowExtents.x ||
                ball->GetPosition().y >= m_windowExtents.y || ball->GetPosition().y <= -m_windowExtents.y)
            {
                PocketEnter(ball);
            }
        }
    }

    if (m_cueBall->GetVelocity() == glm::vec2(0))
    {
        // Check if all balls have stopped moving
        if (!AllBallsStopped()) return;
        
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
                                    m_windowPixelSize.y * m_windowPixelSize.y) * 0.5f;

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
    for (int i = 0; i < m_actors.size(); i++)
    {
        Billiard* ball = dynamic_cast<Billiard*>(m_actors.at(i));

        if (ball != nullptr && ball->billiardSprite != nullptr)
        {
            ball->billiardSprite->position = ViewToPixelSpacePos(ball->GetPosition());
        }
    }
    
    // Draw table background texture
    m_renderer2D->begin();
    
    m_renderer2D->setUVRect(0,0,1,1);
    m_renderer2D->drawSprite(m_table->texture, m_table->position.x, m_table->position.y,
                             m_table->size.x, m_table->size.y);

    m_renderer2D->end();
    
    PhysicsScene::Draw();

    if (m_dragging) // Draw aim line
    {
        glm::vec2 dragStart = PixelToViewSpacePos(m_dragStartPos);
        glm::vec2 mousePos = PixelToViewSpacePos(m_dragStartPos - m_dragVector);

        float maxForce = sqrtf(m_windowPixelSize.x * m_windowPixelSize.x +
                                m_windowPixelSize.y * m_windowPixelSize.y) * 0.5f;

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
}

void Pool_Table::MakeTriangle(glm::vec2 _startPos, float _spacing)
{
    float rads = DegreeToRadian(30);
    glm::vec2 offset = glm::vec2(cosf(rads), sinf(rads)) * _spacing;
    
    float mass = m_cueBall->GetMass() * 1.25f;
    float radius = m_cueBall->GetRadius() * 1.1f;
    
    AddActor(new Billiard(_startPos, glm::vec2(0), mass, radius, glm::vec4()));
    
    AddActor(new Billiard(_startPos + offset, glm::vec2(0), mass, radius, glm::vec4()));
    AddActor(new Billiard(_startPos + glm::vec2(offset.x, -offset.y), glm::vec2(0), mass, radius, glm::vec4()));

    AddActor(new Billiard(_startPos + glm::vec2(2.f * offset.x, 2.f * offset.y), glm::vec2(0), mass, radius, glm::vec4()));
    AddActor(new Billiard(_startPos + glm::vec2(2.f * offset.x, 0), glm::vec2(0), mass, radius, glm::vec4()));
    AddActor(new Billiard(_startPos + glm::vec2(2.f * offset.x, 2.f * -offset.y), glm::vec2(0), mass, radius, glm::vec4()));

    AddActor(new Billiard(_startPos + glm::vec2(3.f * offset.x, 3.f * offset.y), glm::vec2(0), mass, radius, glm::vec4()));
    AddActor(new Billiard(_startPos + glm::vec2(3.f * offset.x, offset.y), glm::vec2(0), mass, radius, glm::vec4()));
    AddActor(new Billiard(_startPos + glm::vec2(3.f * offset.x, -offset.y), glm::vec2(0), mass, radius, glm::vec4()));
    AddActor(new Billiard(_startPos + glm::vec2(3.f * offset.x, 3.f * -offset.y), glm::vec2(0), mass, radius, glm::vec4()));

    AddActor(new Billiard(_startPos + glm::vec2(4.f * offset.x, 4.f * offset.y), glm::vec2(0), mass, radius, glm::vec4()));
    AddActor(new Billiard(_startPos + glm::vec2(4.f * offset.x, 2.f * offset.y), glm::vec2(0), mass, radius, glm::vec4()));
    AddActor(new Billiard(_startPos + glm::vec2(4.f * offset.x, 0), glm::vec2(0), mass, radius, glm::vec4()));
    AddActor(new Billiard(_startPos + glm::vec2(4.f * offset.x, 2.f * -offset.y), glm::vec2(0), mass, radius, glm::vec4()));
    AddActor(new Billiard(_startPos + glm::vec2(4.f * offset.x, 4.f * -offset.y), glm::vec2(0), mass, radius, glm::vec4()));

    // Set billiard type, elasticity and drag
    for (int i = 0; i < 15; i++)
    {
        Billiard* ball = dynamic_cast<Billiard*>(m_actors.at(m_actors.size() - 1 - i));

        std::string textureLocation = "./textures/ball" + std::to_string(15 - i) + ".png";
        aie::Texture* ballTexture = new aie::Texture(textureLocation.c_str());
        
        ball->billiardSprite = new Sprite(ballTexture, glm::vec2(),
            glm::vec2(ViewToPixelSpacePos(glm::vec2(ball->GetRadius() - m_windowExtents.x)).x * 2.f));
        ball->renderer2D = m_renderer2D;
        
        // Even - stripe
        if (i % 2 == 0)
            ball->billiardType = Billiard::ColorBall1;

        // Odd - solid
        if (i % 2 == 1)
            ball->billiardType = Billiard::ColorBall2;

        // 4 - eight ball
        if (i == 4)
            ball->billiardType = Billiard::EightBall;

        ball->SetElasticity(0.8f);
        ball->SetLinearDrag(0.9f);
    }
}

void Pool_Table::MakePoolTable(glm::vec2 _tableCenterOffset, glm::vec2 _tableExtents, bool _showBounds)
{
    // -- Edge colliders --
    glm::vec2 collPos = _tableCenterOffset + glm::vec2(-40.f, 46.5f);
    glm::vec2 collSize = glm::vec2(36.f, 5);

    // Top
    AddActor(new Box(collPos, glm::vec2(0), 1.f, collSize, 0, glm::vec4(_showBounds)));
    AddActor(new Box(collPos + glm::vec2(-collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    AddActor(new Box(collPos + glm::vec2(collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    
    collPos.x *= -1.f;
        
    AddActor(new Box(collPos, glm::vec2(0), 1.f, collSize, 0, glm::vec4(_showBounds)));
    AddActor(new Box(collPos + glm::vec2(-collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    AddActor(new Box(collPos + glm::vec2(collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    
    // Bottom
    collPos = _tableCenterOffset - glm::vec2(-40.f, 46.5f);
    
    AddActor(new Box(collPos, glm::vec2(0), 1.f, collSize, 0, glm::vec4(_showBounds)));
    AddActor(new Box(collPos - glm::vec2(-collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    AddActor(new Box(collPos - glm::vec2(collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    
    collPos.x *= -1.f;
        
    AddActor(new Box(collPos, glm::vec2(0), 1.f, collSize, 0, glm::vec4(_showBounds)));
    AddActor(new Box(collPos - glm::vec2(-collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    AddActor(new Box(collPos - glm::vec2(collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    
    // Left
    collPos = _tableCenterOffset + glm::vec2(-85, 0);
    collSize = glm::vec2(5, 38.f);
    
    AddActor(new Box(collPos, glm::vec2(0), 1.f, collSize, 0, glm::vec4(_showBounds)));
    AddActor(new Box(collPos + glm::vec2(-collSize.x / 2.f, collSize.y), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    AddActor(new Box(collPos + glm::vec2(-collSize.x / 2.f, -collSize.y), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    
    // Right
    collPos.x *= -1.f;
    
    AddActor(new Box(collPos, glm::vec2(0), 1.f, collSize, 0, glm::vec4(_showBounds)));
    AddActor(new Box(collPos - glm::vec2(-collSize.x / 2.f, collSize.y), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    AddActor(new Box(collPos - glm::vec2(-collSize.x / 2.f, -collSize.y), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    
    // Set colliders to kinematic
    for (int i = 0; i < 18; i++)
    {
        Rigidbody* rb = dynamic_cast<Rigidbody*>(m_actors.at(m_actors.size() - 1 - i));
        rb->SetKinematic(true);
    }
    
    // -- Pocket Triggers --
    for (int i = 0; i < 2; i++)
    {
        glm::vec4 triggerColor = glm::vec4(1, 0, 0, _showBounds);
        float triggerRadius = 7.f;

        glm::vec2 cornerPos = glm::vec2(_tableExtents.x + 10.f, _tableExtents.y + 4.5f);
        
        float multi = i % 2 == 0 ? 1.f : -1.f;
        
        AddActor(new Circle(_tableCenterOffset + glm::vec2(-cornerPos.x, cornerPos.y) * multi, glm::vec2(), 1, triggerRadius, triggerColor));
        AddActor(new Circle(_tableCenterOffset + glm::vec2(0, (_tableExtents.y + 7.5f) * multi), glm::vec2(), 1, triggerRadius, triggerColor));
        AddActor(new Circle(_tableCenterOffset + cornerPos * multi, glm::vec2(), 1, triggerRadius, triggerColor));

        for (int j = 0; j < 3; j++) // set them to triggers, and add a callback
        {
            Circle* pocketTrigger = dynamic_cast<Circle*>(m_actors.at(m_actors.size() - 1 - j));
        
            pocketTrigger->SetTrigger(true);
            pocketTrigger->triggerEnter = std::bind(&Pool_Table::PocketEnter, this, std::placeholders::_1);
        }
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

            RemoveActor(billiard);
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
            // First team ball sunk, set team types and add to team vectors
            if (m_team1Type == Billiard::Null || m_team2Type == Billiard::Null)
            {
                Billiard::BilliardType otherType = type == Billiard::ColorBall1 ? Billiard::ColorBall2 : Billiard::ColorBall1;

                m_team1Type = m_playersTurn == 0 ? type : otherType;
                m_team2Type = m_playersTurn == 1 ? type : otherType;
                
                // Fill the teams vectors
                for (int i = 0; i < m_actors.size(); i++)
                {
                    Billiard* temp = dynamic_cast<Billiard*>(m_actors.at(i));
                    
                    if (temp != nullptr)
                    {
                        if (temp->billiardType == m_team1Type)
                            m_team1.push_back(temp);

                        else m_team2.push_back(temp);
                    }
                }
            }
            
            // Remove from the teams vector
            auto team1Found = std::find(m_team1.begin(), m_team1.end(), billiard);
            if (team1Found != m_team1.end()) m_team1.erase(team1Found);

            auto team2Found = std::find(m_team2.begin(), m_team2.end(), billiard);
            if (team2Found != m_team2.end()) m_team2.erase(team2Found);
            
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
            if ((m_playersTurn == 0 && !m_team1.empty()) ||
                (m_playersTurn == 1 && !m_team2.empty()))
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