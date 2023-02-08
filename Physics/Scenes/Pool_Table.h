#pragma once
#include "../PhysicsScene.h"

class Pool_Table : public PhysicsScene
{
public:
    void Startup() override;
    void Update(float _dt) override;
    void Draw() override;

    vector<Circle*> MakeTriangle(glm::vec2 _startPos, float _xDiff, int _rows = 5);
    void ColorTriangle(vector<Circle*>& _balls, glm::vec4 _color1, glm::vec4 _color2,
                        glm::vec4 _8BallColor = glm::vec4(0.2f, 0.2f, 0.2f, 1));
    
protected:
    Circle* m_cueBall = nullptr;
};
