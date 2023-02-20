#pragma once
#include "PhysicsScene.h"

class MainScene : public PhysicsScene
{
public:
    void Startup(aie::Application* _app) override;
    void Update(float _dt) override;
    void Draw() override;

protected:
    glm::vec2 m_shapePos1 = glm::vec2();
    glm::vec2 m_shapePos2 = glm::vec2();

    vector<glm::vec2> m_shapePts1;
    vector<glm::vec2> m_shapePts2;
    
    vector<glm::vec2> m_points;
};