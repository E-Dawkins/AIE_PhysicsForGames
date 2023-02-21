#pragma once
#include <Texture.h>
#include <glm/glm.hpp>

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
