#pragma once
#include <Font.h>
#include <Texture.h>

#include "../PhysicsScene.h"

class Billiard : public Circle
{
public:
    Billiard(glm::vec2 _position, glm::vec2 _velocity, float _mass, float _radius,
                glm::vec4 _color = glm::vec4(1), float _elasticity = 1) :
    Circle(_position, _velocity, _mass, _radius, _color, _elasticity) {}
    
    enum BilliardType
    {
        Null = -1,
        CueBall,
        ColorBall1,
        ColorBall2,
        EightBall
    };
    
    BilliardType billiardType = CueBall;
};

class Sprite
{
public:
    Sprite(aie::Texture*& _texture, glm::vec2 _pos, glm::vec2 _size)
    {
        texture = _texture;
        position = _pos;
        size = _size;
    }

    aie::Texture* texture = nullptr;
    glm::vec2 position = glm::vec2(0);
    glm::vec2 size = glm::vec2(0);
};

class Pool_Table : public PhysicsScene
{
public:
    void Startup(aie::Application* _app) override;
    void Update(float _dt) override;
    void Draw() override;

    // Generator functions
    void MakeTriangle(glm::vec2 _startPos, float _spacing = 6.f);
    void MakePoolTable(glm::vec2 _tableCenterOffset, glm::vec2 _tableExtents,
                        bool _showTriggers = false);

    void PocketEnter(PhysicsObject* _other);
    void CueBallCollision(PhysicsObject* _other);

    void EndGame(int _winningTeam);

    // Helper Functions
    void ExtraTurn()
    {
        m_turnAddCountdown = 2;
        m_playersTurn = 1 - m_playersTurn;
    }

    bool AllBallsStopped()
    {
        for (auto obj : m_actors)
        {
            Rigidbody* rb = dynamic_cast<Rigidbody*>(obj);

            if (rb != nullptr && rb->GetVelocity() != glm::vec2(0))
                return false;
        }

        return true;
    }
    
protected:
    Billiard* m_cueBall = nullptr;
    glm::vec2 m_cueBallStartPos = glm::vec2(0);

    Billiard::BilliardType m_team1Type = Billiard::Null;
    Billiard::BilliardType m_team2Type = Billiard::Null;
    
    int m_team1Counter = 0;
    int m_team2Counter = 0;

    int m_playersTurn = 0;
    int m_turnAddCountdown = 1;
    
    Billiard* m_firstHit = nullptr;

    bool m_runEndgame = false;
    bool m_eightBallFirst = false;

    glm::vec2 m_dragStartPos = glm::vec2(0);
    glm::vec2 m_dragVector = glm::vec2(0);
    bool m_dragging = false;

    bool m_potted = false;

    aie::Font* m_font = nullptr;
    Sprite* m_table = nullptr;
};
