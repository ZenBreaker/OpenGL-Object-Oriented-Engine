#include "AABB.h"

#include <limits>
#include "Engine.h"

AABB::AABB() : m_Max(-INFINITY), m_Min(INFINITY)
{
}

AABB::~AABB()
{
}

void AABB::Draw()
{
  Debug::Rect3D rect;

  rect.topLeft = Debug::Point(Center());

  glm::vec3 size = Size();
  rect.width = size.x;
  rect.height = size.y;
  rect.depth = size.z;

  Engine::get().m_Debug.drawWorldRects(rect, 1, 1, 1, true);
}

void AABB::Clear()
{
  m_Max = glm::vec3(-INFINITY);
  m_Min = glm::vec3(INFINITY);
}

glm::vec3 AABB::Center()
{
  return (m_Max + m_Min) / 2.0f;
}

glm::vec3 AABB::Size()
{
  return m_Max - m_Min;
}

void AABB::Update(const std::vector<glm::vec3>& points)
{
  Clear();

  for(unsigned i = 0; i < points.size(); ++i)
  {
    if (points[i].x < m_Min.x)
    {
      m_Min.x = points[i].x;
    }
    else if (points[i].x > m_Max.x)
    {
      m_Max.x = points[i].x;
    }

    if (points[i].y < m_Min.y)
    {
      m_Min.y = points[i].y;
    }
    else if (points[i].y > m_Max.y)
    {
      m_Max.y = points[i].y;
    }

    if (points[i].z < m_Min.z)
    {
      m_Min.z = points[i].z;
    }
    else if (points[i].z > m_Max.z)
    {
      m_Max.z = points[i].z;
    }
  }
}
