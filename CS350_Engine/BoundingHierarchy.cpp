#include "BoundingHierarchy.h"

#include <algorithm>

#include "Debug.h"
#include "AxisAlingedBoundingBox.h"
#include "Engine.h"

std::vector<std::string> BoundingHierarchy::MethodNames
{
    "AABB",
    "PCA",
};

std::vector<std::string> BoundingHierarchy::ConstructionNames
{
    "TopDown",
    "BottomUp",
};


void BoundingHierarchy::Draw(const std::vector<Object>& objects)
{
  switch (m_Construction)
  {
  case TopDown:
  {
    TopDownConstruction(objects);
    break;
  }
  case BottomUp:
  {
    BottomUpConstruction(objects);
    break;
  }
  }
}

void BoundingHierarchy::TopDownConstruction(const std::vector<Object>& objects)
{
}

void BoundingHierarchy::BottomUpConstruction(const std::vector<Object>& objects)
{
  std::vector<std::vector<::AxisAlingedBoundingBox>> map;

  for(unsigned i = 0; i < objects.size(); ++i)
  {
    map.emplace_back();
  }

  for (unsigned i = 0; i < objects.size(); ++i)
  {
    map[0].emplace_back(objects[i].m_AABB);
  }

  for(unsigned i = 1; i < map.size(); ++i)
  {
    map[i] = map[i - 1];
    const size_t NumberOfObjects = map[i].size();

    float shortestDistance(INFINITY);
    int index0 = -1;
    int index1 = -1;

    for(size_t j = 0; j < NumberOfObjects; ++j)
    {
      for(size_t k = 0; k < NumberOfObjects; ++k)
      {
        if(j == k)
          continue;

        const glm::vec3 object0 = map[i][j].Center();
        const glm::vec3 object1 = map[i][k].Center();

        const float distance = glm::distance(object0, object1);

        if(distance < shortestDistance)
        {
          shortestDistance = distance;

          index0 = (int)j;
          index1 = (int)k;
        }
      }
    }

    if(index0 != -1 && index1 != -1)
    {
      ::AxisAlingedBoundingBox newAABB;
      const ::AxisAlingedBoundingBox aabb0 = map[i][index0];
      const ::AxisAlingedBoundingBox aabb1 = map[i][index1];
      newAABB.m_Max.x = std::max(aabb0.m_Max.x, aabb1.m_Max.x);
      newAABB.m_Max.y = std::max(aabb0.m_Max.y, aabb1.m_Max.y);
      newAABB.m_Max.z = std::max(aabb0.m_Max.z, aabb1.m_Max.z);

      newAABB.m_Min.x = std::min(aabb0.m_Min.x, aabb1.m_Min.x);
      newAABB.m_Min.y = std::min(aabb0.m_Min.y, aabb1.m_Min.y);
      newAABB.m_Min.z = std::min(aabb0.m_Min.z, aabb1.m_Min.z);

      map[i].erase(map[i].begin() + std::max(index0, index1));
      map[i][std::min(index0, index1)] = newAABB;
    }
  }


  for(unsigned i = 0; i < map.size(); ++i)
  {
    float fraction =  (float)i/ (map.size() - 1);
    for(unsigned j = 0; j < map[i].size(); ++j)
    {
      map[i][j].Draw(glm::vec3{ fraction , 0 , 0 });
    }
  }
  ;
}
