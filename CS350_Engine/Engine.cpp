/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Engine.cpp
Purpose: manages all the managers
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
#include <fstream>

#include "Engine.h"

static Engine* s_Engine = nullptr;

Engine& Engine::get()
{
  return *s_Engine;
}

Engine::Engine() :
  m_Window(nullptr),
  m_IsRunning(true)
{
  s_Engine = this;
};

void Engine::Init(int windowWidth, int windowHeight, const char* windowName, int& exitCode)
{
  m_Width = windowWidth;
  m_Height = windowHeight;
  m_DeltaTime = 0.0f;
  m_TimeScale = 1.0f;
  m_LastFrame = 0.0f;

  //Init OpenGL
  {
    // Initialise GLFW
    if (!glfwInit())
    {
      fprintf(stderr, "Failed to initialize GLFW\n");
      getchar();
      exitCode = -1;
      m_Window = nullptr;
    }

    // Setting up OpenGL properties
    glfwWindowHint(GLFW_SAMPLES, 0); // change for anti-aliasing
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Window = glfwCreateWindow(windowWidth, windowHeight, // window dimensions
      windowName, // window title
      nullptr, // which monitor (if full-screen mode)
      nullptr); // if sharing context with another window

    if (m_Window == nullptr)
    {
      fprintf(stderr,
        "Failed to open GLFW window. If you have an Intel GPU, they are not 4.0 compatible.\n");
      getchar();
      glfwTerminate();
      exitCode = -2;
      m_Window = nullptr;
    }

    // OpenGL resource model - "glfwCreateWindow" creates the necessary data storage for the OpenGL
    // context but does NOT make the created context "current". We MUST make it current with the following
    // call
    glfwMakeContextCurrent(m_Window);

    // Initialize GLEW
    glewExperimental = static_cast<GLboolean>(true); // Needed for core profile
    if (glewInit() != GLEW_OK)
    {
      fprintf(stderr, "Failed to initialize GLEW\n");
      getchar();
      glfwTerminate();
      exitCode = -3;
      m_Window = nullptr;
    }
  }
  
  //Init Managers
  {
    m_Input.Init(m_Window);
    m_AssetManager.Init();
    m_Debug.Init();
    m_RenderingManager.Init();
    m_SceneManager.Init();
  }
}

void Engine::Shutdown()
{
  m_SceneManager.Shutdown();
  m_RenderingManager.Shutdown();
  m_AssetManager.Shutdown();

  glfwDestroyWindow(m_Window);
  glfwTerminate();
}

void Engine::PreRender()
{
  int width, height;
  glfwPollEvents();
  glfwGetWindowSize(m_Window, &width, &height);
  if (width == 0 || height == 0)
  {
    glfwSwapBuffers(m_Window);
    return;
  }
  glViewport(0, 0, width, height);
  m_Width = width;
  m_Height = height;
  float currentFrame = (float)glfwGetTime();
  m_DeltaTime = currentFrame - m_LastFrame;
  m_LastFrame = currentFrame;

  m_Input.ProcessInput(m_DeltaTime * m_TimeScale);
  
  if (glfwWindowShouldClose(m_Window))
  {
    m_IsRunning = false;
  }

  m_RenderingManager.PreRender(m_SceneManager.m_CurrentScene);
}

void Engine::Render()
{
  m_SceneManager.Update(m_DeltaTime * m_TimeScale);
  m_RenderingManager.Render(m_SceneManager.m_CurrentScene);
}

void Engine::PostRender()
{
  m_RenderingManager.PostRender(m_SceneManager.m_CurrentScene);
  m_Debug.Update();
}
