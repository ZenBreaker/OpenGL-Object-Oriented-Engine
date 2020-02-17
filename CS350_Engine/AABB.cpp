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

void AABB::AddPoint(const glm::vec3 & point)
{
  if (point.x < m_Min.x)
  {
    m_Min.x = point.x;
  }
  else if (point.x > m_Max.x)
  {
    m_Max.x = point.x;
  }

  if (point.y < m_Min.y)
  {
    m_Min.y = point.y;
  }
  else if (point.y > m_Max.y)
  {
    m_Max.y = point.y;
  }

  if (point.z < m_Min.z)
  {
    m_Min.z = point.z;
  }
  else if (point.z > m_Max.z)
  {
    m_Max.z = point.z;
  }
}
