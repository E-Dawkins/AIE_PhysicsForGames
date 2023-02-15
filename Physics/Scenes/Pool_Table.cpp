#include "Pool_Table.h"

#include <Gizmos.h>
#include <Input.h>
#include <Renderer2D.h>
#include <glm/ext.hpp>

void Pool_Table::Startup(aie::Application* _app)
{
    // Initiate member variables
    m_cueBallSunk = false;
    m_team1Type = Billiard::Null;
    m_team2Type = Billiard::Null;
    m_playersTurn = 0;
    m_turnAddCountdown = 1;
    m_runEndgame = false;
    m_eightBallFirst = false;
    m_dragStartPos = glm::vec2(0);
    m_dragVector = glm::vec2(0);
    m_dragging = false;
    m_potted = false;
    m_winner = 0;
    
    // Load assets
    m_fontSmall = new aie::Font("./font/consolas_bold.ttf", 16);
    m_fontBig = new aie::Font("./font/consolas_bold.ttf", 32);
    aie::Texture* tempTable = new aie::Texture("./textures/table.png");
    m_table = new Sprite(tempTable, ViewToPixelSpacePos(glm::vec2(0, -5)),
                                    ViewToPixelSpacePos(m_windowExtents * 0.75f));

    // Pool table colliders and pocket triggers
    MakePoolTable();
    
    // Cue ball
    m_cueBall = new Billiard(glm::vec2(-47.5f, -5), glm::vec2(0), 2.f, 1.85f, glm::vec4(0), 0.9f);

    aie::Texture* cueBallTexture = new aie::Texture("./textures/ball16.png");
    m_cueBall->billiardSprite = new Sprite(cueBallTexture, glm::vec2(),
            glm::vec2(ViewToPixelSpacePos(glm::vec2(m_cueBall->GetRadius() - m_windowExtents.x)).x * 2.f));
    m_cueBall->renderer2D = m_renderer2D;
    
    m_cueBall->SetLinearDrag(0.8f);
    m_cueBall->collisionCallback = std::bind(&Pool_Table::CueBallCollision, this, std::placeholders::_1);
    AddActor(m_cueBall);
    
    // Prevents errors later on
    m_firstHit = m_cueBall;

    // Makes triangle of billiards
    MakeTriangle(glm::vec2(32.f, -5), 4.5f);
}

void Pool_Table::Shutdown()
{
    delete m_cueBall;
    m_team1.clear();
    m_team2.clear();
    delete m_firstHit;
    delete m_fontSmall;
    delete m_fontBig;
    delete m_table;
    m_fadingTexts.clear();
}

void Pool_Table::Update(float _dt)
{
    PhysicsScene::Update(_dt);
    
    aie::Input* input = aie::Input::getInstance();

    if(m_winner != 0)
    {
        if (input->wasKeyPressed(aie::INPUT_KEY_R))
        {
            // Remove all actors
            for (int i = m_actors.size() - 1; i >= 0; i--)
            {
                RemoveActor(m_actors.at(i));
            }

            Shutdown();
            Startup(nullptr);
        }

        return;
    }
    
    CheckFadingTexts();

    for (auto text : m_fadingTexts)
        text->UpdateText(_dt);

    m_cueBall->SetTrigger(m_cueBallSunk);
    
    if (m_cueBallSunk) // move the cue ball logic
    {
        glm::vec2 mousePos = PixelToViewSpacePos(glm::vec2(input->getMouseX(), input->getMouseY()));
        m_cueBall->SetPosition(mousePos);

        glm::vec2 cueBallPos = m_cueBall->GetPosition();

        // Placeable area; pos = (-62.5, -5), extents = (15.f, 39.f)
        if (cueBallPos.x <= -77.5f) m_cueBall->SetPosition(glm::vec2(-77.5f, m_cueBall->GetPosition().y));
        if (cueBallPos.x >= -47.5f) m_cueBall->SetPosition(glm::vec2(-47.5f, m_cueBall->GetPosition().y));
        if (cueBallPos.y <= -44.f) m_cueBall->SetPosition(glm::vec2(m_cueBall->GetPosition().x, -44.f));
        if (cueBallPos.y >= 34.f) m_cueBall->SetPosition(glm::vec2(m_cueBall->GetPosition().x, 34.f));

        // Place cue ball
        if (input->wasMouseButtonPressed(aie::INPUT_MOUSE_BUTTON_RIGHT))
        {
            m_cueBallSunk = false;
            m_cueBall->SetTrigger(false);
        }

        return;
    }

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
            EndGame();
            return;
        }
        
        // Didn't hit anything last time, extra turn
        if (m_firstHit == nullptr)
        {
            ExtraTurn();
            m_firstHit = m_cueBall;
            AddFadingText("Foul", ViewToPixelSpacePos(m_cueBall->GetPosition()) + 10.f);
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
                m_cueBall->SetColor(glm::vec4(0.85f));
            }
        }
    }
}

void Pool_Table::Draw()
{
    // Update the sprites to be on the balls position
    for (int i = 0; i < m_actors.size(); i++)
    {
        Billiard* ball = dynamic_cast<Billiard*>(m_actors.at(i));

        if (ball != nullptr && ball->billiardSprite != nullptr)
        {
            ball->billiardSprite->position = ViewToPixelSpacePos(ball->GetPosition());
        }
    }
    
    // Draw background elements
    m_renderer2D->begin();
    
    m_renderer2D->setUVRect(0,0,1,1);
    m_renderer2D->drawSprite(m_table->texture, m_table->position.x, m_table->position.y,
                             m_table->size.x, m_table->size.y);
    
    m_renderer2D->end();

    // Draw physics objects
    aie::Gizmos::clear();
    PhysicsScene::Draw();
    
    // Draw aim line
    if (m_dragging)
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

    // UI elements
    m_renderer2D->begin();

    glm::vec4 prevCol = m_renderer2D->getRenderColour();
    
    m_renderer2D->setRenderColour(1, 1, 0, 1);
    glm::vec2 turnIndicatorPos = glm::vec2(m_windowPixelSize.x * 0.05f, m_windowPixelSize.y * 0.94f);
    turnIndicatorPos.x = m_playersTurn == 0 ? m_windowPixelSize.x * 0.05f : m_windowPixelSize.x * 0.95f;
    m_renderer2D->drawCircle(turnIndicatorPos.x, turnIndicatorPos.y, 10.f);
    m_renderer2D->setRenderColour(prevCol.r, prevCol.g, prevCol.b, prevCol.a);
    
    m_renderer2D->drawText(m_fontBig, "P1", m_windowPixelSize.x * 0.075f, m_windowPixelSize.y * 0.925f);
    m_renderer2D->drawText(m_fontBig, "P2", m_windowPixelSize.x * 0.895f, m_windowPixelSize.y * 0.925f);
    
    if (m_cueBallSunk)
    {
        glm::vec2 cueBallPos = ViewToPixelSpacePos(m_cueBall->GetPosition()) + 10.f;
        m_renderer2D->drawText(m_fontSmall, "Place with RMB", cueBallPos.x, cueBallPos.y);
    }

    for (auto text : m_fadingTexts)
        text->DrawText();

    if (m_winner != 0) // draw text displaying winner
    {
        prevCol = m_renderer2D->getRenderColour();

        m_renderer2D->setRenderColour(0.3f, 0.3f, 0.3f, 0.75f);
        m_renderer2D->drawBox(m_windowPixelSize.x * 0.5f, m_windowPixelSize.y * 0.465f, 350, 100);
        m_renderer2D->setRenderColour(prevCol.r, prevCol.g, prevCol.b, prevCol.a);
        
        std::string winnerText = "Player " + std::to_string(m_winner) + " won!";
        m_renderer2D->drawText(m_fontBig, winnerText.c_str(), m_windowPixelSize.x * 0.415f, m_windowPixelSize.y * 0.475f);

        m_renderer2D->drawText(m_fontBig, "Press 'r' to reset", m_windowPixelSize.x * 0.375f, m_windowPixelSize.y * 0.425f);
    }

    m_renderer2D->end();
}

void Pool_Table::MakeTriangle(glm::vec2 _startPos, float _spacing)
{
    float rads = DegreeToRadian(30);
    glm::vec2 offset = glm::vec2(cosf(rads), sinf(rads)) * _spacing;
    
    float mass = m_cueBall->GetMass() * 1.25f;
    float radius = m_cueBall->GetRadius() * 1.1f;

    // Solids
    AddActor(new Billiard(_startPos, glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(offset.x, -offset.y), glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(3.f * offset.x, 3.f * offset.y), glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(3.f * offset.x, -offset.y), glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(3.f * offset.x, 3.f * -offset.y), glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(4.f * offset.x, 2.f * offset.y), glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(4.f * offset.x, 2.f * -offset.y), glm::vec2(0), mass, radius));
    
    // Eight Ball
    AddActor(new Billiard(_startPos + glm::vec2(2.f * offset.x, 0), glm::vec2(0), mass, radius));
    
    // Stripes
    AddActor(new Billiard(_startPos + offset, glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(2.f * offset.x, 2.f * offset.y), glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(2.f * offset.x, 2.f * -offset.y), glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(3.f * offset.x, offset.y), glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(4.f * offset.x, 4.f * offset.y), glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(4.f * offset.x, 0), glm::vec2(0), mass, radius));
    AddActor(new Billiard(_startPos + glm::vec2(4.f * offset.x, 4.f * -offset.y), glm::vec2(0), mass, radius));

    // Set billiard type, elasticity and drag
    for (int i = 0; i < 15; i++)
    {
        Billiard* ball = dynamic_cast<Billiard*>(m_actors.at(m_actors.size() - 1 - i));

        std::string textureLocation = "./textures/ball" + std::to_string(15 - i) + ".png";
        aie::Texture* ballTexture = new aie::Texture(textureLocation.c_str());
        
        ball->billiardSprite = new Sprite(ballTexture, glm::vec2(),
            glm::vec2(ViewToPixelSpacePos(glm::vec2(ball->GetRadius() - m_windowExtents.x)).x * 2.f));
        ball->renderer2D = m_renderer2D;
        
        // 1 -> 7 - solids
        if ((15 - i) <= 7)
            ball->billiardType = Billiard::Solids;

        // 8 -> 15 - stripes
        if ((15 - i) > 8)
            ball->billiardType = Billiard::Stripes;

        // 8 - eight ball
        if ((15 - i) == 8)
            ball->billiardType = Billiard::EightBall;

        ball->SetElasticity(0.8f);
        ball->SetLinearDrag(0.9f);
    }
}

void Pool_Table::MakePoolTable(bool _showBounds)
{
    // -- Edge colliders --
    glm::vec2 collPos = glm::vec2(-40.f, 41.5f);
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
    collPos = glm::vec2(-40.f, -51.5f);
    
    AddActor(new Box(collPos, glm::vec2(0), 1.f, collSize, 0, glm::vec4(_showBounds)));
    AddActor(new Box(collPos - glm::vec2(-collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    AddActor(new Box(collPos - glm::vec2(collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    
    collPos.x *= -1.f;
        
    AddActor(new Box(collPos, glm::vec2(0), 1.f, collSize, 0, glm::vec4(_showBounds)));
    AddActor(new Box(collPos - glm::vec2(-collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    AddActor(new Box(collPos - glm::vec2(collSize.x, collSize.y / 2.f), glm::vec2(0), 1.f, glm::vec2(5), DegreeToRadian(45), glm::vec4(_showBounds)));
    
    // Left
    collPos = glm::vec2(-85, -5);
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
    float triggerRadius = 7.f;

    // Top
    glm::vec2 cornerPos = m_windowExtents * 0.75f + glm::vec2(10.5f, -0.5f);
    AddActor(new Circle(glm::vec2(-cornerPos.x, cornerPos.y), glm::vec2(), 1.f, triggerRadius, glm::vec4(_showBounds)));
    AddActor(new Circle(glm::vec2(cornerPos.x, cornerPos.y), glm::vec2(), 1.f, triggerRadius, glm::vec4(_showBounds)));
    AddActor(new Circle(glm::vec2(0, cornerPos.y + 3.5f), glm::vec2(), 1.f, triggerRadius, glm::vec4(_showBounds)));

    // Bottom
    cornerPos.y = -cornerPos.y - 10.f;
    AddActor(new Circle(glm::vec2(-cornerPos.x, cornerPos.y), glm::vec2(), 1.f, triggerRadius, glm::vec4(_showBounds)));
    AddActor(new Circle(glm::vec2(cornerPos.x, cornerPos.y), glm::vec2(), 1.f, triggerRadius, glm::vec4(_showBounds)));
    AddActor(new Circle(glm::vec2(0, cornerPos.y - 3.5f), glm::vec2(), 1.f, triggerRadius, glm::vec4(_showBounds)));
    
    for (int i = 0; i < 6; i++) // set them to triggers, and add a callback
    {
        Circle* pocketTrigger = dynamic_cast<Circle*>(m_actors.at(m_actors.size() - 1 - i));
        
        pocketTrigger->SetTrigger(true);
        pocketTrigger->triggerEnter = std::bind(&Pool_Table::PocketEnter, this, std::placeholders::_1);
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

            // Cue ball was NOT sunk, so eight ball went in first
            m_eightBallFirst = !m_cueBallSunk;

            RemoveActor(billiard);
        }
        
        // Cue ball went into pocket
        if (type == Billiard::CueBall)
        {
            // Extra turn for the other team, reset the cue ball
            ExtraTurn();
            
            m_cueBall->SetVelocity(glm::vec2(0));
            m_firstHit = m_cueBall;

            m_cueBallSunk = true;

            AddFadingText("Foul", ViewToPixelSpacePos(m_cueBall->GetPosition()) + 10.f);
        }

        // Team ball went into pocket
        if (type == Billiard::Solids || type == Billiard::Stripes)
        {
            // First team ball sunk, set team types and add to team vectors
            if (m_team1Type == Billiard::Null || m_team2Type == Billiard::Null)
            {
                Billiard::BilliardType otherType = type == Billiard::Solids ? Billiard::Stripes : Billiard::Solids;

                m_team1Type = m_playersTurn == 0 ? type : otherType;
                m_team2Type = m_playersTurn == 1 ? type : otherType;
            }
            
            // Add to the right teams vector
            if (type == m_team1Type)
            {
                billiard->SetPosition(glm::vec2(-70 + m_team1.size() * billiard->GetRadius(), 50));
                billiard->SetVelocity(glm::vec2(0));
                billiard->SetTrigger(true);
                
                m_team1.push_back(billiard);
            }

            if (type == m_team2Type)
            {
                billiard->SetPosition(glm::vec2(70 - m_team1.size() * billiard->GetRadius(), 50));
                billiard->SetVelocity(glm::vec2(0));
                billiard->SetTrigger(true);
                
                m_team2.push_back(billiard);
            }
            
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
            AddFadingText("Foul", ViewToPixelSpacePos(m_cueBall->GetPosition()) + 10.f);
        }

        // The first hit was of type eight ball, and team still has balls left
        else if (m_firstHit->billiardType == Billiard::EightBall)
        {
            if ((m_playersTurn == 0 && m_team1.size() < 7) ||
                (m_playersTurn == 1 && m_team2.size() < 7))
            {
                ExtraTurn();
                AddFadingText("Foul", ViewToPixelSpacePos(m_cueBall->GetPosition()) + 10.f);
            }
        }
    }
}

void Pool_Table::EndGame()
{
    if (m_playersTurn == 0)
    {
        if (m_team1.size() == 7) // potted eight ball last
        {
            // If cue ball sunk, current player loses
            m_winner = m_cueBallSunk ? 2 : 1;
        }

        else if (m_firstHit->billiardType == Billiard::EightBall)
            m_winner = 1;

        else m_winner = 2;
    }

    if (m_playersTurn == 1)
    {
        if (m_team2.size() == 7) // potted eight ball last
        {
            // If cue ball sunk, current player loses
            m_winner = m_cueBallSunk ? 1 : 2;
        }

        else if (m_firstHit->billiardType == Billiard::EightBall)
            m_winner = 2;

        else m_winner = 1;
    }
}