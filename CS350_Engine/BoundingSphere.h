/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: BoundingSphere.h
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

#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#include <glm/glm.hpp>
#include <string>
#include <vector>


/**
 * @brief 
 *   Bounding Sphere debug object
 */
class BoundingSphere
{
public:
  //! Bounding types
  enum Type
  {
    Centroid,
    Ritters,
    Larssons,
    PCA,
  };

  //! String version of bounding types
  static std::vector<std::string> TypeNames;

  // Constructor a new Bounding Sphere object
  BoundingSphere();
  // Destructure the Bounding Sphere object
  ~BoundingSphere();

  // Requests to debug draw sphere
  void Draw();

  // Reset bounding information
  void Clear();

  // Set bounding type method
  Type SetBoundingType(Type type);

  // Calculates the bounding volume of the points
  void Update(const glm::mat4& modelToWorld, const std::vector<glm::vec3>& vertices);

  // Centroid Method calculation
  void CentroidMethod(const std::vector<glm::vec3>& vertices);

  // Ritters Method calculation
  void RittersMethod(const std::vector<glm::vec3>& vertices);

  // Larssons Method calculation
  void LarssonsMethod(const std::vector<glm::vec3>& vertices);

  // PCA Method calculation
  void PCAMethod(const std::vector<glm::vec3>& vertices);
  
public:
  glm::vec3 m_Center;  //!< Center of the sphere
  float m_Radius;      //!< Radius of the sphere
  Type m_Type; //!< Bounding type


  static void BoundingSphereCentroid(const std::vector<glm::vec3>& vertices)
  {
    glm::vec3 center;
    float radius = 0.0f;

    for (unsigned int i = 0; i < vertices.size(); ++i)
    {
      center += vertices[i];
    }

    center /= (float)vertices.size();

    for (unsigned int i = 0; i < vertices.size(); ++i)
    {
      const float dx = center.x - vertices[i].x;
      const float dy = center.y - vertices[i].y;
      const float dz = center.z - vertices[i].z;
      radius = std::max(dx * dx + dy * dy + dz * dz, radius);
    }

    radius = sqrtf(radius) * 2.0f;
  }
};



#endif