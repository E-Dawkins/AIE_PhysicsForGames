#include "Billiard.h"

void Billiard::Draw(float _alpha)
{
    if (renderer2D != nullptr && billiardSprite != nullptr)
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
