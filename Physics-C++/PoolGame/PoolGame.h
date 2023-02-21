#pragma once
#include <Font.h>
#include <Texture.h>

#include "PhysicsScene.h"
#include "Sprite.h"
#include "Billiard.h"
#include "FadingText.h"

class PoolGame : public PhysicsScene
{
public:
    void Startup(aie::Application* _app) override;
    void Shutdown();
    void Update(float _dt) override;
    void Draw() override;

    // Generator functions
    void MakeTriangle(glm::vec2 _startPos, float _spacing = 6.f);
    void MakePoolTable(bool _showBounds = false);

    void PocketEnter(PhysicsObject* _other);
    void CueBallCollision(PhysicsObject* _other);

    void EndGame();

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

    void AddFadingText(const char* _text, glm::vec2 _pos)
    {
        m_fadingTexts.push_back(new FadingText(_text, _pos, m_fontBig, m_renderer2D, glm::vec4(1, 0, 0, 1)));
    }

    void CheckFadingTexts()
    {
        for (int i = m_fadingTexts.size() - 1; i >= 0; i--)
        {
            if (m_fadingTexts.at(i)->alphaPercent == 0)
                m_fadingTexts.erase(m_fadingTexts.begin() + i);
        }
    }

protected:
    Billiard* m_cueBall = nullptr;
    bool m_cueBallSunk = false;

    Billiard::BilliardType m_team1Type = Billiard::Null;
    Billiard::BilliardType m_team2Type = Billiard::Null;

    vector<Billiard*> m_team1;
    vector<Billiard*> m_team2;

    int m_playersTurn = 0;
    int m_turnAddCountdown = 1;

    Billiard* m_firstHit = nullptr;

    bool m_runEndgame = false;
    bool m_eightBallFirst = false;

    glm::vec2 m_dragStartPos = glm::vec2(0);
    glm::vec2 m_dragVector = glm::vec2(0);
    bool m_dragging = false;

    bool m_potted = false;

    aie::Font* m_fontSmall = nullptr;
    aie::Font* m_fontBig = nullptr;
    Sprite* m_table = nullptr;

    vector<FadingText*> m_fadingTexts;

    int m_winner = 0;
};