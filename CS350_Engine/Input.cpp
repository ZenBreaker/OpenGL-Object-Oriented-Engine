/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Input.cpp
Purpose: manages input
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
#include "imgui/imgui.h"
#include "Input.h"
#include "Engine.h"

void MouseCallback(GLFWwindow* window, double xpos, double ypos);

Input::Input()
{
  m_FirstMouse = false;
}

void Input::Init(GLFWwindow * window)
{
  m_Window = window;
  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(m_Window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
}

void Input::ProcessInput(float deltaTime)
{
  auto& engine = Engine::get();

  if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
  {
    glfwSetWindowShouldClose(m_Window, true);
  }

  if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
  {
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    GLFWcursorposfun oldCallback = glfwSetCursorPosCallback(m_Window, MouseCallback);

    if (oldCallback == nullptr)
    {
      m_FirstMouse = true;
    }
  }
  if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
  {
    glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    m_FirstMouse = false;
    glfwSetCursorPosCallback(m_Window, nullptr);
  }

  if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
    engine.m_SceneManager.m_CurrentScene->m_Camera.Update(FORWARD, deltaTime);
  if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
    engine.m_SceneManager.m_CurrentScene->m_Camera.Update(BACKWARD, deltaTime);
  if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
    engine.m_SceneManager.m_CurrentScene->m_Camera.Update(LEFT, deltaTime);
  if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
    engine.m_SceneManager.m_CurrentScene->m_Camera.Update(RIGHT, deltaTime);
}

void MouseCallback(GLFWwindow* window, double xpos, double ypos)
{
  if (ImGui::GetIO().WantCaptureMouse)
  {
    return;
  }

  auto& input = Engine::get().m_Input;
  if (input.m_FirstMouse)
  {
    input.m_LastX = xpos;
    input.m_LastY = ypos;
    input.m_FirstMouse = false;
  }

  float xoffset = xpos - input.m_LastX;
  float yoffset = input.m_LastY - ypos; // reversed since y-coordinates go from bottom to top

  input.m_LastX = xpos;
  input.m_LastY = ypos;

  Engine::get().m_SceneManager.m_CurrentScene->m_Camera.MouseMovement(xoffset, yoffset);
}