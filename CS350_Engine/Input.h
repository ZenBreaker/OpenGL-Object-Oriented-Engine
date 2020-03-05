/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: Input.h
Purpose: manages input
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
#ifndef INPUT_H
#define INPUT_H

#include <glew-2.1.0/GL/glew.h>
#include <glfw-3.3.bin.WIN64/GLFW/glfw3.h>

/**
 * @brief 
 *  manages the input for the program
 */
class Input
{
public:
  // Constructor a new Input object
  Input();

  // initialize glfw window
  void Init(GLFWwindow * window);

  // process input that isnt a callback
  void ProcessInput(float deltaTime);

  bool m_FirstMouse;     //!< variable for clicking and draging
  GLFWwindow * m_Window; //!< glfw window
  float m_LastX;         //!< last mouse x position
  float m_LastY;         //!< last mouse y position

private:
};

#endif