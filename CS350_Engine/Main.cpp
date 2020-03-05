/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Main.cpp
Purpose: start of program and main loop
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
#include <glew-2.1.0/GL/glew.h>

#include <glfw-3.3.bin.WIN64/GLFW/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/color_space.hpp>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include "Engine.h"
#include "Editor.h"

int windowWidth = 1024;                    //!< default window width
int windowHeight = 768;                    //!< default window height
const char *glsl_version = "#version 130"; //!< default glsl version

int main()
{
  int exitCode = 0; // exit code

  // create engine 
  Engine engine;

  // create editor
  Editor editor;

  // initialize engine
  engine.Init(windowWidth, windowHeight, "CS350 - Engine - Michael Ngo", exitCode);

  // check if glfw window was made
  if (!engine.m_Window)
  {
    return exitCode; // exit out of main
  }

  // initialize editor
  editor.Init(engine.m_Window, glsl_version);

  // start PRNG
  srand(unsigned(time(NULL)));

  // keep looping wihle the engine is running
  while (engine.m_IsRunning)
  {
    // PreRender
    engine.PreRender();
    editor.PreRender();

    // Render
    engine.Render();
    editor.Render();

    // PostRender
    engine.PostRender();
    editor.PostRender();

    // Swap glfw frame buffers
    glfwSwapBuffers(engine.m_Window);
  }

  // shutdow editor then engine
  editor.Shutdown();
  engine.Shutdown();

  // exit cleanly
  return 0;
}
