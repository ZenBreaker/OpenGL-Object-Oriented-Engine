#include "Debug.h"
#include <glew-2.1.0/GL/glew.h>

#include <glfw-3.3.bin.WIN64/GLFW/glfw3.h>
#include "includes/glm/glm.hpp"

#include "Engine.h"

float BoxVertices[] = {
  // positions        
  -0.5f,  0.5f, 0.5f,    //bottom right forward
  0.5f, 0.5f, 0.5f,      //top right forward
  -0.5f,  -0.5f, 0.5f,   //bottom left forward
  0.5f, -0.5f, 0.5f,     //top left forward
  -0.5f, 0.5f, -0.5f,    //bottom right back
  0.5f, 0.5f, -0.5f,     //top right back
  -0.5f, -0.5f, -0.5f,   //bottom left back
  0.5f, -0.5f, -0.5f,    //top left back
};

Debug::Debug()
{
}

Debug::~Debug()
{
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_EBO);
  glDeleteVertexArrays(1, &m_VAO);
}

void Debug::Init()
{
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  m_ViewUniform = glGetUniformLocation(Engine::get().m_AssetManager.GetShader(ShaderIndex::DefaultShader)->m_ProgramID, "ViewMatrix");
  m_PerspectiveUniform = glGetUniformLocation(Engine::get().m_AssetManager.GetShader(ShaderIndex::DefaultShader)->m_ProgramID, "PerspectiveMatrix");
  m_ModelUniform = glGetUniformLocation(Engine::get().m_AssetManager.GetShader(ShaderIndex::DefaultShader)->m_ProgramID, "ModelMatrix");
}

void Debug::Update()
{
  glUseProgram(Engine::get().m_AssetManager.GetShader(ShaderIndex::DefaultShader)->m_ProgramID);

  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Debug::Point), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Debug::Point), (void*)offsetof(Debug::Point, r));

  glUniformMatrix4fv(m_ViewUniform, 1, GL_FALSE, &Engine::get().m_RenderingManager.m_View[0][0]);
  glUniformMatrix4fv(m_PerspectiveUniform, 1, GL_FALSE, &Engine::get().m_RenderingManager.m_Projection[0][0]);
  glm::mat4 identity(1);
  

  glUniformMatrix4fv(m_ModelUniform, 1, GL_FALSE, &identity[0][0]);

  for (int i = 0; i < queue.size(); ++i)
  {
    glBufferData(GL_ARRAY_BUFFER, queue[i].points.size() * sizeof(Debug::Point), queue[i].points.data(), GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, queue[i].elementIndex.size() * sizeof(unsigned int),
      queue[i].elementIndex.data(), GL_STREAM_DRAW);

    glDrawElements(GL_LINES, (GLsizei)queue[i].elementIndex.size(), GL_UNSIGNED_INT, NULL);
  }

  glBindVertexArray(0);
  queue.clear();
}

void Debug::drawPoints(std::vector<Point> points, bool depthEnabled)
{
}

void Debug::drawWorldLines(std::vector<Line> lines, bool depthEnabled)
{
}

void Debug::drawScreenRects(std::vector<Rect2D> rects, bool depthEnable)
{
}

void Debug::drawWorldRects(const Rect3D &rect, float r, float g, float b, bool depthEnable)
{
  DrawData data;

  data.points.reserve(8);
  for (int i = 0; i < 24; i+=3)
  {
    data.points.emplace_back(BoxVertices[i] * rect.width + rect.topLeft.x, BoxVertices[i+1] * rect.height + rect.topLeft.y, BoxVertices[i+2] * rect.depth + rect.topLeft.z);
  }

  data.elementIndex.reserve(24);

  data.elementIndex.emplace_back(0);
  data.elementIndex.emplace_back(1);

  data.elementIndex.emplace_back(2);
  data.elementIndex.emplace_back(3);

  data.elementIndex.emplace_back(1);
  data.elementIndex.emplace_back(3);

  data.elementIndex.emplace_back(0);
  data.elementIndex.emplace_back(2);

  data.elementIndex.emplace_back(4);
  data.elementIndex.emplace_back(5);

  data.elementIndex.emplace_back(6);
  data.elementIndex.emplace_back(7);

  data.elementIndex.emplace_back(5);
  data.elementIndex.emplace_back(7);

  data.elementIndex.emplace_back(4);
  data.elementIndex.emplace_back(6);

  data.elementIndex.emplace_back(0);
  data.elementIndex.emplace_back(4);

  data.elementIndex.emplace_back(1);
  data.elementIndex.emplace_back(5);

  data.elementIndex.emplace_back(2);
  data.elementIndex.emplace_back(6);

  data.elementIndex.emplace_back(3);
  data.elementIndex.emplace_back(7);

  data.depthEnable = depthEnable;
  data.fill = false;
  data.r = r;
  data.g = g;
  data.b = b;
  queue.emplace_back(data);
}
