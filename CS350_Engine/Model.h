/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.h
Purpose: constructs models
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
#ifndef MODEL_H
#define MODEL_H

#include <glew-2.1.0/GL/glew.h>
#include <glm/glm.hpp>

#include <memory>
#include <vector>

#include "AABB.h"

enum ModelIndex
{
  FourSphere,
  Bunny,
  BunnyHighPoly,
  Cube,
  CubeTwo,
  Cup,
  LucyPrinceton,
  Quad,
  Rhino,
  Sphere,
  SphereModified,
  StarWarsOne,
  Triangle,
};

static const char* const ModelNames[] =
{
  "4Sphere",
  "bunny",
  "bunny_high_poly",
  "cube",
  "cube2",
  "cup",
  "lucy_princeton",
  "quad",
  "rhino",
  "sphere",
  "sphere_modified",
  "starwars1",
  "triangle",
};

class Model
{
public:
  Model(ModelIndex index = ModelIndex::Bunny);
  Model(const Model& rhs) = delete;
  Model(Model&& rhs) = delete;
  ~Model();
  Model& operator=(Model&& rhs) noexcept;

  ModelIndex m_ModelIndex;
  std::vector<glm::vec3> m_Vertices;
  std::vector<glm::vec3> m_VertexNormals;
  std::vector<glm::uint> m_Indices;
  std::vector<glm::vec3> m_FaceNormals;
  std::vector<glm::vec3> m_FacesPosition;
  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_VNBO;
  GLuint m_EBO;
  GLuint m_DrawMode;


  AABB& Bounds(const glm::mat4 & modelToWorld);

private:
  AABB m_Bounds;

  void ParseModel(const char * filename);
  void GenerateBuffers();
  void CleanUp();
  void SetZero();
  void CalculateBounds(const glm::mat4 & modelToWorld);
};

using ModelPtr = std::shared_ptr<Model>;

#endif
