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

int windowWidth = 1024;
int windowHeight = 768;
const char* glsl_version = "#version 130";

int main()
{
  int exitCode = 0;
  Engine engine;
  Editor editor;

  engine.Init(windowWidth, windowHeight, "CS350 - Engine - Michael Ngo", exitCode);

  if (!engine.m_Window)
  {
    return exitCode;
  }

  editor.Init(engine.m_Window, glsl_version);

  srand(unsigned(time(NULL)));

  while (engine.m_IsRunning)
  {
    engine.PreRender();
    editor.PreRender();
    
    engine.Render();
    editor.Render();

    engine.PostRender();
    editor.PostRender();

    glfwSwapBuffers(engine.m_Window);
  }

  editor.Shutdown();
  engine.Shutdown();

  return 0;
}
