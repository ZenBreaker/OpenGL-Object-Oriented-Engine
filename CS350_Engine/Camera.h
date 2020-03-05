/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: Camera.h
Purpose: handels camera functionality
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
#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

//! Camera Directions
enum Camera_Movement {
  FORWARD,
  BACKWARD,
  LEFT,
  RIGHT
};

/**
 * @brief 
 *   Camera class 
 */
class Camera
{
public:
  // Constructor a new Camera object
  Camera();
  // Destructure the Camera object
  ~Camera();

  glm::vec3 m_Position; //!< camera's position vector
  glm::vec3 m_Foward;   //!< camera's forward vector
  glm::vec3 m_Up;       //!< camera's up vector
  glm::vec3 m_Right;    //!< camera's right vector
  float m_Speed;        //!< camera's current speed
  float m_BaseSpeed;    //!< camera's base speed
  bool m_TimeDependent; //!< camera's speed time dependent
  float m_Yaw;          //!< camera's Euler yaw
  float m_Pitch;        //!< camera's Euler pitch
  float m_Sensitivity;  //!< camera's mouse sensitivity
  float m_LastX;        //!< 
  float m_LastY;        //!< 

  // Update the position of the Camera
  void Update(Camera_Movement direction, float deltaTime);

  // Taking mouse movement and update camera vectors
  void MouseMovement(float xoffset, float yoffset);

  // Using Euler angles, calculate camera vectors
  void UpdateCameraVectors();

  // calculate the view matrix
  glm::mat4 GetViewMatrix() const;
};

#endif