/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: RenderingManager.h
Purpose: manages the renderer
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
#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#include "Scene.h"
#include "Object.h"

enum FSQ
{
  All,
  Position,
  Normal,
  AlbedoSpec,
};

static const char* const FSQNames[] =
{
  "All",
  "Position",
  "Normal",
  "AlbedoSpec",
};

struct LightData
{
  Light lights[16];

  int numberOfLights;
  float constant;
  float linear;
  float quadratic;

  glm::vec3 GlobalAmbient;
  float ZFar;

  glm::vec3 Atmospheric;
  float ZNear;

  glm::vec4 IFog;
};

class RenderingManager
{
public:
  RenderingManager();
  ~RenderingManager() { Shutdown(); }
  void Init();
  void Shutdown();
  void PreRender(const Scene* scene);
  void Render(const Scene* scene);
  void PostRender(const Scene* scene);

  GLuint m_SSBOUniform;
  LightData m_Lights;
  FSQ m_FSQ;
  bool m_DepthCopy;

  glm::mat4 m_Projection;
  glm::mat4 m_View;
private:
  void RenderObject(const Scene* scene, const Object& object);

  glm::vec4 m_Eye;
  unsigned int gBuffer, gPosition, gNormal, gAlbedoSpec, rboDepth;
  GLuint m_gPositionUniform, m_gNormalUniform, m_gAlbedoSpecUniform;
};

#endif
