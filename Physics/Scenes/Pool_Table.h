#pragma once
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

class Pool_Table : public PhysicsScene
{
public:
    void Startup(aie::Application* _app) override;
    void Update(float _dt) override;
    void Draw() override;

    // Generator functions
    vector<Billiard*> MakeTriangle(glm::vec2 _startPos, float _xDiff, int _rows = 5);
    void ColorTriangle(vector<Billiard*>& _balls, glm::vec4 _color1, glm::vec4 _color2,
                        glm::vec4 _8BallColor = glm::vec4(0.2f, 0.2f, 0.2f, 1));
    void MakePoolTable();

    void PocketEnter(PhysicsObject* _other);
    void CueBallCollision(PhysicsObject* _other);
    
protected:
    Billiard* m_cueBall = nullptr;
    glm::vec2 m_cueBallStartPos = glm::vec2(0);

    Billiard::BilliardType m_team1Type = Billiard::Null;
    Billiard::BilliardType m_team2Type = Billiard::Null;
    
    int m_team1Counter = 0;
    int m_team2Counter = 0;

    int m_turnAddCountdown = 1;
    
    int m_playersTurn = 0;
    Billiard* m_firstHit = nullptr;

    void ExtraTurn()
    {
        m_turnAddCountdown = 2;
        m_playersTurn = (m_playersTurn + 1) % 2;
    }
};
