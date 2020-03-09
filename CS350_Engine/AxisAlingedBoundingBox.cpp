/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: AxisAlingedBoundingBox.c
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

#include "AxisAlingedBoundingBox.h" /* AxisAlingedBoundingBox */

#include <limits> 
#include "Engine.h" /* Engine */

/**
 * @brief 
 *   Constructor a new AxisAlingedBoundingBox object
 */
AxisAlingedBoundingBox::AxisAlingedBoundingBox() : m_Max(-INFINITY), m_Min(INFINITY)
{
}

/**
 * @brief 
 *   Destructure the AxisAlingedBoundingBox object
 */
AxisAlingedBoundingBox::~AxisAlingedBoundingBox()
{
}

/**
 * @brief 
 *   Draws the debug AxisAlingedBoundingBox information
 */
void AxisAlingedBoundingBox::Draw(const glm::vec3& color) const
{
  Rect3D rect; // temp rect for debug draw

  const glm::vec3 size = Size(); //getting the size of the bounding box

  // setting the size of the rect 
  rect.width = size.x;
  rect.height = size.y;
  rect.depth = size.z;

  // setting the position of the rect
  rect.Center = Center();

  // request to draw the rect
  Engine::get().m_Debug.drawWorldRects(rect, color, true);
}

/**
 * @brief 
 *   Clears the AxisAlingedBoundingBox information
 */
void AxisAlingedBoundingBox::Clear()
{
  m_Max = glm::vec3(-INFINITY); // resetting the max to the lowest number
  m_Min = glm::vec3(INFINITY);  // resetting the min to the highest number
}

/**
 * @brief 
 *   Returns the center of the AxisAlingedBoundingBox Object
 * 
 * @return glm::vec3 
 */
glm::vec3 AxisAlingedBoundingBox::Center() const
{
  return (m_Max + m_Min) / 2.0f; // calculating the center of the rect
}

/**
 * @brief 
 *   Returns the size of the AxisAlingedBoundingBox Object
 * 
 * @return glm::vec3 
 *   The size in x y and z directions
 */
glm::vec3 AxisAlingedBoundingBox::Size() const 
{
  return m_Max - m_Min; // calculating the size of the rect
}

/**
 * @brief 
 *   Calculates the bounding volume of the points
 * 
 * @param points 
 *   An array of points to create the AxisAlingedBoundingBox object around
 */
void AxisAlingedBoundingBox::Update(const glm::mat4& modelToWorld, const std::vector<glm::vec3>& points)
{
  Clear(); // clear the previous information
  //
  // static temp vector, so there isnt reallocations every frame just for this vector
  static std::vector<glm::vec3> temp;

  // clear for every draw call and make sure there is enough space
  temp.clear();
  temp.reserve(points.size());

  // loop through all the vertices of the model
  for (unsigned i = 0; i < points.size(); ++i)
  {
    // convert vertices to world space
    temp.emplace_back(glm::vec3(modelToWorld * glm::vec4(points[i], 1.0f)));
  }

  //for each point in points
  for(unsigned i = 0; i < temp.size(); ++i)
  {
    if (temp[i].x < m_Min.x) // check for lower x value
    {
      m_Min.x = temp[i].x;
    }
    else if (temp[i].x > m_Max.x) // check for higher x value
    {
      m_Max.x = temp[i].x;
    }

    if (temp[i].y < m_Min.y) // check for lower y value
    {
      m_Min.y = temp[i].y;
    }
    else if (temp[i].y > m_Max.y) // check for higher y value
    {
      m_Max.y = temp[i].y;
    }

    if (temp[i].z < m_Min.z) // check for lower z value
    {
      m_Min.z = temp[i].z;
    }
    else if (temp[i].z > m_Max.z) // check for higher z value
    {
      m_Max.z = temp[i].z;
    }
  }
}
