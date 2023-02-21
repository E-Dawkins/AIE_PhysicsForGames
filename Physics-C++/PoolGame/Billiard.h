#pragma once
#include <Circle.h>
#include <Renderer2D.h>

#include "Sprite.h"

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

    void Draw(float _alpha) override;
};
