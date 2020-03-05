/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: Camera.cpp
Purpose: handels camera functionality using Euler Angles
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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "Camera.h"

/**
 * @brief 
 *   Constructor a new Camera object
 */
Camera::Camera()
{
  m_Foward = glm::vec3(0.0f, 0.0f, -1.0f);
  m_Yaw = -90.0f;
  m_Pitch = 0.0f;
  m_BaseSpeed = 1.0f;
  m_Speed = m_BaseSpeed;
  m_Sensitivity = .1f;
  m_TimeDependent = true;
  UpdateCameraVectors();
}

/**
 * @brief 
 *   Destructure the Camera object
 */
Camera::~Camera()
{
}

/**
 * @brief 
 *   Update the position of the Camera
 * 
 * @param direction 
 *   What direction is the camera moving in
 * 
 * @param deltaTime 
 *   Scale the amount the camera by delta time 
 */
void Camera::Update(Camera_Movement direction, float deltaTime)
{
  // set base speed
  float velocity = m_Speed;

  // check if camera is time dependent
  if(m_TimeDependent)
  {
    velocity *= deltaTime; // scale by delta time
  }

  // update camera based on moving direction
  if (direction == FORWARD)
    m_Position += m_Foward * velocity;
  if (direction == BACKWARD)
    m_Position -= m_Foward * velocity;
  if (direction == LEFT)
    m_Position -= m_Right * velocity;
  if (direction == RIGHT)
    m_Position += m_Right * velocity;
}

/**
 * @brief 
 *   Taking mouse movement and update camera vectors
 * 
 * @param xoffset 
 *   change in x direction
 * 
 * @param yoffset 
 *   change in y direction
 */
void Camera::MouseMovement(float xoffset, float yoffset)
{
  // scale how much mouse movement affects the camera
  xoffset *= m_Sensitivity;
  yoffset *= m_Sensitivity;

  // update euler angles
  m_Yaw += xoffset;
  m_Pitch += yoffset;

  // clamp Pitch to -89 to 89 degress
  if (m_Pitch > 89.0f)
    m_Pitch = 89.0f;
  if (m_Pitch < -89.0f)
    m_Pitch = -89.0f;

  // Update Front, Right and Up Vectors using the updated Euler angles
  UpdateCameraVectors();
}

/**
 * @brief 
 *   Using Euler angles, calculate camera vectors
 */
void Camera::UpdateCameraVectors()
{
  // calculate Euler angles
  glm::vec3 front;
  front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
  front.y = sin(glm::radians(m_Pitch));
  front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

  // set camera vectors
  m_Foward = glm::normalize(front);
  m_Right = glm::normalize(glm::cross(m_Foward, glm::vec3(0.0f,1.0f,0.0f)));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
  m_Up = glm::normalize(glm::cross(m_Right, m_Foward));
}

/**
 * @brief 
 *   calculate the view matrix
 * 
 * @return glm::mat4 
 *   View Maxtrix from the camera's position
 */
glm::mat4 Camera::GetViewMatrix() const 
{
  return glm::lookAt(m_Position, m_Position + m_Foward, m_Up);
}