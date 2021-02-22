/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
File Name: Plane.h
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

#ifndef PLANE_H
#define PLANE_H
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/detail/func_geometric.inl>

#include "Triangle.h"

class Plane
{
public:
  // (n.x, n.y, n.z, d)
  // a*x + b*y + c*z + d = 0
  // a*x + b*y + c*z = -d
  Plane(glm::vec3 normal, float d)
  {
    m_Normal.w = d / glm::length(normal);
    (glm::vec3)m_Normal = normalize(normal);
  }

  Plane(Triangle triangle)
  {
    const glm::vec3 u = triangle.v2 - triangle.v1;
    const glm::vec3 v = triangle.v3 - triangle.v1;

    (glm::vec3)m_Normal = glm::cross(u, v);

    m_Normal.w = glm::dot(triangle.v1, (glm::vec3)m_Normal);

    m_Normal.w = m_Normal.w / glm::length((glm::vec3)m_Normal);

    normalize(m_Normal);
  }

  float DistanceFromPlane(glm::vec3 point) const
  {
    return glm::dot(point, (glm::vec3)m_Normal) - m_Normal.w;
  }

  glm::vec4 m_Normal;
};

#endif
