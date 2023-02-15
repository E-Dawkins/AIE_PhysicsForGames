#pragma once
#include <Font.h>
#include <Texture.h>

#include "../PhysicsScene.h"

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
        Solids,
        Stripes,
        EightBall
    };
    
    BilliardType billiardType = CueBall;
    Sprite* billiardSprite = nullptr;
    aie::Renderer2D* renderer2D = nullptr;

    void Draw(float _alpha) override
    {
        if (renderer2D != nullptr && billiardSprite  != nullptr)
        {
            renderer2D->begin();

            glm::vec4 prev = renderer2D->getRenderColour();

            renderer2D->setRenderColour(m_color.r, m_color.g, m_color.b, m_color.a);
            
            renderer2D->drawSprite(billiardSprite->texture, billiardSprite->position.x,
               billiardSprite->position.y, billiardSprite->size.x, billiardSprite->size.y);

            renderer2D->setRenderColour(prev.r, prev.g, prev.b, prev.a);
            
            renderer2D->end();
        }
    }
};

class FadingText
{
public:
    FadingText(const char* _text, glm::vec2 _pos, aie::Font*& _font, aie::Renderer2D*& _renderer, glm::vec4 _color = glm::vec4(1))
    {
        text = _text;
        position = _pos;
        font = _font;
        renderer2D = _renderer;
        color = _color;
    }

    void DrawText()
    {
        glm::vec4 prevColor = renderer2D->getRenderColour();
        
        renderer2D->setRenderColour(color.r, color.g, color.b, alphaPercent);
        renderer2D->drawText(font, text, position.x, position.y);

        renderer2D->setRenderColour(prevColor.r, prevColor.g, prevColor.b, prevColor.a);
    }

    void UpdateText(float _dt)
    {
        alphaPercent -= _dt;

        if (alphaPercent < 0) alphaPercent = 0;
    }
    
    float alphaPercent = 1;
    
    const char* text = nullptr;
    glm::vec2 position = glm::vec2();
    aie::Font* font = nullptr;
    aie::Renderer2D* renderer2D = nullptr;
    glm::vec4 color = glm::vec4(0);
};

class Pool_Table : public PhysicsScene
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
