/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: AABB.c
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
Creation date: 2/2/2020
End Header --------------------------------------------------------*/

#include "AABB.h" /* AABB */

#include <limits> 
#include "Engine.h" /* Engine */

/**
 * @brief 
 *   Constructor a new AABB object
 */
AABB::AABB() : m_Max(-INFINITY), m_Min(INFINITY)
{
}

/**
 * @brief 
 *   Destructure the AABB object
 */
AABB::~AABB()
{
}

/**
 * @brief 
 *   Draws the debug AABB information
 */
void AABB::Draw()
{
  Debug::Rect3D rect; // temp rect for debug draw

  const glm::vec3 size = Size(); //getting the size of the bounding box

  // setting the size of the rect 
  rect.width = size.x;
  rect.height = size.y;
  rect.depth = size.z;

  // setting the position of the rect
  rect.Center = Center();

  // request to draw the rect
  Engine::get().m_Debug.drawWorldRects(rect, 1, 1, 1, true);
}

/**
 * @brief 
 *   Clears the AABB information
 */
void AABB::Clear()
{
  m_Max = glm::vec3(-INFINITY); // resetting the max to the lowest number
  m_Min = glm::vec3(INFINITY);  // resetting the min to the highest number
}

/**
 * @brief 
 *   Returns the center of the AABB Object
 * 
 * @return glm::vec3 
 */
glm::vec3 AABB::Center()
{
  return (m_Max + m_Min) / 2.0f; // calculating the center of the rect
}

/**
 * @brief 
 *   Returns the size of the AABB Object
 * 
 * @return glm::vec3 
 *   The size in x y and z directions
 */
glm::vec3 AABB::Size()
{
  return m_Max - m_Min; // calculating the size of the rect
}

/**
 * @brief 
 *   Calculates the bounding volume of the points
 * 
 * @param points 
 *   An array of points to create the AABB object around
 */
void AABB::Update(const std::vector<glm::vec3>& points)
{
  Clear(); // clear the previous information

  //for each point in points
  for(unsigned i = 0; i < points.size(); ++i)
  {
    if (points[i].x < m_Min.x) // check for lower x value
    {
      m_Min.x = points[i].x;
    }
    else if (points[i].x > m_Max.x) // check for higher x value
    {
      m_Max.x = points[i].x;
    }

    if (points[i].y < m_Min.y) // check for lower y value
    {
      m_Min.y = points[i].y;
    }
    else if (points[i].y > m_Max.y) // check for higher y value
    {
      m_Max.y = points[i].y;
    }

    if (points[i].z < m_Min.z) // check for lower z value
    {
      m_Min.z = points[i].z;
    }
    else if (points[i].z > m_Max.z) // check for higher z value
    {
      m_Max.z = points[i].z;
    }
  }
}
