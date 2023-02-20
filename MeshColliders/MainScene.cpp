#include "MainScene.h"

#include <Gizmos.h>
#include <iostream>

void MainScene::Startup(aie::Application* _app)
{
    m_shapePos1 = glm::vec2(45, 30);
    m_shapePos2 = glm::vec2(40, 5);
}

void MainScene::Update(float _dt)
{
    m_shapePts1.clear();
    m_shapePts2.clear();
    
    for (int i = 0; i < 5; i++)
    {
        glm::vec2 ptPos = glm::vec2(cosf(DegreeToRadian(18.f + 72.f * i)), sinf(DegreeToRadian(18.f + 72.f * i)));
        m_shapePts1.push_back(m_shapePos1 + ptPos * 7.5f);
    }

    for (int i = 0; i < 5; i++)
    {
        glm::vec2 ptPos = glm::vec2(cosf(DegreeToRadian(18.f + 72.f * i)), sinf(DegreeToRadian(18.f + 72.f * i)));
        m_shapePts2.push_back(m_shapePos2 + ptPos * 7.5f);
    }

    m_shapePos1 += glm::vec2(0, -_dt * 5.f);
    // m_shapePos2 += glm::vec2(_dt * 3.f, 0);

    m_points.clear();
    
    for (auto pt1 : m_shapePts1)
    {
        for (auto pt2 : m_shapePts2)
        {
            m_points.push_back(pt1 - pt2);
        }
    }
}

void MainScene::Draw()
{
    aie::Gizmos::clear();
    aie::Gizmos::add2DCircle(m_shapePos1, 7.5f, 5, glm::vec4(1, 0, 0, 0));
    aie::Gizmos::add2DCircle(m_shapePos2, 7.5f, 5, glm::vec4(1, 0, 0, 0));

    aie::Gizmos::add2DLine(glm::vec2(-m_windowExtents.x, 0), glm::vec2(m_windowExtents.x, 0), glm::vec4(0, 1, 0, 1));
    aie::Gizmos::add2DLine(glm::vec2(0, -m_windowExtents.y), glm::vec2(0, m_windowExtents.y), glm::vec4(0, 1, 0, 1));

    for (int i = 1; i < m_points.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            aie::Gizmos::add2DLine(m_points.at(i), m_points.at(j), glm::vec4(1));
        }
    }
}
