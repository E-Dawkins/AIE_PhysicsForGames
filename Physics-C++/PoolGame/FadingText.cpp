#include "FadingText.h"

void FadingText::DrawText()
{
    glm::vec4 prevColor = renderer2D->getRenderColour();

    renderer2D->setRenderColour(color.r, color.g, color.b, alphaPercent);
    renderer2D->drawText(font, text, position.x, position.y);

    renderer2D->setRenderColour(prevColor.r, prevColor.g, prevColor.b, prevColor.a);
}

void FadingText::UpdateText(float _dt)
{
    alphaPercent -= _dt;

    if (alphaPercent < 0) alphaPercent = 0;
}