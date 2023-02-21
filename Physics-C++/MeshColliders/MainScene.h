#pragma once
#include "PhysicsScene.h"

struct Simplex;

class MainScene : public PhysicsScene
{
public:
    void Startup(aie::Application* _app) override;
    void Update(float _dt) override;
    void Draw() override;

protected:
    std::vector<glm::vec2> m_shapeAVerts;
    std::vector<glm::vec2> m_shapeBVerts;

    glm::vec2 m_shapeAPos = glm::vec2();
    glm::vec2 m_shapeBPos = glm::vec2();
    
    vector<glm::vec2> m_points;
};
