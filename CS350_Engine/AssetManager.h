/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: AssetManager.h
Purpose: manage assets
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
#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Model.h"
#include "Shader.h"
#include <map>
#include <string>

class AssetManager
{
public:
  AssetManager();
  ~AssetManager() { Shutdown(); }

  void Init();
  void Shutdown();
  ModelPtr GetModel(ModelIndex index);
  ShaderPtr GetShader(ShaderIndex index);

  const std::map<std::string, ModelPtr>& Models(void) const { return m_Models; };
  const std::map<std::string, ShaderPtr>& Shaders(void) const { return m_Shaders; };

private:
  std::map<std::string, ModelPtr> m_Models;
  std::map<std::string, ShaderPtr> m_Shaders;
};

#endif
