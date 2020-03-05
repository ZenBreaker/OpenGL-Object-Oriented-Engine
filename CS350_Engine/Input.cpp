/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: Input.cpp
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
#include "imgui/imgui.h"
#include "Input.h"
#include "Engine.h"

/**
 * @brief 
 *   Constructor a new Input object
 */
Input::Input()
{
  // set constructor member variables
  m_FirstMouse = false;
}

/**
 * @brief 
 *   initialize glfw window
 * 
 * @param window 
 *   glfw window
 */
void Input::Init(GLFWwindow * window)
{
  // set window member variable
  m_Window = window;

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(m_Window, GLFW_STICKY_KEYS, GL_TRUE);
  glfwSetInputMode(m_Window, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);
}

/**
 * @brief 
 *   Callback if the mouse has moved
 * 
 * @param window 
 *   glfw window that was callbacked from
 * 
 * @param xpos
 *   new x position of the mouse
 *  
 * @param ypos 
 *   new y position of the mouse
 */
void MouseCallback(GLFWwindow *window, double xpos, double ypos)
{
  // if clicking on imgui
  if (ImGui::GetIO().WantCaptureMouse)
  {
    return; // dont update
  }

  // get input
  auto &input = Engine::get().m_Input;

  // if its the first time for the application
  if (input.m_FirstMouse)
  {
    // set the initial values
    input.m_LastX = (float)xpos;
    input.m_LastY = (float)ypos;

    // never comeback inside
    input.m_FirstMouse = false;
  }

  // calculate the difference in mouse movement
  float xoffset = (float)xpos - input.m_LastX;
  float yoffset = input.m_LastY - (float)ypos; // reversed since y-coordinates go from bottom to top

  // update last positions
  input.m_LastX = (float)xpos;
  input.m_LastY = (float)ypos;

  // update camera movements
  Engine::get().m_SceneManager.m_CurrentScene->m_Camera.MouseMovement(xoffset, yoffset);
}

/**
 * @brief 
 *   process input that isnt a callback
 * 
 * @param deltaTime 
 *   delta time of the engine
 */
void Input::ProcessInput(float deltaTime)
{
  // get camera from the scene
  auto &camera = Engine::get().m_SceneManager.m_CurrentScene->m_Camera;

  // click and drag movement
  {
    // check if application close button was pressed
    if (glfwGetKey(m_Window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
      // call glfw window to shutdown
      glfwSetWindowShouldClose(m_Window, true);
    }

    // check if application left mouse click was pressed
    if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
      // remove the cursor from the screen
      glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

      // call callback for cursor position
      GLFWcursorposfun oldCallback = glfwSetCursorPosCallback(m_Window, MouseCallback);

      // if old callback was not set
      if (oldCallback == nullptr)
      {
        // set the first time for the mouse
        m_FirstMouse = true;
      }
    }

    // check if application left mouse click was released
    if (glfwGetMouseButton(m_Window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_RELEASE)
    {
      // give the cursor back to the user
      glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

      // the mouse should be false for click and drag movement
      m_FirstMouse = false;

      // set cursor callback to false for click and drag movement
      glfwSetCursorPosCallback(m_Window, nullptr);
    }
  }
  
  // check if the user what to move faster with shift, slower with control
  if(glfwGetKey(m_Window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
  {
    camera.m_Speed = camera.m_BaseSpeed * 2.0f;
  }
  else if (glfwGetKey(m_Window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
  {
    camera.m_Speed = camera.m_BaseSpeed / 2.0f;
  }
  else
  {
    camera.m_Speed = camera.m_BaseSpeed;
  }

  // check if user wants to move around with the camera
  if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
    camera.Update(FORWARD, deltaTime);
  if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
    camera.Update(BACKWARD, deltaTime);
  if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
    camera.Update(LEFT, deltaTime);
  if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
    camera.Update(RIGHT, deltaTime);
}

