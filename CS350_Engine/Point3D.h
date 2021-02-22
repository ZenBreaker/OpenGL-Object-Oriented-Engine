/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
File Name: Point3D.h
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

#ifndef POINT3D_H
#define POINT3D_H
#include <glm/vec3.hpp>

class Point3D
{
public:
  Point3D(glm::vec3 pos):
  m_Position(pos)
  {
  }

  // (x, y, z)
  glm::vec3 m_Position;
};

#endif
