/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: OctTree.h
Purpose: Creates and shows OctTree
Language: C++ and Visual Studio 2017
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
Project: michael.ngo_CS350_1
Author: Michael Ngo, michael.ngo, 90003217
Creation date: 2/2/2020
End Header --------------------------------------------------------*/
#ifndef OCTTREE_H
#define OCTTREE_H

#include "Object.h"
#include "Debug.h"
#include <vector>
 
enum axis
{
  X = 0,
  Y = 1,
  Z = 2
};

struct OctTreeNode
{
public:
  OctTreeNode()
  {
    childrenList.resize(8);
  }
  static const int maxDepth = 4;
  glm::vec3 center = glm::vec3{0,0,0};
  float halfWidth = -1;
  OctTreeNode* parent;
  std::vector<OctTreeNode*> childrenList;
  std::vector<Object*> objectsList;
  bool showBoxes = false;
  int level = -1;
  void Draw();
};

inline OctTreeNode* BuildOctree(glm::vec3 center, float halfWidth, int level)
{
  if (level < 0)
  {
    return NULL;
  }
  else 
  {
    // Construct and fill in ’root’ of this subtree
    OctTreeNode* node = new OctTreeNode();
    node->center = center;
    node->halfWidth = halfWidth;

    // Recursively construct the eight children of the subtree
    glm::vec3 offset;

    float step = halfWidth * 0.5f;

    for (int i = 0; i < 8; i++) {
      offset.x = ((i & 1) ? step : -step);
      offset.y = ((i & 2) ? step : -step);
      offset.z = ((i & 4) ? step : -step);
      node->childrenList[i] = BuildOctree(center + offset, step, level - 1);
      if (node->childrenList[i])
      {
        node->childrenList[i]->parent = node;
      }
    }
    node->level = level;
    return node;
  }
}

inline void OctTreeInsertObject(OctTreeNode* pTree, Object* pObject)
{
  int index = 0, straddle = 0;
  // Compute the octant number [0..7] the object sphere center is in
  // If straddling any of the dividing x, y, or z planes, exit directly
  for (int i = 0; i < 3; i++) {
    float delta = pObject->Centroid()[i] - pTree->center[i];

    if (glm::abs(delta) < pTree->halfWidth + 0.1f) {
      straddle = 1;
      break;
    }
    if (delta > 0.0f) index |= (1 << i); // ZYX
  }
  
  if (!straddle) 
  {
    if (pTree->childrenList[index] == NULL) 
    {
      pTree->childrenList[index] = new OctTreeNode;
    }
    OctTreeInsertObject(pTree->childrenList[index], pObject);
  }
  else 
  {
    // Straddling, or no child node to descend into
    pTree->objectsList.push_back(pObject);
  }
}

#endif