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
