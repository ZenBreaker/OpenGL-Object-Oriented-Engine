/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: BoundingSphere.cpp
Purpose: Bounding sphere for debug draw
Language: C++ and Visual Studio 2019
Platform:
compiler version:
  14.1 - 14.16
hardware requirements:
  1.8 GHz or faster processor. Dual-core or better recommended
  2 GB of RAM; 4 GB of RAM recommended (2.5 GB minimum if running on a virtual machine)
  Hard disk space: up to 130 GB of available space, depending on features installed; typical installations require 20-50 GB of free space.
  Hard disk speed: to improve performance, install Windows and Visual Studio on a solid state drive (SSD).
  Video card that supports a minimum display resolution of 720p (1280 by 720); Visual Studio will work best at a resolution of WXGA (1366 by 768) or higher.
operating systems:
  Windows 10 64bit
Project: michael.ngo_CS350_1
Author: Michael Ngo, michael.ngo, 90003217
Creation date: 2/2/2020
End Header --------------------------------------------------------*/

#include "BoundingSphere.h"

#include "Engine.h"

std::vector<std::string> BoundingSphere::TypeNames
{
  "Centroid",
  "Ritters",
  "Larssons",
  "PCA",
};


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
 * @return Type 
 *   bounding type that will be use to debug draw
 */
BoundingSphere::Type BoundingSphere::SetBoundingType(BoundingSphere::Type type)
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
 * @param modelToWorld
 *   Model to world matrix
 *
 * @param vertices 
 *   An array of points to create the bounding sphere object around
 */
void BoundingSphere::Update(const glm::mat4& modelToWorld, const std::vector<glm::vec3> & vertices)
{
  Clear(); // clear the previous information

  // static temp vector, so there isnt reallocations every frame just for this vector
  static std::vector<glm::vec3> temp;

  // clear for every draw call and make sure there is enough space
  temp.clear();
  temp.reserve(vertices.size());

  // loop through all the vertices of the model
  for (unsigned i = 0; i < vertices.size(); ++i)
  {
    // convert vertices to world space
    temp.emplace_back(glm::vec3(modelToWorld * glm::vec4(vertices[i], 1.0f)));
  }

  //call right type of method to use
  switch (m_Type)
  {
  case Centroid:
  {
    CentroidMethod(temp);
    break;
  }
  case Ritters:
  {
    RittersMethod(temp);
    break;
  }
  case Larssons:
  {
    LarssonsMethod(temp);
    break;
  }
  case PCA:
  {
    PCAMethod(temp);
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
  for(unsigned int i = 0; i < vertices.size(); ++i)
  {
    m_Center += vertices[i];
  }

  m_Center /= (float)vertices.size();

  for (unsigned int i = 0; i < vertices.size(); ++i)
  { 
    const float euclidenDistance = distance(m_Center, vertices[i]);

    if(m_Radius < euclidenDistance)
    {
      m_Radius = euclidenDistance;
    }
  }
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
