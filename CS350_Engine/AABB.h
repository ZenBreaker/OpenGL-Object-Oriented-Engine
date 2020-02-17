#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>

class AABB
{
public:
  AABB();
  ~AABB();
  void Draw();
  void Clear();
  glm::vec3 Center();
  glm::vec3 Size();
  void AddPoint(const glm::vec3 & point);
private:
  glm::vec3 m_Max;
  glm::vec3 m_Min;
};

#endif