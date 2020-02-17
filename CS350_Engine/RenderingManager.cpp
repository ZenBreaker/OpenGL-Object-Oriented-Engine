/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: RenderingManager.cpp
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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include <vector>
#include <iostream>

#include "RenderingManager.h"
#include "Engine.h"
extern int windowWidth;
extern int windowHeight;
static const unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
void renderQuad();
RenderingManager::RenderingManager()
{
}

void RenderingManager::Init()
{
  m_Lights.constant = 1.0f;
  m_Lights.linear = 0.7f;
  m_Lights.quadratic = 1.8f;
  m_Lights.ZFar = 100.0f;
  m_Lights.ZNear = 0.0001f;
  m_FSQ = FSQ::All;
  m_DepthCopy = true;

  glGenBuffers(1, &m_SSBOUniform);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBOUniform);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(LightData), &m_Lights, GL_DYNAMIC_COPY);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  glEnable(GL_DEPTH_TEST);

  glGenFramebuffers(1, &gBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
  // position color buffer
  glGenTextures(1, &gPosition);
  glBindTexture(GL_TEXTURE_2D, gPosition);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Engine::get().m_Width, Engine::get().m_Height, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosition, 0);
  // normal color buffer
  glGenTextures(1, &gNormal);
  glBindTexture(GL_TEXTURE_2D, gNormal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, Engine::get().m_Width, Engine::get().m_Height, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);
  // color + specular color buffer
  glGenTextures(1, &gAlbedoSpec);
  glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Engine::get().m_Width, Engine::get().m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedoSpec, 0);
  // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
  glDrawBuffers(3, attachments);
  // create and attach depth buffer (renderbuffer)
  glGenRenderbuffers(1, &rboDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, Engine::get().m_Width, Engine::get().m_Height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
  // finally check if framebuffer is complete
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  Engine::get().m_AssetManager.GetShader(ShaderIndex::GeometryShader);
  auto lightingPassShaderID = Engine::get().m_AssetManager.GetShader(ShaderIndex::LightingPass)->m_ProgramID;

  m_gPositionUniform = glGetUniformLocation(lightingPassShaderID, "gPosition");
  m_gNormalUniform = glGetUniformLocation(lightingPassShaderID, "gNormal");
  m_gAlbedoSpecUniform = glGetUniformLocation(lightingPassShaderID, "gAlbedoSpec");
}

void RenderingManager::Shutdown()
{
}

void RenderingManager::PreRender(const Scene* scene)
{
  glClearColor(0.5f, 0.0f, 0.5f, 1.0f); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
  
  glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  m_View = scene->m_Camera.GetViewMatrix();
  m_Projection = glm::perspective(glm::radians(90.0f), (float)windowWidth / (float)windowHeight, 0.0001f, 100.0f);
  glViewport(0, 0, windowWidth, windowHeight);
  for (int i = 0; i < scene->m_Objects.size(); ++i)
  {
    RenderObject(scene, scene->m_Objects[i]);
  }

  glBindFramebuffer(GL_FRAMEBUFFER, 0);
  glViewport(0, 0, Engine::get().m_Width, Engine::get().m_Height);
}

void RenderingManager::Render(const Scene* scene)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  
  if (m_FSQ != FSQ::All)
  {
    glUseProgram(Engine::get().m_AssetManager.GetShader(ShaderIndex::TextureShader)->m_ProgramID);
    switch (m_FSQ)
    {
    case Position:
      glUniform1i(m_gPositionUniform, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, gPosition);
      break;
    case Normal:
      glUniform1i(m_gNormalUniform, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, gNormal);
      break;
    case AlbedoSpec:
      glUniform1i(m_gAlbedoSpecUniform, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
      break;
    default:
      break;
    }
  }
  else
  {
    glUseProgram(Engine::get().m_AssetManager.GetShader(ShaderIndex::LightingPass)->m_ProgramID);
    
    glUniform1i(m_gPositionUniform, 0);
    glUniform1i(m_gNormalUniform, 1);
    glUniform1i(m_gAlbedoSpecUniform, 2);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gAlbedoSpec);
  }

  m_Lights.numberOfLights = (int)scene->m_Lights.size();
  for (int i = 0; i < scene->m_Lights.size(); ++i)
  {
    m_Lights.lights[i] = *scene->m_Lights[i].m_Light;
  }
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBOUniform);
  GLvoid* pLocal = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  memcpy(pLocal, &m_Lights, sizeof(LightData));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
  m_Projection = glm::perspective(glm::radians(90.0f), (float)Engine::get().m_Width / (float)Engine::get().m_Height, 0.0001f, 100.0f);
  // finally render quad
  renderQuad();
}

void RenderingManager::PostRender(const Scene* scene)
{
  if (m_DepthCopy)
  {
    glBindFramebuffer(GL_READ_FRAMEBUFFER, gBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
    // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
    // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
    // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, Engine::get().m_Width, Engine::get().m_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  

  for (int i = 0; i < scene->m_Lights.size(); ++i)
  {
    RenderObject(scene, scene->m_Lights[i]);
  }
}

void RenderingManager::RenderObject(const Scene* scene, const Object& object)
{
  glUseProgram(object.m_Shader->m_ProgramID);

  glBindVertexArray(object.m_Model->m_VAO);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, object.m_Model->m_VBO);
  glVertexAttribPointer(0,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void*)0);
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, object.m_Model->m_VNBO);
  glVertexAttribPointer(1,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void *)0);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.m_Model->m_EBO);

  glUniformMatrix4fv(object.m_ViewMatrixUniform, 1, GL_FALSE, &m_View[0][0]);
  glUniformMatrix4fv(object.m_PerspectiveMatrixUniform, 1, GL_FALSE, &m_Projection[0][0]);
  glUniformMatrix4fv(object.m_ModelMatrixUniform, 1, GL_FALSE, &object.matrix4()[0][0]);
  glUniformMatrix4fv(object.m_NormalMatrixUniform, 1, GL_FALSE, &object.normalMatrix()[0][0]);

  glm::vec4 eye(scene->m_Camera.m_Position, 1.0f);
  glUniform4fv(object.m_EyePositionUniform, 1, &eye[0]);

  glUniform3fv(object.m_AmbiantColorUniform, 1, &object.m_Material.ambiant_color[0]);

  glDrawElements(object.m_Model->m_DrawMode, (GLsizei)object.m_Model->m_Indices.size(), GL_UNSIGNED_INT, NULL);

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
  if (quadVAO == 0)
  {
    float quadVertices[] = {
      // positions        // texture Coords
      -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
      -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
       1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
       1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };
    // setup plane VAO
    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);
    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
  }
  glDepthMask(GL_FALSE);
  glBindVertexArray(quadVAO);
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
  glBindVertexArray(0);
  glDepthMask(GL_TRUE);
}