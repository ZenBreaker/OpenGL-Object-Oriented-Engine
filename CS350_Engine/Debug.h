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
   *   point struct
   */
  struct Point
  {
    /**
     * @brief 
     *   Constructor a new Point object with 3 floats
     * 
     * @param x 
     *   x position value
     * 
     * @param y 
     *   y position value
     * 
     * @param z 
     *   z position value
     */
    Point(float x, float y, float z) : x(x), y(y), z(z)
    {
    }

    /**
     * @brief 
     *   Constructor a new Point object with a glm::vec3
     * 
     * @param point 
     *   vec3 position vector
     */
    Point(const glm::vec3 & point) : x(point.x), y(point.y), z(point.z)
    {
    }

    /**
     * @brief 
     *   default constructor a new Point object  
     */
    Point() : x(0), y(0), z(0)
    {
    }

    float x; //!< x position value
    float y; //!< y position value
    float z; //!< z position value

    float r; //!< r color value
    float g; //!< g color value
    float b; //!< b color value
  };

  /**
   * @brief 
   *   line struct
   */
  struct Line
  {
    Point point0, point1; // the two end points
    float r, g, b;        // the color of the line
  };

  /**
   * @brief 
   *   rect 2d struct
   */
  struct Rect2D
  {
    Point topLeft; //!< top left position of the box
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

    Point Center; //!< center of the rect
    float width;  //!< width of the rect
    float height; //!< height of the rect
    float depth;  //!< depth of the rect
  };

  /**
   * @brief 
   *   draw data struct
   */
  struct DrawData
  {
    std::vector<Point> points;              //!< vertices
    std::vector<unsigned int> elementIndex; //!< vertex indices
    bool depthEnable;                       //!< depth enable
    bool fill;                              //!< fill 
    float r, g, b;                          //!< color value
  };

  // draw debug points
  void drawPoints(std::vector<Point> points, bool depthEnabled);

  // draw debug world lines
  void drawWorldLines(std::vector<Line> lines, bool depthEnabled);

  // draw debug screen rects
  void drawScreenRects(std::vector<Rect2D> rects, bool depthEnable);

  // draw debug a rect in world space
  void drawWorldRects(const Rect3D &rect, float r, float g, float b, bool depthEnable);

  // debug draw a sphere in world space
  void drawWorldSphere(glm::vec3 center, float radius, bool depthEnable);

private:
  std::vector<DrawData> queue;                                //!< queue of debug to draw
  GLuint m_VAO;                                               //!< vao
  GLuint m_VBO;                                               //!< vbo
  GLuint m_EBO;                                               //!< ebo
  GLuint m_ViewUniform, m_PerspectiveUniform, m_ModelUniform; //!< shader uniforms
};

#endif
