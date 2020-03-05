/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: AssetManager.h
Purpose: Manages the assets of the project. Currently Models and Shaders.
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

#include "AssetManager.h"
#include "Engine.h"

/**
 * @brief 
 *   Constructor a new Asset Manager
 */
AssetManager::AssetManager()
{
}

/**
 * @brief 
 *   Initialize any assets that should be created on start up
 */
void AssetManager::Init()
{
}

/**
 * @brief 
 *   Clean up any assets
 */
void AssetManager::Shutdown()
{
}

/**
 * @brief 
 *   Getter for the all models that can be loaded for the project 
 * 
 * @param index 
 *   Index of what model to return
 * 
 * @return ModelPtr 
 *   A shared pointer of the model so the call doesn't handle with the
 *   destructor
 */
ModelPtr AssetManager::GetModel(ModelIndex index)
{
  // checks if the model has already been loaded
  if (m_Models.find(ModelNames[index]) == m_Models.end()) 
  {
    m_Models.emplace(ModelNames[index], new Model(index)); // loads model
  }
  
  return m_Models[ModelNames[index]]; // returns the model
}

/**
 * @brief 
 *   Getter for the all shaders that can be loaded for the project 
 * 
 * @param index 
 *   Index of what shader to return
 * 
 * @return ShaderPtr 
 *   A shared pointer of the shader so the call doesn't handle with the
 *   destructor
 */
ShaderPtr AssetManager::GetShader(ShaderIndex index)
{
  // checks if the shader has already been loaded
  if (m_Shaders.find(ShaderNames[index]) == m_Shaders.end())
  {
    m_Shaders.emplace(ShaderNames[index], new Shader(index)); // load shader

    // temp block index and attach light data to the shader
    GLuint Local_block_index = 0;
    Local_block_index = glGetProgramResourceIndex(m_Shaders[ShaderNames[index]]->m_ProgramID, GL_SHADER_STORAGE_BLOCK, "LightData");

    // attach ssbo to the shader
    GLuint ssbo_binding_point_index = 0;
    glShaderStorageBlockBinding(m_Shaders[ShaderNames[index]]->m_ProgramID, Local_block_index, ssbo_binding_point_index);

    // save the SSBO uniform
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, ssbo_binding_point_index, Engine::get().m_RenderingManager.m_SSBOUniform);
  }

  return m_Shaders[ShaderNames[index]]; // returns the shader
}
