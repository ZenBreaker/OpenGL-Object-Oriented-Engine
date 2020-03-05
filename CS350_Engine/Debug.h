/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
File Name: Debug.h
Purpose: Debug Manager. Collects requests of debug calls and draws them at the end of a frame
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
Creation date: 2/28/2020
End Header --------------------------------------------------------*/

#ifndef DEBUG_H
#define DEBUG_H

#include <glew-2.1.0/GL/glew.h>

#include <glfw-3.3.bin.WIN64/GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

/**
 * @brief 
 *   Debug manager for queued debug draw
 */
class Debug
{
public:
  // Constructor a new Debug Manager
  Debug();

  // Destructor for the Debug Manager
  ~Debug();

  // Initialize any debug information that should be created on start up
  void Init();

  // draw all queued debug calls
  void Update();

  /**
   * @brief 
   *   line struct
   */
  struct Line
  {
    glm::vec3 point0, point1; // the two end points
    float r, g, b;        // the color of the line
  };

  /**
   * @brief 
   *   rect 2d struct
   */
  struct Rect2D
  {
    glm::vec3 topLeft; //!< top left position of the box
    float width;   //!< width of the boz
    float height;  //!< height of the box
  };

  /**
   * @brief 
   *  rect 3d struct
   */
  struct Rect3D
  {
    /**
     * @brief 
     *   default constructor a new Rect 3 D object
     */
    Rect3D() : width(1), height(1), depth(1), Center() {}

    /**
     * @brief 
     *   Destructer the Rect 3D object
     */
    ~Rect3D(){}

    glm::vec3 Center; //!< center of the rect
    float width;  //!< width of the rect
    float height; //!< height of the rect
    float depth;  //!< depth of the rect
  };

  struct EBOData
  {
    int id;
    GLuint size;
  };

  /**
   * @brief 
   *   draw data struct
   */
  struct DrawData
  {
    std::vector<glm::vec3> points;              //!< vertices
    EBOData EBO;
    bool depthEnable;                       //!< depth enable
    bool fill;                              //!< fill 
    float r, g, b;                          //!< color value
  };

  // draw debug points
  void drawPoints(std::vector<glm::vec3> points, bool depthEnabled);

  // draw debug world lines
  void drawWorldLines(std::vector<Line> lines, bool depthEnabled);

  // draw debug screen rects
  void drawScreenRects(std::vector<Rect2D> rects, bool depthEnable);

  // draw debug a rect in world space
  void drawWorldRects(const Rect3D &rect, float r, float g, float b, bool depthEnable);

  // debug draw a sphere in world space
  void drawWorldSphere(glm::vec3 center, float radius, bool depthEnable);

private:
  std::vector<DrawData> m_Queue; //!< queue of debug objects to draw
  GLuint m_VAO;                  //!< vao
  GLuint m_VBO;                  //!< vbo
  GLuint m_ViewUniform;          //!< View Uniform
  GLuint m_PerspectiveUniform;   //!< Perspective Uniform
  GLuint m_ModelUniform;         //!< Model Uniform


  GLuint m_EBOStatic3DRect; //!< ebo of a 3d rect
  GLuint m_EBOStaticSphere; //!< ebo of a sphere
};

#endif
