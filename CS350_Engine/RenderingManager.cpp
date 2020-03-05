/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
File Name: RenderingManager.cpp
Purpose: Manages the rendering of objects. Uses Deferred Rendering
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
Author: Michael Ngo
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

// initialized default in main
extern int windowWidth;
extern int windowHeight;

// static attachments for deferred rendering
static const unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };

// static full size quad
const float quadVertices[] = {
  // positions        // texture Coords
  -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
  -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
   1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
   1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
};

RenderingManager::RenderingManager()
{
}

void RenderingManager::Init()
{
  // initialized member variables 
  m_Lights.constant = 1.0f;
  m_Lights.linear = 0.7f;
  m_Lights.quadratic = 1.8f;
  m_Lights.ZFar = 100.0f;
  m_Lights.ZNear = 0.0001f;
  m_FSQ = GeometryBuffer::All;
  m_DepthCopy = true;

  // Generate SSBO for Deferred Rendering
  glGenBuffers(1, &m_SSBOUniform);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBOUniform);
  glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(LightData), &m_Lights, GL_DYNAMIC_COPY);
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

  // Activate Depth Test
  glEnable(GL_DEPTH_TEST);

  // Generate geometry framebuffer for Deferred Rendering
  glGenFramebuffers(1, &m_GeometryBuffer);
  glBindFramebuffer(GL_FRAMEBUFFER, m_GeometryBuffer);

  // Get application width and height
  const int width = Engine::get().m_Width;
  const int height = Engine::get().m_Height;

  // position color buffer
  glGenTextures(1, &m_GeometryPosition);
  glBindTexture(GL_TEXTURE_2D, m_GeometryPosition);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_GeometryPosition, 0);

  // normal color buffer
  glGenTextures(1, &m_GeometryNormal);
  glBindTexture(GL_TEXTURE_2D, m_GeometryNormal);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, width, height, 0, GL_RGB, GL_FLOAT, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_GeometryNormal, 0);

  // color + specular color buffer
  glGenTextures(1, &m_GeometryAlbedoSpec);
  glBindTexture(GL_TEXTURE_2D, m_GeometryAlbedoSpec);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_GeometryAlbedoSpec, 0);

  // tell OpenGL which color attachments we'll use (of this framebuffer) for rendering 
  glDrawBuffers(3, attachments);

  // create and attach depth buffer (renderbuffer)
  glGenRenderbuffers(1, &m_RenderBufferObjectDepth);
  glBindRenderbuffer(GL_RENDERBUFFER, m_RenderBufferObjectDepth);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_RenderBufferObjectDepth);

  // finally check if framebuffer is complete
  if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    std::cout << "Framebuffer not complete!" << std::endl;
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // set lighting data uniforms for Deferred Rendering
  const auto lightingPassShaderID = Engine::get().m_AssetManager.GetShader(ShaderIndex::LightingPass)->m_ProgramID;
  m_GeometryPositionUniform = glGetUniformLocation(lightingPassShaderID, "gPosition");
  m_GeometryNormalUniform = glGetUniformLocation(lightingPassShaderID, "gNormal");
  m_GeometryAlbedoSpecUniform = glGetUniformLocation(lightingPassShaderID, "gAlbedoSpec");

  // setup plane VAO
  glGenVertexArrays(1, &m_FullQuadVAO);
  glGenBuffers(1, &m_FullQuadVBO);
  glBindVertexArray(m_FullQuadVAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_FullQuadVBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
}

void RenderingManager::Shutdown()
{
  // delete all buffers
  glDeleteBuffers(1, &m_FullQuadVBO);
  glDeleteBuffers(1, &m_SSBOUniform);
  glDeleteVertexArrays(1, &m_FullQuadVAO);
}

void RenderingManager::PreRender(const Scene* scene)
{
  // clear framebuffer
  glClearColor(0.5f, 0.0f, 0.5f, 1.0f); 
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 

  // bind geometry buffer to capture lighting data
  glBindFramebuffer(GL_FRAMEBUFFER, m_GeometryBuffer);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // get view and projection matrices
  m_View = scene->m_Camera.GetViewMatrix();
  m_Projection = glm::perspective(glm::radians(90.0f), (float)windowWidth / (float)windowHeight, 0.0001f, 100.0f);

  //set viewport to default size
  glViewport(0, 0, windowWidth, windowHeight);

  // save binded program id
  GLuint lastBindedProgramID = -1;

  // loop through all objects for a first pass
  for (int i = 0; i < scene->m_Objects.size(); ++i)
  {
    RenderObject(scene, scene->m_Objects[i], lastBindedProgramID);
  }

  // unbind the geometry buffer
  glBindFramebuffer(GL_FRAMEBUFFER, 0);

  // set the viewport back to application width and height
  glViewport(0, 0, Engine::get().m_Width, Engine::get().m_Height);
}

void RenderingManager::Render(const Scene* scene)
{
  // clear color
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // check which geometry Buffer to show
  if (m_FSQ != GeometryBuffer::All)
  {
    // Use Texture Shader to render a full screen quad
    glUseProgram(Engine::get().m_AssetManager.GetShader(ShaderIndex::TextureShader)->m_ProgramID);

    switch (m_FSQ)
    {
    case Position:
    {
      glUniform1i(m_GeometryPositionUniform, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m_GeometryPosition);
      break;
    }
      
    case Normal:
    {
      glUniform1i(m_GeometryNormalUniform, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m_GeometryNormal);
      break;
    }
    case AlbedoSpec:
    {
      glUniform1i(m_GeometryAlbedoSpecUniform, 0);
      glActiveTexture(GL_TEXTURE0);
      glBindTexture(GL_TEXTURE_2D, m_GeometryAlbedoSpec);
      break;
    }
    }
  }
  else // render the phong lighting 
  {
    // use the second pass to calculate color 
    glUseProgram(Engine::get().m_AssetManager.GetShader(ShaderIndex::LightingPass)->m_ProgramID);

    // set textures for the program to read from 
    glUniform1i(m_GeometryPositionUniform, 0);
    glUniform1i(m_GeometryNormalUniform, 1);
    glUniform1i(m_GeometryAlbedoSpecUniform, 2);

    // enable and bind each texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, m_GeometryPosition);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, m_GeometryNormal);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, m_GeometryAlbedoSpec);
  }

  // set the number of lights in the scene
  m_Lights.numberOfLights = (int)scene->m_Lights.size();

  // loop through and grab updated light data
  for (int i = 0; i < scene->m_Lights.size(); ++i)
  {
    m_Lights.lights[i] = *scene->m_Lights[i].m_Light;
  }

  // bind SSBO
  glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_SSBOUniform);

  // write light data to the SSBO
  GLvoid* pLocal = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
  memcpy(pLocal, &m_Lights, sizeof(LightData));
  glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

  // use projection matrix from application
  m_Projection = glm::perspective(glm::radians(90.0f), (float)Engine::get().m_Width / (float)Engine::get().m_Height, 0.0001f, 100.0f);

  // finally render quad
  RenderQuad();
}

void RenderingManager::PostRender(const Scene* scene)
{
  // if using depth copy
  if (m_DepthCopy)
  {
    // copying the depth buffer values to the last binded framebuffer
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GeometryBuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0); // write to default framebuffer
    // blit to default framebuffer. Note that this may or may not work as the internal formats of both the FBO and default framebuffer have to match.
    // the internal formats are implementation defined. This works on all of my systems, but if it doesn't on yours you'll likely have to write to the 		
    // depth buffer in another shader stage (or somehow see to match the default framebuffer's internal format with the FBO's internal format).
    glBlitFramebuffer(0, 0, windowWidth, windowHeight, 0, 0, Engine::get().m_Width, Engine::get().m_Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
  }
  
  // save binded program id
  GLuint lastBindedProgramID = -1;

  // loop through all Lights to show where they are in the scene
  for (int i = 0; i < scene->m_Lights.size(); ++i)
  {
    RenderObject(scene, scene->m_Lights[i], lastBindedProgramID);
  }
}

void RenderingManager::RenderObject(const Scene* scene, const Object& object, GLuint & lastBindedProgramID)
{
  // check if last program id is the same as the current id
  if(lastBindedProgramID != object.m_Shader->m_ProgramID)
  {
    // use the new program id
    glUseProgram(object.m_Shader->m_ProgramID);
    lastBindedProgramID = object.m_Shader->m_ProgramID;
  }

  // bind the model's vao
  glBindVertexArray(object.m_Model->m_VAO);

  // enable vertex positions
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, object.m_Model->m_VBO);
  glVertexAttribPointer(0,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void*)0);

  // enable vertex normals
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, object.m_Model->m_VNBO);
  glVertexAttribPointer(1,
    3,
    GL_FLOAT,
    GL_FALSE,
    0,
    (void *)0);

  // bind index buffer
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, object.m_Model->m_EBO);

  // set uniforms 
  glUniformMatrix4fv(object.m_ViewMatrixUniform, 1, GL_FALSE, &m_View[0][0]);
  glUniformMatrix4fv(object.m_PerspectiveMatrixUniform, 1, GL_FALSE, &m_Projection[0][0]);
  glUniformMatrix4fv(object.m_ModelMatrixUniform, 1, GL_FALSE, &object.matrix4()[0][0]);
  glUniformMatrix4fv(object.m_NormalMatrixUniform, 1, GL_FALSE, &object.normalMatrix()[0][0]);

  glm::vec4 eye(scene->m_Camera.m_Position, 1.0f);
  glUniform4fv(object.m_EyePositionUniform, 1, &eye[0]);

  glUniform3fv(object.m_AmbiantColorUniform, 1, &object.m_Material.ambiant_color[0]);

  // draw object 
  glDrawElements(object.m_Model->m_DrawMode, (GLsizei)object.m_Model->m_Indices.size(), GL_UNSIGNED_INT, NULL);

  // disable attributes
  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}

void RenderingManager::RenderQuad() const 
{
  // disable depth
  glDepthMask(GL_FALSE);

  // bind full screen quad vao
  glBindVertexArray(m_FullQuadVAO);

  // draw full screen quad
  glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

  // unbind vao
  glBindVertexArray(0);

  // re-enable depth 
  glDepthMask(GL_TRUE);
}