/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: BoundingSphere.h
Purpose: Bounding sphere for debug draw
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

#ifndef ASSETMANAGER_H
#define ASSETMANAGER_H

#include "Model.h"
#include "Shader.h"
#include <map>
#include <string>

/**
 * @brief 
 *   Manages the assets
 */
class AssetManager
{
public:
  // Constructor a new Asset Manager
  AssetManager();

  // Deconstructor for the Asset Manager
  ~AssetManager() { Shutdown(); }

  // Initialize any assets that should be created on start up
  void Init();

  // Clean up any assets
  void Shutdown();

  // Getter for the all models that can be loaded for the project
  ModelPtr GetModel(Model::Index index);

  // Getter for the all shaders that can be loaded for the project
  ShaderPtr GetShader(ShaderIndex index);

  ShaderPtr ReloadShader(ShaderIndex index);

  /**
   * @brief 
   *   Getter for the map of models that are loaded
   * 
   * @return const std::map<std::string, ModelPtr>& 
   *   A const reference of the map of models
   */
  const std::map<std::string, ModelPtr>& Models(void) const { return m_Models; };

  /**
   * @brief 
   *   Getter for the map of shaders that are loaded
   * 
   * @return const std::map<std::string, ShaderPtr>& 
   *   A const reference of the map of shaders
   */
  const std::map<std::string, ShaderPtr>& Shaders(void) const { return m_Shaders; };

private:
  std::map<std::string, ModelPtr> m_Models;   //!< Map of models
  std::map<std::string, ShaderPtr> m_Shaders; //!< Map of shaders
};

#endif
