/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: Object.cpp
Purpose: holds object data
Language: C++ and Visual Studio 2017
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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Object.h"

Object::Object(const char* name) :
  m_Name(name),
  m_Model(nullptr),
  m_Shader(nullptr),
  m_Centroid(glm::vec3(0.0f)),
  m_ScaleVector(glm::vec3(1.0f)),
  m_RotationVector(glm::vec3(0.0f, 1.0f, 0.0f)),
  m_RotationAngle(0.0f),
  m_RotationAmount(0.0f),
  m_DrawAABB(true)
{
}

Object::~Object()
{
  m_Model = nullptr;
  m_Shader = nullptr;
}

void Object::Update(float deltaTime)
{
  m_RotationAngle += m_RotationAmount * deltaTime;

  m_DrawAABB = false;

  if (m_DrawAABB) 
  {
    m_Model->DrawAABBBounds(matrix4());
  }

  // currently bounding spheres is not implamented
  m_DrawBoundingSphere = false;
  
   if (m_DrawBoundingSphere)
  {
    m_Model->DrawBoundingSphere(matrix4());
  }
}

void Object::SetShader(const ShaderPtr& shader)
{
  m_Shader = shader;

  m_PerspectiveMatrixUniform = glGetUniformLocation(shader->m_ProgramID, "PerspectiveMatrix");
  m_ViewMatrixUniform = glGetUniformLocation(shader->m_ProgramID, "ViewMatrix");
  m_ModelMatrixUniform = glGetUniformLocation(shader->m_ProgramID, "ModelMatrix");
  m_NormalMatrixUniform = glGetUniformLocation(shader->m_ProgramID, "NormalMatrix");

  m_EyePositionUniform = glGetUniformLocation(shader->m_ProgramID, "EyePosition");

  m_AmbiantColorUniform = glGetUniformLocation(shader->m_ProgramID, "AmbiantColor");
}

glm::mat4 Object::matrix4(void) const 
{
  return glm::translate(m_Centroid) *
    glm::rotate(m_RotationAngle, m_RotationVector) *
    glm::scale(m_ScaleVector);
}

glm::mat4 Object::normalMatrix(void) const 
{
  return glm::transpose(glm::inverse(matrix4()));
}
