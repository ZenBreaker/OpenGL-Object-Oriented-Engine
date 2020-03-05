#include "Debug.h"
#include <glew-2.1.0/GL/glew.h>

#include <glfw-3.3.bin.WIN64/GLFW/glfw3.h>
#include "includes/glm/glm.hpp"

#include "Engine.h"

//! const NDC box vertices
const float BoxVertices[] = { 
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

/**
 * @brief 
 *   Constructor a new Debug Manager
 */
Debug::Debug()
{
}

/**
 * @brief 
 *   Destructor for the Debug Manager
 */
Debug::~Debug()
{
  glDeleteBuffers(1, &m_VBO);
  glDeleteBuffers(1, &m_EBO);
  glDeleteVertexArrays(1, &m_VAO);
}

/**
 * @brief 
 *   Initialize any debug information that should be created on start up
 */
void Debug::Init()
{
  // generate vao and buffers
  glGenVertexArrays(1, &m_VAO);
  glGenBuffers(1, &m_VBO);
  glGenBuffers(1, &m_EBO);

  // get uniforms from debug shader
  m_ViewUniform = glGetUniformLocation(Engine::get().m_AssetManager.GetShader(ShaderIndex::DefaultShader)->m_ProgramID, "ViewMatrix");
  m_PerspectiveUniform = glGetUniformLocation(Engine::get().m_AssetManager.GetShader(ShaderIndex::DefaultShader)->m_ProgramID, "PerspectiveMatrix");
  m_ModelUniform = glGetUniformLocation(Engine::get().m_AssetManager.GetShader(ShaderIndex::DefaultShader)->m_ProgramID, "ModelMatrix");
}

/**
 * @brief 
 *   draw all queued debug calls
 */
void Debug::Update()
{
  // use debug shader
  glUseProgram(Engine::get().m_AssetManager.GetShader(ShaderIndex::DefaultShader)->m_ProgramID);

  // attach vao and buffers
  glBindVertexArray(m_VAO);
  glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

  // enable attribute location 0 and 1
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Debug::Point), (void*)0);
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Debug::Point), (void*)offsetof(Debug::Point, r));

  // Set matrix uniforms for the shader 
  glUniformMatrix4fv(m_ViewUniform, 1, GL_FALSE, &Engine::get().m_RenderingManager.m_View[0][0]);
  glUniformMatrix4fv(m_PerspectiveUniform, 1, GL_FALSE, &Engine::get().m_RenderingManager.m_Projection[0][0]);
  glm::mat4 identity(1);
  glUniformMatrix4fv(m_ModelUniform, 1, GL_FALSE, &identity[0][0]);

  // loop through every queued debug draw request
  for (int i = 0; i < queue.size(); ++i)
  {
    // set buffer data
    glBufferData(GL_ARRAY_BUFFER, queue[i].points.size() * sizeof(Debug::Point), queue[i].points.data(), GL_STREAM_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, queue[i].elementIndex.size() * sizeof(unsigned int),
      queue[i].elementIndex.data(), GL_STREAM_DRAW);

    // debug draw 
    glDrawElements(GL_LINES, (GLsizei)queue[i].elementIndex.size(), GL_UNSIGNED_INT, NULL);
  }

  // unbind vao
  glBindVertexArray(0);

  // clear queue
  queue.clear();
}

/**
 * @brief 
 *   draw debug points 
 * 
 * @param points 
 *   points to draw
 * 
 * @param depthEnabled 
 *   depth dependent, (world or screen space)
 */
void Debug::drawPoints(std::vector<Point> points, bool depthEnabled)
{
}

/**
 * @brief 
 *   draw debug world lines 
 * 
 * @param lines 
 *   lines to draw
 * 
 * @param depthEnabled 
 *   depth dependent
 */
void Debug::drawWorldLines(std::vector<Line> lines, bool depthEnabled)
{
}

/**
 * @brief 
 *   draw debug screen rects
 * 
 * @param rects 
 *   rects to draw
 * 
 * @param depthEnabled 
 *   depth dependent
 */
void Debug::drawScreenRects(std::vector<Rect2D> rects, bool depthEnable)
{
}

/**
 * @brief 
 *   draw debug a rect in world space
 * 
 * @param rect 
 *   rect to draw
 * 
 * @param r 
 *   red color
 * 
 * @param g 
 *   green color
 * 
 * @param b 
 *   blue color
 * 
 * @param depthEnable 
 *   depth dependent
 */
void Debug::drawWorldRects(const Rect3D &rect, float r, float g, float b, bool depthEnable)
{
  // temp data object
  DrawData data;

  // reserve space for size of box points
  data.points.reserve(8);

  // loop though 8 verties and scale and position the vertex in the world
  for (int i = 0; i < 24; i+=3)
  {
    // NDC * scale + translate
    data.points.emplace_back(BoxVertices[i] * rect.width + rect.Center.x, BoxVertices[i+1] * rect.height + rect.Center.y, BoxVertices[i+2] * rect.depth + rect.Center.z);
  }

  // reserve space for size of box indices
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

  // set debug info
  data.depthEnable = depthEnable;
  data.fill = false;
  data.r = r;
  data.g = g;
  data.b = b;

  // queue box to draw later
  queue.emplace_back(data);
}

/**
 * @brief 
 *   debug draw a sphere in world space
 * 
 * @param center 
 *   center of the sphere
 * 
 * @param radius 
 *   radius of the sphere
 * 
 * @param depthEnable 
 *   depth dependent
 */
void Debug::drawWorldSphere(glm::vec3 center, float radius, bool depthEnable)
{
  // grab sphere model
  auto sphere = Engine::get().m_AssetManager.GetModel(ModelIndex::Sphere);

  // temp data 
  DrawData data;

  // reserves size of sphere vertices
  data.points.reserve(sphere->m_Vertices.size());

  // loop through each vertex
  for(unsigned i = 0; i < sphere->m_Vertices.size(); ++i)
  {
    // add vertex to points, NDC * scale + translate
    data.points.emplace_back(sphere->m_Vertices[i] * radius + center);
  }

  // set indices
  data.elementIndex = sphere->m_Indices;
  
  // set debug info
  data.depthEnable = depthEnable;
  data.fill = false;

  // queue sphere to draw later
  queue.emplace_back(data);
}
