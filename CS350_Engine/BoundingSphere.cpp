#include "BoundingSphere.h"

#include "Engine.h"


BoundingSphere::BoundingSphere()
{
  SetBoundingType(Centroid);
}


BoundingSphere::~BoundingSphere()
{
}

void BoundingSphere::Draw()
{
  Engine::get().m_Debug.drawWorldSphere(m_Center, m_Radius, true);
}

void BoundingSphere::Clear()
{
  m_Center = glm::vec3();
  m_Radius = 0;
}

BoundingType BoundingSphere::SetBoundingType(BoundingType type)
{
  if(m_Type != type)
  {
    Clear();

    m_Type = type;
  }

  return m_Type;
}

void BoundingSphere::Update(const std::vector<glm::vec3> & vertices)
{
  Clear();

  switch (m_Type)
  {
  case Centroid:
  {
    CentroidMethod(vertices);
    break;
  }
  case Ritters:
  {
    RittersMethod(vertices);
    break;
  }
  case Larssons:
  {
    LarssonsMethod(vertices);
    break;
  }
  case PCA:
  {
    PCAMethod(vertices);
    break;
  }
  }
}

void BoundingSphere::CentroidMethod(const std::vector<glm::vec3>& vertices)
{
  m_Center = {};
  m_Radius = 1.0f;
}

void BoundingSphere::RittersMethod(const std::vector<glm::vec3>& vertices)
{
}

void BoundingSphere::LarssonsMethod(const std::vector<glm::vec3>& vertices)
{
}

void BoundingSphere::PCAMethod(const std::vector<glm::vec3>& vertices)
{
}
