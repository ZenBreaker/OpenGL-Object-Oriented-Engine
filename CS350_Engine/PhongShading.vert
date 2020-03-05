/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: PhongShading.vert
Purpose: forward phong shading
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

#version 430 core

layout(location = 0) in vec3 vPosition;
layout(location = 1) in vec3 vNormal;

uniform mat4 PerspectiveMatrix;
uniform mat4 ViewMatrix;
uniform mat4 NormalMatrix;
uniform mat4 ModelMatrix;

out vec4 world_normal;
out vec4 world_position;
out vec3 model_position;
out vec3 ModelNormal;

void main()
{
  vec4 vPos = vec4( vPosition, 1.0f );
  gl_Position = PerspectiveMatrix * ViewMatrix * ModelMatrix * vPos;

  world_position = ModelMatrix * vPos;
  model_position = vPos.xyz;
  world_normal = vec4(mat3(NormalMatrix) * vNormal.xyz, 1.0);
  ModelNormal = vNormal;
}
