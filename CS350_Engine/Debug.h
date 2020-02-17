#ifndef DEBUG_H
#define DEBUG_H

#include <glew-2.1.0/GL/glew.h>

#include <glfw-3.3.bin.WIN64/GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <vector>

class Debug
{
public:
  Debug();
  ~Debug();
  void Init();
  void Update();

  struct Point
  {
    Point(float x, float y, float z) : x(x), y(y), z(z)
    {

    }
    Point(const glm::vec3 & point) : x(point.x), y(point.y), z(point.z)
    {

    }
    Point() : x(0), y(0), z(0)
    {

    }
    float x, y, z;
    float r, g, b;
  };

  struct Line
  {
    Point point0, point1;
    float r, g, b;
  };

  struct Rect2D
  {
    Point topLeft;
    float width;
    float height;
  };

  struct Rect3D
  {
    Rect3D() : width(1), height(1), depth(1), topLeft() {}
    ~Rect3D(){}
    Point topLeft;
    float width;
    float height;
    float depth;
  };

  struct DrawData
  {
    std::vector<Point> points;
    std::vector<unsigned int> elementIndex;
    bool depthEnable;
    bool fill;
    float r, g, b;
  };

  void drawPoints(std::vector<Point> points, bool depthEnabled);
  void drawWorldLines(std::vector<Line> lines, bool depthEnabled);
  void drawScreenRects(std::vector<Rect2D> rects, bool depthEnable);
  void drawWorldRects(const Rect3D &rect, float r, float g, float b, bool depthEnable);
  
private:
  std::vector<DrawData> queue;
  GLuint m_VAO;
  GLuint m_VBO;
  GLuint m_EBO;
  GLuint m_ViewUniform, m_PerspectiveUniform, m_ModelUniform;
};

#endif
