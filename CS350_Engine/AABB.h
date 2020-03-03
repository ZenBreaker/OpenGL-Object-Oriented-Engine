#ifndef AABB_H
#define AABB_H

#include <glm/glm.hpp>
#include <vector>

class AABB
{
public:
  AABB();
  ~AABB();
  void Draw();
  void Clear();
  glm::vec3 Center();
  glm::vec3 Size();
  void Update(const std::vector<glm::vec3> & points);
private:
  glm::vec3 m_Max;
  glm::vec3 m_Min;
};

#endif