#include "MainScene.h"

#include <Gizmos.h>
#include <iostream>

void MainScene::Startup(aie::Application* _app)
{
    m_shapeAPos = glm::vec2(40, 50);
    m_shapeBPos = glm::vec2(45, 5);
}

void MainScene::Update(float _dt)
{
    m_shapeAVerts.clear();
    m_shapeBVerts.clear();
    
    for (int i = 0; i < 5; i++)
    {
        glm::vec2 pos = glm::vec2(cosf(DegreeToRadian(72.f * i)), sinf(DegreeToRadian(72.f * i)));
        m_shapeAVerts.push_back(m_shapeAPos + pos * 7.5f);
    }

    for (int i = 0; i < 6; i++)
    {
        glm::vec2 pos = glm::vec2(cosf(DegreeToRadian(60.f * i)), sinf(DegreeToRadian(60.f * i)));
        m_shapeBVerts.push_back(m_shapeBPos + pos * 7.5f);
    }

    m_shapeAPos += glm::vec2(0, -5.0f * _dt);
    
    m_points.clear();
    
    for (auto pt1 : m_shapeAVerts)
    {
        for (auto pt2 : m_shapeBVerts)
        {
            m_points.push_back(pt1 - pt2);
        }
    }
}

void MainScene::Draw()
{
    // Axes
    aie::Gizmos::add2DLine(glm::vec2(-m_windowExtents.x, 0), glm::vec2(m_windowExtents.x, 0), glm::vec4(0, 1, 0, 1));
    aie::Gizmos::add2DLine(glm::vec2(0, -m_windowExtents.y), glm::vec2(0, m_windowExtents.y), glm::vec4(0, 1, 0, 1));

    // Shapes
    for (int i = 1; i < m_shapeAVerts.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            aie::Gizmos::add2DLine(m_shapeAVerts.at(i), m_shapeAVerts.at(j), glm::vec4(1, 0, 0, 1));
        }
    }

    for (int i = 1; i < m_shapeBVerts.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            aie::Gizmos::add2DLine(m_shapeBVerts.at(i), m_shapeBVerts.at(j), glm::vec4(1, 0, 0, 1));
        }
    }
    
    // Minkowski difference
    for (int i = 1; i < m_points.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            aie::Gizmos::add2DLine(m_points.at(i), m_points.at(j), glm::vec4(1));
        }
    }
}
