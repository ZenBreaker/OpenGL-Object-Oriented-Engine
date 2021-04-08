/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
File Name: Triangle.h
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

#ifndef TRIANGLE_H
#define TRIANGLE_H
#include <glm/vec3.hpp>

class Triangle
{
public:
  Triangle(const glm::vec3& _v1 = { 0,0,0 }, const glm::vec3& _v2 = { 1,0,0 }, const glm::vec3& _v3 = {0,1,0}) :
    v1(_v1),
    v2(_v2),
    v3(_v3)
  {}

  const glm::vec3& operator[](size_t index) const
  {
    switch (index)
    {
    case 0:
      return v1;
    case 1:
      return v2;
    case 2:
      return v3;
    }

    throw "You dumb, indexing out of the array";
  }

  glm::vec3& operator[](size_t index)
  {
    switch (index)
    {
    case 0:
      return v1;
    case 1:
      return v2;
    case 2:
      return v3;
    }

    throw "You dumb, indexing out of the array";
  }

  size_t size() const
  {
    return 3;
  }

  glm::vec3 v1;
  glm::vec3 v2;
  glm::vec3 v3;
};

#endif
