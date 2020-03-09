/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: AxisAlingedBoundingBox.h
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

#ifndef AXISALINGEDBOUNDINGBOX_H
#define AXISALINGEDBOUNDINGBOX_H

#include <glm/glm.hpp> /* vec3   */
#include <vector>      /* vector */

/**
 * @brief
 *   Axis Alinged Bounding Boxes Structure
 */
class AxisAlingedBoundingBox
{
public:
  // Constructor a new AxisAlingedBoundingBox object
  AxisAlingedBoundingBox();

  // Destructure the AxisAlingedBoundingBox object
  ~AxisAlingedBoundingBox();

  // Draws the debug AxisAlingedBoundingBox information
  void Draw(const glm::vec3 & color) const;

  // Clears the AxisAlingedBoundingBox information
  void Clear();

  // Returns the center of the AxisAlingedBoundingBox Object
  glm::vec3 Center() const;

  // Returns the size of the AxisAlingedBoundingBox Object
  glm::vec3 Size() const;

  // Calculates the bounding volume of the points
  void Update(const glm::mat4 & modelToWorld, const std::vector<glm::vec3> & points);
  
  glm::vec3 m_Max; //!< The maximum point of the points
  glm::vec3 m_Min; //!< The minimum point of the points
private:

};

#endif