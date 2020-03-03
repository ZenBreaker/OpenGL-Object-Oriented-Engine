#ifndef BOUNDINGSPHERE_H
#define BOUNDINGSPHERE_H

#include <glm/glm.hpp>
#include <vector>

enum BoundingType
{
  Centroid,
  Ritters,
  Larssons,
  PCA,
};

static const char* const BoundingTypeNames[] =
{
  "Centroid",
  "Ritters",
  "Larssons",
  "PCA",
};

class BoundingSphere
{
public:
  BoundingSphere();
  ~BoundingSphere();
  void Draw();
  void Clear();
  BoundingType SetBoundingType(BoundingType type);
  void Update(const std::vector<glm::vec3>& vertices);
  void CentroidMethod(const std::vector<glm::vec3>& vertices);
  void RittersMethod(const std::vector<glm::vec3>& vertices);
  void LarssonsMethod(const std::vector<glm::vec3>& vertices);
  void PCAMethod(const std::vector<glm::vec3>& vertices);
private:
  glm::vec3 m_Center;
  float m_Radius;
  BoundingType m_Type;
};

#endif