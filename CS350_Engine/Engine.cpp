/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: Engine.cpp
Purpose: manages all the managers
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
#include <fstream>

#include "Engine.h"

static Engine* s_Engine = nullptr; //!< engine pointer

/**
 * @brief 
 *   getter for the engine 
 * 
 * @return Engine& 
 *   A reference of the engine
 */
Engine& Engine::get()
{
  return *s_Engine;
}

/**
 * @brief 
 *   Constructor a new Engine object
 */
Engine::Engine() :
  m_Window(nullptr),
  m_IsRunning(true)
{
  s_Engine = this; // set static pointer
};

/**
 * @brief 
 *   Initialize engine components
 * 
 * @param windowWidth 
 *   engine starting width
 * 
 * @param windowHeight 
 *   engine starting height
 * 
 * @param windowName 
 *   engine starting name
 * 
 * @param exitCode 
 *   exit error code if there was an error
 */
void Engine::Init(int windowWidth, int windowHeight, const char* windowName, int& exitCode)
{
  // settin member variables
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
    {
      GLenum error_out;
      while ((error_out = glGetError()) != GL_NO_ERROR)
      {
        __debugbreak();
        printf("oof %i", error_out);
      }
    }
    m_AssetManager.Init();
    {
      GLenum error_out;
      while ((error_out = glGetError()) != GL_NO_ERROR)
      {
        __debugbreak();
        printf("oof %i", error_out);
      }
    }
    m_Debug.Init();
    {
      GLenum error_out;
      while ((error_out = glGetError()) != GL_NO_ERROR)
      {
        __debugbreak();
        printf("oof %i", error_out);
      }
    }
    m_RenderingManager.Init();
    {
      GLenum error_out;
      while ((error_out = glGetError()) != GL_NO_ERROR)
      {
        __debugbreak();
        printf("oof %i", error_out);
      }
    }
    m_SceneManager.Init();
  }
}

/**
 * @brief 
 *   shutdown the engine's components
 */
void Engine::Shutdown()
{
  // shutdown all managers
  m_SceneManager.Shutdown();
  m_RenderingManager.Shutdown();
  m_AssetManager.Shutdown();

  // close glfw window
  glfwDestroyWindow(m_Window);
  glfwTerminate();
}

/**
 * @brief 
 *   engine's prerender 
 */
void Engine::PreRender()
{
  // poll glfw events
  glfwPollEvents();

  // save window new width and height
  int width, height;
  glfwGetWindowSize(m_Window, &width, &height);

  //check if width or height is 0
  if (width == 0 || height == 0)
  {
    // dont update the engine
    glfwSwapBuffers(m_Window);
    return;
  }

  // resize viewport
  glViewport(0, 0, width, height);
  m_Width = width;
  m_Height = height;

  // calculate delta time from last frame
  float currentFrame = (float)glfwGetTime();
  m_DeltaTime = currentFrame - m_LastFrame;
  m_LastFrame = currentFrame;

  // process input
  m_Input.ProcessInput(m_DeltaTime * m_TimeScale);
  
  // check if engine should close 
  if (glfwWindowShouldClose(m_Window))
  {
    m_IsRunning = false;
  }

  // prerender all managers that need too
  m_RenderingManager.PreRender(m_SceneManager.m_CurrentScene);
}

/**
 * @brief 
 *   engine's render
 */
void Engine::Render()
{
  // render all managers that need too
  m_SceneManager.Update(m_DeltaTime * m_TimeScale);
  m_RenderingManager.Render(m_SceneManager.m_CurrentScene);
}

/**
 * @brief 
 *  engine's post render
 */
void Engine::PostRender()
{
  // post render all managers that need too
  m_RenderingManager.PostRender(m_SceneManager.m_CurrentScene);

  // draw debug information
  m_Debug.Update();
}
