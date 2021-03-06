/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
File Name: Model.h
Purpose: Loads and Parses ".obj" files for models
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
Author: Michael Ngo, michael.ngo
Creation date: 2/2/2020
End Header --------------------------------------------------------*/

#include <glm/glm.hpp>

#include <fstream>
#include <algorithm>
#include <string>
#include <map>

#include "Model.h"


std::vector<std::string> Model::IndexNames
{
  "4Sphere",
    "bunny",
    "bunny_high_poly",
    "cube",
    "cube2",
    "cup",
    "lucy_princeton",
    "quad",
    "sphere",
    "sphere_modified",
};


/**
 * @brief
 *   Default constructor for a new Model object
 *
 * @param index
 *   Index of model to create
 */
Model::Model(Index index)
{
  m_ModelIndex = index;
  std::string filename("models\\");
  filename.append(IndexNames[index]);
  filename.append(".obj");
  ParseModel(filename.c_str());
}

/**
 * @brief
 *   Destructure for the Model object
 */
Model::~Model()
{
  CleanUp();
}

/**
 * @brief
 *   Move constructor for a Model object
 *
 * @param rhs
 *   Model to copy over
 *
 * @return Model&
 *   Model reference
 */
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

/**
 * @brief
 *   Cleanup the model, destructing
 */
void Model::CleanUp()
{
  // delete buffers
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_EBO);
  glDeleteVertexArrays(1, &m_VAO);

  // set member variables to zero
  SetZero();
}

/**
 * @brief
 *   Set member variables to zero
 */
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

/**
 * @brief
 *   Model Parsing for ".obj"
 *
 * @param filename
 *   file name to load
 */
void Model::ParseModel(const char* filename)
{
  std::ifstream infile(filename); // open the file
  std::string line;               // string to hold each line of the file

  glm::vec3 max(-INFINITY); // max point of the model
  glm::vec3 min(INFINITY); // min point of the model
  std::vector<std::pair<int, int>> VertexNormal;    // Vertex normal map to count references

  if (infile.is_open()) // if the file did not open
  {
    while (std::getline(infile, line, '\n')) // if not at the end of file
    {
      switch (line[0]) // check what the first letter is
      {
      case 'v': // grab the vertex
      {
        switch (line[1]) // check for the second letter is
        {
          // if its just a vertex
        case ' ':
        case '\t':
        {
          // temp x, y and z for sscanf to fill
          float x, y, z;

          // sscanf for both types of line, these are considered the same
          sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
          sscanf(line.c_str(), "v\t%f %f %f", &x, &y, &z);

          // push back the vertex 
          m_Vertices.emplace_back(x, y, z);

          // check if point is a max point
          if (max.x < m_Vertices.back().x)
          {
            max.x = m_Vertices.back().x;
          }
          if (max.y < m_Vertices.back().y)
          {
            max.y = m_Vertices.back().y;
          }
          if (max.z < m_Vertices.back().z)
          {
            max.z = m_Vertices.back().z;
          }

          // check if point is a min point
          if (min.x > m_Vertices.back().x)
          {
            min.x = m_Vertices.back().x;
          }
          if (min.y > m_Vertices.back().y)
          {
            min.y = m_Vertices.back().y;
          }
          if (min.z > m_Vertices.back().z)
          {
            min.z = m_Vertices.back().z;
          }
          break;
        }
        // if its a vertex normal
        case 'n':
        {
          // temp x, y and z for sscanf to fill
          float x, y, z;

          // sscanf the current line
          sscanf(line.c_str(), "vn %f %f %f", &x, &y, &z);

          // push back the vertex
          m_VertexNormals.emplace_back(x, y, z);
          break;
        }
        }
        break;
      }
      // if its a face
      case 'f':
      {
        // temp values for sscanf to fill
        int a1, a2, a3, a4;

        // scan the line for how many triangles
        int successnumber = sscanf(line.c_str(), "f %i %i %i %i", &a1, &a2, &a3, &a4);
        switch (successnumber)
        {
          // One Triangles
        case 3:
        {
          m_Indices.push_back(a1 - 1); // Triangle 1
          m_Indices.push_back(a2 - 1);
          m_Indices.push_back(a3 - 1);
          break;
        }
        // Two Triangles
        case 4:
        {
          m_Indices.push_back(a1 - 1); // Triangle 1
          m_Indices.push_back(a2 - 1);
          m_Indices.push_back(a3 - 1);
          m_Indices.push_back(a1 - 1); // Triangle 2
          m_Indices.push_back(a3 - 1);
          m_Indices.push_back(a4 - 1);
          break;
        }
        }
        break;
      }
      }
    }
  }

  // find the range of the model
  const float rangex = max.x - min.x;
  const float rangey = max.y - min.y;
  const float rangez = max.z - min.z;

  const float maxRange = std::max(std::max(rangex, rangey), rangez);

  // find the center
  const glm::vec3 totalSize{ maxRange };
  const glm::vec3 center{ (min + max) / 2.0f };

  // loop all the vertices, covert to ndc and center the vertices around the model equally
  for (glm::vec3& vertex : m_Vertices)
  {
    vertex -= center;
    vertex /= totalSize;
  }

  // manually calculate the vertex normals
  m_VertexNormals.resize(m_Vertices.size());

  // map of how many times a vertex been reference
  std::map<int, int> count;

  // loop through all the indices 
  for (size_t i = 0; i < m_Indices.size() - 2; i += 3)
  {
    // get 3 points for a triangle
    glm::vec3 v0 = m_Vertices[m_Indices[i + 0]];
    glm::vec3 v1 = m_Vertices[m_Indices[i + 1]];
    glm::vec3 v2 = m_Vertices[m_Indices[i + 2]];

    // create the face
    glm::vec3 A = v1 - v0;
    glm::vec3 B = v2 - v0;

    // calculate normal
    m_FaceNormals.emplace_back(glm::normalize(glm::cross(A, B)));

    // push back face position
    m_FacesPosition.emplace_back((v0 + v1 + v2) / 3.0f);

    // increment the count in the map, and add the vertex normal with the face
    count[m_Indices[i + 0]] += 1;
    m_VertexNormals[m_Indices[i + 0]] += m_FaceNormals.back();

    count[m_Indices[i + 1]] += 1;
    m_VertexNormals[m_Indices[i + 1]] += m_FaceNormals.back();

    count[m_Indices[i + 2]] += 1;
    m_VertexNormals[m_Indices[i + 2]] += m_FaceNormals.back();
  }

  // loop through the indices
  for (size_t i = 0; i < m_Indices.size(); ++i)
  {
    // normalize the vertex normal 
    m_VertexNormals[m_Indices[i]] = glm::normalize(m_VertexNormals[m_Indices[i]] / (float)count[m_Indices[i]]);
  }

  // generate buffers for shaders
  GenerateBuffers();

  // draw mode for models from obj's are by default triangles
  m_DrawMode = GL_TRIANGLES;
}

/**
 * @brief
 *   Generate Buffers for the models for shader to use
 */
void Model::GenerateBuffers()
{
  // clear buffers
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_VNBO);
  glDeleteBuffers(1, &m_EBO);
  glDeleteVertexArrays(1, &m_VAO);

  // generate new buffers
  glGenVertexArrays(1, &m_VAO);
  glBindVertexArray(m_VAO);

  // bind vertices
  glGenBuffers(1, &m_VBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(glm::vec3),
    m_Vertices.data(), GL_STATIC_DRAW);

  // enable vertex positions
  glEnableVertexAttribArray(0);
  //glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // bind vertex normals
  glGenBuffers(1, &m_VNBO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VNBO);
  glBufferData(GL_ARRAY_BUFFER, m_VertexNormals.size() * sizeof(glm::vec3),
    m_VertexNormals.data(), GL_STATIC_DRAW);

  // enable vertex normals
  glEnableVertexAttribArray(1);
  //glBindBuffer(GL_ARRAY_BUFFER, m_VNBO);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

  // bind vertex indices
  glGenBuffers(1, &m_EBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(glm::uint),
    m_Indices.data(), GL_STATIC_DRAW);

  glBindVertexArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}
