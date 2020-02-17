/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: AssetManager.cpp
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

#include "AssetManager.h"
#include "Engine.h"

AssetManager::AssetManager()
{
}

void AssetManager::Init()
{
}

void AssetManager::Shutdown()
{
}

ModelPtr AssetManager::GetModel(ModelIndex index)
{
  if (m_Models.find(ModelNames[index]) == m_Models.end())
  {
    m_Models.emplace(ModelNames[index], new Model(index));
  }
  
  return m_Models[ModelNames[index]];
}

ShaderPtr AssetManager::GetShader(ShaderIndex index)
{
  if (m_Shaders.find(ShaderNames[index]) == m_Shaders.end())
  {
    m_Shaders.emplace(ShaderNames[index], new Shader(index));

    GLuint Local_block_index = 0;
    Local_block_index = glGetProgramResourceIndex(m_Shaders[ShaderNames[index]]->m_ProgramID, GL_SHADER_STORAGE_BLOCK, "LightData");

    GLuint ssbo_binding_point_index = 0;
    glShaderStorageBlockBinding(m_Shaders[ShaderNames[index]]->m_ProgramID, Local_block_index, ssbo_binding_point_index);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, Engine::get().m_RenderingManager.m_SSBOUniform);
  }

  return m_Shaders[ShaderNames[index]];
}
