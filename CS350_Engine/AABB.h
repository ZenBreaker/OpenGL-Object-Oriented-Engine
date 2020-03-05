/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: AABB.h
Purpose: Axis Alinged Bounding Boxes for Debug purposes 
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
Creation date: 2/28/2020
End Header --------------------------------------------------------*/

#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp> /* vec3   */
#include <vector>      /* vector */

/**
 * @brief Axis Alinged Bounding Boxes Structure
 *   
 */
class AABB
{
public:
  // Constructor a new AABB object
  AABB();

  // Destructure the AABB object
  ~AABB();

  // Draws the debug AABB information
  void Draw();

  // Clears the AABB information
  void Clear();

  // Returns the center of the AABB Object
  glm::vec3 Center();

  // Returns the size of the AABB Object
  glm::vec3 Size();

  // Calculates the bounding volume of the points
  void Update(const std::vector<glm::vec3> & points);
  
private:

  glm::vec3 m_Max; //!< The maximum point of the points
  glm::vec3 m_Min; //!< The minimum point of the points
};

#endif