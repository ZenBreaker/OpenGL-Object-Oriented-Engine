/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
File Name: AABB.h
Purpose:
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
Project: michael.ngo_CS350_2
Author: Michael Ngo, michael.ngo, 90003217
Creation date: 2/21/2020
End Header --------------------------------------------------------*/

#ifndef AABB_H
#define AABB_H
#include <glm/vec3.hpp>

class AABB
{
public:
  AABB(glm::vec3 min = glm::vec3(FLT_MAX), glm::vec3 max = glm::vec3(-FLT_MAX)) :
  m_Min(min),
  m_Max(max)
  {
  }

  void operator+(const glm::vec3& point)
  {
    if (point.x < m_Min.x) // check for lower x value
    {
      m_Min.x = point.x;
    }
    else if (point.x > m_Max.x) // check for higher x value
    {
      m_Max.x = point.x;
    }

    if (point.y < m_Min.y) // check for lower y value
    {
      m_Min.y = point.y;
    }
    else if (point.y > m_Max.y) // check for higher y value
    {
      m_Max.y = point.y;
    }

    if (point.z < m_Min.z) // check for lower z value
    {
      m_Min.z = point.z;
    }
    else if (point.z > m_Max.z) // check for higher z value
    {
      m_Max.z = point.z;
    }
  }

  glm::vec3 m_Min;
  glm::vec3 m_Max;

  glm::vec3 m_Center;
  glm::vec3 m_HalfExtents;
};

#endif
