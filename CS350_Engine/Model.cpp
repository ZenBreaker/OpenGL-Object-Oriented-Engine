/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: Model.cpp
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
#include <glm/glm.hpp>

#include <fstream>
#include <algorithm>
#include <string>
#include <map>

#include "Model.h"

Model::Model(ModelIndex index)
{
  m_ModelIndex = index;
  std::string filename("models\\");
  filename.append(ModelNames[index]);
  filename.append(".obj");
  ParseModel(filename.c_str());
}

Model::~Model()
{
  CleanUp();
}

Model& Model::operator=(Model&& rhs) noexcept
{
  CleanUp();

  m_VBO = rhs.m_VBO;
  m_EBO = rhs.m_EBO;
  m_VAO = rhs.m_VAO;

  m_Vertices = rhs.m_Vertices; // 
  m_VertexNormals = rhs.m_VertexNormals;
  m_FaceNormals = rhs.m_FaceNormals;
  m_FacesPosition = rhs.m_FacesPosition;
  m_Indices = rhs.m_Indices;  // 

  rhs.SetZero();

  return *this;
}

void Model::CleanUp()
{
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_EBO);
  glDeleteVertexArrays(1, &m_VAO);

  SetZero();
}

void Model::SetZero()
{
  m_VBO = 0;
  m_EBO = 0;
  m_VAO = 0;
  m_Vertices.clear();
  m_VertexNormals.clear();
  m_FaceNormals.clear();
  m_FacesPosition.clear();
  m_Indices.clear();
}

void Model::DrawAABBBounds(const glm::mat4 & modelToWorld)
{
  static std::vector<glm::vec3> temp;
  temp.clear();
  temp.reserve(m_Vertices.size());

  for (unsigned i = 0; i < m_Vertices.size(); ++i)
  {
    temp.emplace_back(glm::vec3(modelToWorld * glm::vec4(m_Vertices[i], 1.0f)));
  }

  m_AABBBounds.Update(temp);
  m_AABBBounds.Draw();
}

void Model::DrawBoundingSphere(const glm::mat4& modelToWorld)
{
  static std::vector<glm::vec3> temp;
  temp.clear();
  temp.reserve(m_Vertices.size());

  for (unsigned i = 0; i < m_Vertices.size(); ++i)
  {
    temp.emplace_back(glm::vec3(modelToWorld * glm::vec4(m_Vertices[i], 1.0f)));
  }

  m_BoundingSphere.Update(temp);
  m_BoundingSphere.Draw();
}

void Model::ParseModel(const char* filename)
{
  std::ifstream infile(filename); // open the file
  std::string line;

  glm::vec3 offset(0);

  glm::vec3 max(std::numeric_limits<float>::min());
  glm::vec3 min(std::numeric_limits<float>::max());
  std::vector<std::pair<int, int>> VertexNormal;

  if (infile.is_open()) // if the file did not open
  {
    while (std::getline(infile, line, '\n')) // if not at the end of file
    {
      switch (line[0]) // check what the first letter is
      {
      case 'v': // grab the vertex
      {
        switch (line[1])
        {
        case ' ':
        case '\t':
        {
          float x, y, z;
          sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
          sscanf(line.c_str(), "v\t%f %f %f", &x, &y, &z);
          glm::vec3 v(x, y, z);
          m_Vertices.push_back(v);
          offset += v;
          if (max.x < v.x) { max.x = v.x; }
          if (max.y < v.y) { max.y = v.y; }
          if (max.z < v.z) { max.z = v.z; }

          if (min.x > v.x) { min.x = v.x; }
          if (min.y > v.y) { min.y = v.y; }
          if (min.z > v.z) { min.z = v.z; }
          break;
        }
        case 'n':
        {
          float x, y, z;
          sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);
          m_VertexNormals.push_back(glm::vec3(x, y, z));
          break;
        }
        }
        break;
      }
      case 'f': // grab the face
      {
        int a1, a2, a3, a4;
        int successnumber = sscanf(line.c_str(), "f %i %i %i %i", &a1, &a2, &a3, &a4);
        switch (successnumber)
        {
        case 3:
        {
          m_Indices.push_back(a1 - 1); //tri1
          m_Indices.push_back(a2 - 1);
          m_Indices.push_back(a3 - 1);
          break;
        }
        case 4:
        {
          m_Indices.push_back(a1 - 1); //tri1
          m_Indices.push_back(a2 - 1);
          m_Indices.push_back(a3 - 1);
          m_Indices.push_back(a1 - 1); //tri2
          m_Indices.push_back(a3 - 1);
          m_Indices.push_back(a4 - 1);
          break;
        }
        default:
        {
          break;
        }
        }
        break;
      }
      default:
      {
        break;
      }
      }
    }
  }

  offset /= m_Vertices.size();
  //max -= offset;
  //min -= offset;
  float rangex = max.x - min.x;
  float rangey = max.y - min.y;
  float rangez = max.z - min.z;

  float rangemax(std::max(std::max(rangex, rangey), rangez));

  for (glm::vec3& vertex : m_Vertices)
  {
    vertex -= offset;
    vertex /= rangemax;
  }

  m_VertexNormals.resize(m_Vertices.size());
  std::map<int, int> count;
  for (size_t i = 0; i < m_Indices.size() - 2; i += 3)
  {
    glm::vec3 v0 = m_Vertices[m_Indices[i + 0]];
    glm::vec3 v1 = m_Vertices[m_Indices[i + 1]];
    glm::vec3 v2 = m_Vertices[m_Indices[i + 2]];
    glm::vec3 A = v1 - v0;
    glm::vec3 B = v2 - v0;
    m_FaceNormals.emplace_back(glm::normalize(glm::cross(A, B)));
    m_FacesPosition.emplace_back((v0 + v1 + v2) / 3.0f);

    count[m_Indices[i + 0]] += 1;
    m_VertexNormals[m_Indices[i + 0]] += m_FaceNormals.back();

    count[m_Indices[i + 1]] += 1;
    m_VertexNormals[m_Indices[i + 1]] += m_FaceNormals.back();

    count[m_Indices[i + 2]] += 1;
    m_VertexNormals[m_Indices[i + 2]] += m_FaceNormals.back();
  }

  for (size_t i = 0; i < m_Indices.size(); ++i)
  {
    m_VertexNormals[m_Indices[i]] = glm::normalize(m_VertexNormals[m_Indices[i]] / (float)count[m_Indices[i]]);
  }

  GenerateBuffers();

  m_DrawMode = GL_TRIANGLES;
}

void Model::GenerateBuffers()
{
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_VNBO);
  glDeleteBuffers(1, &m_EBO);
  glDeleteVertexArrays(1, &m_VAO);

  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(glm::vec3),
    m_Vertices.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &m_VNBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VNBO);
  glBufferData(GL_ARRAY_BUFFER, m_VertexNormals.size() * sizeof(glm::vec3),
    m_VertexNormals.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(glm::uint),
    m_Indices.data(), GL_STATIC_DRAW);
}
