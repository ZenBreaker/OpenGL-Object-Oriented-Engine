

#include "BoundingSphere.h"

#include "Engine.h"

/**
 * @brief 
 *   Constructor a new Bounding Sphere object
 */
BoundingSphere::BoundingSphere()
{
  // defult bounding method
  SetBoundingType(Centroid);
}

/**
 * @brief 
 *   Destructure the Bounding Sphere object
 */
BoundingSphere::~BoundingSphere()
{
}

/**
 * @brief 
 *   Requests to debug draw sphere
 */
void BoundingSphere::Draw()
{
  // request sphere
  Engine::get().m_Debug.drawWorldSphere(m_Center, m_Radius, true);
}

/**
 * @brief 
 *   Reset bounding information
 */
void BoundingSphere::Clear()
{
  m_Center = glm::vec3();
  m_Radius = 0;
}

/**
 * @brief 
 *   Set bounding type method
 * 
 * @param type 
 *   Calculating type method
 * 
 * @return BoundingType 
 *   bounding type that will be use to debug draw
 */
BoundingType BoundingSphere::SetBoundingType(BoundingType type)
{
  if(m_Type != type)
  {
    Clear();

    m_Type = type;
  }

  return m_Type;
}

/**
 * @brief 
 *   Calculates the bounding volume of the points
 * 
 * @param vertices 
 *   An array of points to create the bounding sphere object around
 */
void BoundingSphere::Update(const std::vector<glm::vec3> & vertices)
{
  Clear(); // clear the previous information

  //call right type of method to use
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

/**
 * @brief 
 *  Centroid Method calculation
 * 
 * @param vertices 
 *   An array of points to create the bounding sphere object around
 */
void BoundingSphere::CentroidMethod(const std::vector<glm::vec3>& vertices)
{
  m_Center = {};
  m_Radius = 1.0f;
}

/**
 * @brief 
 *  Ritters Method calculation
 * 
 * @param vertices 
 *   An array of points to create the bounding sphere object around
 */
void BoundingSphere::RittersMethod(const std::vector<glm::vec3>& vertices)
{
}

/**
 * @brief 
 *  Larssons Method calculation
 * 
 * @param vertices 
 *   An array of points to create the bounding sphere object around
 */
void BoundingSphere::LarssonsMethod(const std::vector<glm::vec3>& vertices)
{
}

/**
 * @brief 
 *  PCA Method calculation
 * 
 * @param vertices 
 *   An array of points to create the bounding sphere object around
 */
void BoundingSphere::PCAMethod(const std::vector<glm::vec3>& vertices)
{
}
