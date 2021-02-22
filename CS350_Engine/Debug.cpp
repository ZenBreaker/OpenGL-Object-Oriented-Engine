/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
File Name: Debug.cpp
Purpose: Debug Manager. Collects requests of debug calls and draws them at the end of a frame
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
Author: Michael Ngo, michael.ngo
Creation date: 2/28/2020
End Header --------------------------------------------------------*/

#include "Debug.h"
#include <glew-2.1.0/GL/glew.h>

#include <glfw-3.3.bin.WIN64/GLFW/glfw3.h>
#include "includes/glm/glm.hpp"

#include "Engine.h"

//! const NDC box vertices
const float BoxVertices[] = { 
  // positions        
  -0.5f,  0.5f, 0.5f,    //bottom right forward
  0.5f, 0.5f, 0.5f,      //top right forward
  -0.5f,  -0.5f, 0.5f,   //bottom left forward
  0.5f, -0.5f, 0.5f,     //top left forward
  -0.5f, 0.5f, -0.5f,    //bottom right back
  0.5f, 0.5f, -0.5f,     //top right back
  -0.5f, -0.5f, -0.5f,   //bottom left back
  0.5f, -0.5f, -0.5f,    //top left back
};

const std::vector<unsigned int> BoxIndices = {
  0, 1, 2, 3, 1, 3, 0, 2, 4, 5, 6, 7, 5, 7, 4, 6, 0, 4, 1, 5, 2, 6, 3, 7
  };

const float LineVertices[] = {
  // positions        
  1.0f, 0.0f, 0.0f
};

/**
 * @brief 
 *   Constructor a new Debug Manager
 */
Debug::Debug()
{
}

/**
 * @brief 
 *   Destructor for the Debug Manager
 */
Debug::~Debug()
{
  glDeleteBuffers(1, &m_EBOStaticSphere);
  glDeleteBuffers(1, &m_EBOStatic3DRect);
  glDeleteBuffers(1, &m_VBO);
  glDeleteVertexArrays(1, &m_VAO);
}

/**
 * @brief 
 *   Initialize any debug information that should be created on start up
 */
void Debug::Init()
{
  // generate vao and buffers
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  {
    GLenum error_out;
    while ((error_out = glGetError()) != GL_NO_ERROR)
    {
      __debugbreak();
      printf("oof %i", error_out);
    }
  }
  glGenBuffers(1, &m_EBOStatic3DRect);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOStatic3DRect);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, BoxIndices.size() * sizeof(unsigned int), BoxIndices.data(), GL_STREAM_DRAW);
  {
    GLenum error_out;
    while ((error_out = glGetError()) != GL_NO_ERROR)
    {
      __debugbreak();
      printf("oof %i", error_out);
    }
  }
  auto sphere = Engine::get().m_AssetManager.GetModel(Model::Sphere);
  glGenBuffers(1, &m_EBOStaticSphere);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBOStaticSphere);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sphere->m_Indices.size() * sizeof(unsigned int), sphere->m_Indices.data(), GL_STREAM_DRAW);
  {
    GLenum error_out;
    while ((error_out = glGetError()) != GL_NO_ERROR)
    {
      __debugbreak();
      printf("oof %i", error_out);
    }
  }
  // get uniforms from debug shader
  m_ViewUniform = glGetUniformLocation(Engine::get().m_AssetManager.GetShader(ShaderIndex::ColorShader)->m_ProgramID, "ViewMatrix");
  
      {
    GLenum error_out;
    while ((error_out = glGetError()) != GL_NO_ERROR)
    {
      __debugbreak();
      printf("oof %i", error_out);
    }
  }
  m_PerspectiveUniform = glGetUniformLocation(Engine::get().m_AssetManager.GetShader(ShaderIndex::ColorShader)->m_ProgramID, "PerspectiveMatrix");
  
      {
    GLenum error_out;
    while ((error_out = glGetError()) != GL_NO_ERROR)
    {
      __debugbreak();
      printf("oof %i", error_out);
    }
  }
  m_ModelUniform = glGetUniformLocation(Engine::get().m_AssetManager.GetShader(ShaderIndex::ColorShader)->m_ProgramID, "ModelMatrix");
  {
    GLenum error_out;
    while ((error_out = glGetError()) != GL_NO_ERROR)
    {
      __debugbreak();
      printf("oof %i", error_out);
    }
  }
}

/**
 * @brief 
 *   draw all queued debug calls
 */
void Debug::Update()
{
  // use debug shader
  glUseProgram(Engine::get().m_AssetManager.GetShader(ShaderIndex::ColorShader)->m_ProgramID);

  // attach vao and buffers
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

  // enable attribute location 0 and 1
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(DebugPoint), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(DebugPoint), (void*)offsetof(DebugPoint, color));

  // Set matrix uniforms for the shader 
  glUniformMatrix4fv(m_ViewUniform, 1, GL_FALSE, &Engine::get().m_RenderingManager.m_View[0][0]);
  glUniformMatrix4fv(m_PerspectiveUniform, 1, GL_FALSE, &Engine::get().m_RenderingManager.m_Projection[0][0]);
  glm::mat4 identity(1);
  glUniformMatrix4fv(m_ModelUniform, 1, GL_FALSE, &identity[0][0]);

  GLuint lastBindedEBO = -1;

  // loop through every queued debug draw request
  for (int i = 0; i < m_Queue.size(); ++i)
  {
    // set buffer data
    glBufferData(GL_ARRAY_BUFFER, m_Queue[i].points.size() * sizeof(DebugPoint), m_Queue[i].points.data(), GL_STREAM_DRAW);

    if(lastBindedEBO != m_Queue[i].EBO.id)
    {
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Queue[i].EBO.id);
      lastBindedEBO = m_Queue[i].EBO.id;
    }
    
    // debug draw 
    glDrawElements(GL_LINES, m_Queue[i].EBO.size, GL_UNSIGNED_INT, NULL);
  }

  // unbind vao
  glBindVertexArray(0); 

  // clear m_Queue
  m_Queue.clear();
}

/**
 * @brief 
 *   draw debug points 
 * 
 * @param points 
 *   points to draw
 * 
 * @param depthEnabled 
 *   depth dependent, (world or screen space)
 */
void Debug::drawPoints(std::vector<glm::vec3> points, bool depthEnabled)
{
}

/**
 * @brief 
 *   draw debug world lines 
 * 
 * @param lines 
 *   lines to draw
 * 
 * @param depthEnabled 
 *   depth dependent
 */
void Debug::drawWorldLines(std::vector<Line> lines, bool depthEnabled)
{
  /*
  // temp data object
  DrawData data;

  // reserve space for size of box points
  data.points.reserve(2);

  for (int i = 0; i < std::size(LineVertices); ++i)
  {
    // NDC * scale + translate
    const glm::vec3 point{ LineVertices[i] * lines.width + rect.Center.x, LineVertices[i + 1] * rect.height + rect.Center.y, LineVertices[i + 2] * rect.depth + rect.Center.z };
    data.points.emplace_back(point);
  }

  // Set the ebo of the debug object
  data.EBO.id = m_EBOStatic3DRect;
  data.EBO.size = (GLuint)BoxIndices.size();

  // set debug info
  data.depthEnable = depthEnable;
  data.fill = false;


  // m_Queue box to draw later
  m_Queue.emplace_back(data);
  */
}

/**
 * @brief 
 *   draw debug screen rects
 * 
 * @param rects 
 *   rects to draw
 * 
 * @param depthEnabled 
 *   depth dependent
 */
void Debug::drawScreenRects(std::vector<Rect2D> rects, bool depthEnable)
{
}

/**
 * @brief 
 *   draw debug a rect in world space
 * 
 * @param rect 
 *   rect to draw
 * 
 * @param r 
 *   red color
 * 
 * @param g 
 *   green color
 * 
 * @param b 
 *   blue color
 * 
 * @param depthEnable 
 *   depth dependent
 */
void Debug::drawWorldRects(const Rect3D &rect, const glm::vec3 & color, bool depthEnable)
{
  // temp data object
  DrawData data;

  // reserve space for size of box points
  data.points.reserve(8);

  // loop though 8 verties and scale and position the vertex in the world
  for (int i = 0; i < std::size(BoxVertices); i+=3)
  {
    // NDC * scale + translate
    const glm::vec3 point{ BoxVertices[i] * rect.width + rect.Center.x, BoxVertices[i + 1] * rect.height + rect.Center.y, BoxVertices[i + 2] * rect.depth + rect.Center.z };
    data.points.emplace_back(point, color);
  }

  // Set the ebo of the debug object
  data.EBO.id = m_EBOStatic3DRect;
  data.EBO.size = (GLuint)BoxIndices.size();

  // set debug info
  data.depthEnable = depthEnable;
  data.fill = false;


  // m_Queue box to draw later
  m_Queue.emplace_back(data);
}

/**
 * @brief 
 *   debug draw a sphere in world space
 * 
 * @param center 
 *   center of the sphere
 * 
 * @param radius 
 *   radius of the sphere
 * 
 * @param depthEnable 
 *   depth dependent
 */
void Debug::drawWorldSphere(glm::vec3 center, float radius, bool depthEnable)
{
  // grab sphere model
  auto sphere = Engine::get().m_AssetManager.GetModel(Model::Sphere);

  // temp data 
  DrawData data;

  // reserves size of sphere vertices
  data.points.reserve(sphere->m_Vertices.size());

  // loop through each vertex
  for(unsigned i = 0; i < sphere->m_Vertices.size(); ++i)
  {
    // add vertex to points, NDC * scale + translate
    data.points.emplace_back(sphere->m_Vertices[i] * radius + center);
  }

  // Set the ebo of the debug object
  data.EBO.id = m_EBOStaticSphere;
  data.EBO.size = (GLuint)sphere->m_Indices.size();
  
  // set debug info
  data.depthEnable = depthEnable;
  data.fill = false;

  // m_Queue sphere to draw later
  m_Queue.emplace_back(data);
}
