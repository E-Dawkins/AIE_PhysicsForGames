#pragma once
#include <Font.h>
#include <Renderer2D.h>
#include <glm/glm.hpp>

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

    void DrawText();

    void UpdateText(float _dt);

    float alphaPercent = 1;

    const char* text = nullptr;
    glm::vec2 position = glm::vec2();
    aie::Font* font = nullptr;
    aie::Renderer2D* renderer2D = nullptr;
    glm::vec4 color = glm::vec4(0);
};
