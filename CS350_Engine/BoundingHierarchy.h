#ifndef BOUNDINGHIERARCHY_H
#define BOUNDINGHIERARCHY_H

#include "Object.h"

class BoundingHierarchy
{
public:
  enum Method
  {
    AABB,
    PCA,
  };

  enum Construction
  {
    TopDown,
    BottomUp,
  };

  static std::vector<std::string> MethodNames;
  static std::vector<std::string> ConstructionNames;

  void Draw(const std::vector<Object> &objects);

  void TopDownConstruction(const std::vector<Object>& objects);
  void BottomUpConstruction(const std::vector<Object>& objects);

  Method m_Method;
  Construction m_Construction;
};


#endif
