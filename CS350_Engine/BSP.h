/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: BSP.h
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
#ifndef BSP_H
#define BSP_H
#include <glm/glm.hpp>

#include "Collision.h"
#include <vector>
#include "Model.h"

#define MAX_DEPTH 3
#define MIN_LEAF_SIZE 1 
#define MAX_POINTS 500
#define PLANE_THICKNESS_EPSILON 0.0001f
#define POINT_IN_FRONT_OF_PLANE 1
#define POINT_BEHIND_PLANE 2
#define POINT_ON_PLANE 3
#define POLYGON_STRADDLING_PLANE 4
#define POLYGON_IN_FRONT_OF_PLANE 5
#define POLYGON_BEHIND_PLANE 6
#define POLYGON_COPLANAR_WITH_PLANE 7
#define COPLANAR_WITH_PLANE 8
#define IN_FRONT_OF_PLANE 9
#define BEHIND_PLANE 10
#define STRADDLING_PLANE 11


inline bool CompareVec3(glm::vec3 a, glm::vec3 b)
{
  return abs(a.x - b.x) <= EPSILON && abs(a.y - b.y) <= EPSILON && abs(a.z - b.z) <= EPSILON;
}

inline void UniqueAddVector(const glm::vec3& vec, std::vector<glm::vec3>& verties, std::vector<glm::uint>& indices)
{
  bool vertexAdded = false;
  for (size_t j = 0; j < verties.size(); ++j)
  {
    if (CompareVec3(verties[j], vec))
    {
      indices.emplace_back((glm::uint)j);
      vertexAdded = true;
      break;
    }
  }

  if (!vertexAdded)
  {
    verties.emplace_back(vec);
    indices.emplace_back((glm::uint)(verties.size() - 1));
  }
}

inline void UniqueAddTriangle(const Triangle& tri, std::vector<glm::vec3>& verties, std::vector<glm::uint>& indices)
{
  for (size_t k = 0; k < tri.size(); k++)
  {
    bool vertexAdded = false;
    for (size_t j = 0; j < verties.size(); ++j)
    {
      if (CompareVec3(verties[j], tri[k]))
      {
        indices.emplace_back((glm::uint)j);
        vertexAdded = true;
        break;
      }
    }

    if (!vertexAdded)
    {
      verties.emplace_back(tri[k]);
      indices.emplace_back((glm::uint)(verties.size() - 1));
    }
  }
}

inline void UniqueAddVerties(std::vector<glm::vec3>& fromVerties, std::vector<glm::uint>& fromIndices, std::vector<glm::vec3>& toVerties, std::vector<glm::uint>& toIndices)
{
  for (size_t i = 0; i < fromIndices.size(); i++)
  {
    bool vertexAdded = false;
    for (size_t j = 0; j < toVerties.size(); ++j)
    {
      if (CompareVec3(fromVerties[fromIndices[i]], toVerties[j]))
      {
        toIndices.emplace_back((glm::uint)j);
        vertexAdded = true;
        break;
      }
    }

    if (!vertexAdded)
    {
      toVerties.emplace_back(fromVerties[fromIndices[i]]);
      toIndices.emplace_back((glm::uint)(toVerties.size() - 1));
    }
  }
}

struct BSPNode
{
  BSPNode(const std::vector<glm::vec3>& _verties, const std::vector<glm::uint>& _indices) :
    frontTree(nullptr),
    backTree(nullptr),
    verties(_verties),
    indices(_indices)
  {
  }

  BSPNode(BSPNode* _frontTree, BSPNode* _backTree) :
    frontTree(_frontTree),
    backTree(_backTree)
  {
    if (!_frontTree || !_backTree)
    {
      return;
    }
    verties = frontTree->verties;
    indices = frontTree->indices;

    verties.insert(verties.end(), backTree->verties.begin(), backTree->verties.end());
  }

  BSPNode* frontTree;
  BSPNode* backTree;
  Plane splitPlane;

  std::vector<glm::vec3> verties;
  std::vector<glm::uint> indices;

  bool showLines = false;

  void Draw();
};

inline Plane GetPlaneFromTriangle(Triangle triangle)
{
  return Plane(triangle);
}

inline glm::vec3 IntersectEdgeAgainstPlane(glm::vec3 a, glm::vec3 b, Plane plane)
{
  Ray ray;
  ray.m_Position = a;
  glm::vec3 temp = b - a;
  ray.m_Direction = glm::normalize(temp);

  float t;
  Collision::RayPlane(ray, plane, t);

  glm::vec3 intersection = a + ray.m_Direction * t;

  return intersection;
}

// Classify point p to a plane thickened by a given thickness epsilon
inline int ClassifyPointToPlane(glm::vec3 p, Plane plane)
{
  // Compute signed distance of point from plane
  float dist = glm::dot(glm::vec3(plane.m_Normal), p) - plane.m_Normal.w;
  // Classify p based on the signed distance
  if (dist > PLANE_THICKNESS_EPSILON)
    return POINT_IN_FRONT_OF_PLANE;
  if (dist < -PLANE_THICKNESS_EPSILON)
    return POINT_BEHIND_PLANE;
  return POINT_ON_PLANE;
}

// Return value specifying whether the polygon ‘poly’ lies in front of,
// behind of, on, or straddles the plane ‘plane’
inline int ClassifyTriangleToPlane(Triangle triangle, Plane plane)
{
  // Loop over all polygon vertices and count how many vertices
  // lie in front of and how many lie behind of the thickened plane
  int numInFront = 0, numBehind = 0;
  for (size_t i = 0; i < triangle.size(); i++)
  {
    glm::vec3 p = triangle[i];
    switch (ClassifyPointToPlane(p, plane))
    {
    case POINT_IN_FRONT_OF_PLANE:
      numInFront++;
      break;
    case POINT_BEHIND_PLANE:
      numBehind++;
      break;
    }
  }
  // If vertices on both sides of the plane, the polygon is straddling
  if (numBehind != 0 && numInFront != 0)
    return POLYGON_STRADDLING_PLANE;
  // If one or more vertices in front of the plane and no vertices behind
  // the plane, the polygon lies in front of the plane
  if (numInFront != 0)
    return POLYGON_IN_FRONT_OF_PLANE;
  // Ditto, the polygon lies behind the plane if no vertices in front of
  // the plane, and one or more vertices behind the plane
  if (numBehind != 0)
    return POLYGON_BEHIND_PLANE;
  // All vertices lie on the plane so the polygon is coplanar with the plane
  return POLYGON_COPLANAR_WITH_PLANE;
}

inline void SplitTriangle(const Triangle& triangle, const Plane& plane, std::vector<glm::vec3>& frontVerties, std::vector<glm::uint>& frontIndices, std::vector<glm::vec3>& backVerties, std::vector<glm::uint>& backIndices)
{
  // Test all edges (a, b) starting with edge from last to first vertex
  glm::vec3 a = triangle[triangle.size() - 1];
  int aSide = ClassifyPointToPlane(a, plane);
  // Loop over all edges given by vertex pair (n - 1, n)
  for (size_t n = 0; n < triangle.size(); n++)
  {
    glm::vec3 b = triangle[n];
    int bSide = ClassifyPointToPlane(b, plane);
    if (bSide == POINT_IN_FRONT_OF_PLANE)
    {
      if (aSide == POINT_BEHIND_PLANE)
      {
        // Edge (a, b) straddles, output intersection point to both sides
        glm::vec3 i = IntersectEdgeAgainstPlane(a, b, plane);
        assert(ClassifyPointToPlane(i, plane) == POINT_ON_PLANE);
        UniqueAddVector(i, frontVerties, frontIndices);
        UniqueAddVector(i, backVerties, backIndices);
      }
      // In all three cases, output b to the front side
      UniqueAddVector(b, frontVerties, frontIndices);
    }
    else if (bSide == POINT_BEHIND_PLANE)
    {
      if (aSide == POINT_IN_FRONT_OF_PLANE)
      {
        // Edge (a, b) straddles plane, output intersection point
        glm::vec3 i = IntersectEdgeAgainstPlane(a, b, plane);
        assert(ClassifyPointToPlane(i, plane) == POINT_ON_PLANE);
        UniqueAddVector(i, frontVerties, frontIndices);
        UniqueAddVector(i, backVerties, backIndices);
      }
      else if (aSide == POINT_ON_PLANE)
      {
        // Output a when edge (a, b) goes from ‘on’ to ‘behind’ plane
        UniqueAddVector(a, backVerties, backIndices);
      }
      // In all three cases, output b to the back side
      UniqueAddVector(b, backVerties, backIndices);
    }
    else
    {
      // b is on the plane. In all three cases output b to the front side
      UniqueAddVector(b, frontVerties, frontIndices);
      // In one case, also output b to back side
      if (aSide == POINT_BEHIND_PLANE)
        UniqueAddVector(b, backVerties, backIndices);
    }
    // Keep b as the starting point of the next edge
    a = b;
    aSide = bSide;
  }
}

// Given a vector of polygons, attempts to compute a good splitting plane
inline Plane PickSplittingPlane(const std::vector<glm::vec3>& verties, const std::vector<glm::uint>& indices)
{
  // Blend factor for optimizing for balance or splits (should be tweaked)
  const float K = 0.8f;
  // Variables for tracking best splitting plane seen so far
  Plane bestPlane;
  float bestScore = FLT_MAX;
  // Try the plane of each polygon as a dividing plane
  for (size_t i = 0; i < indices.size(); i += 3)
  {
    int numInFront = 0, numBehind = 0, numStraddling = 0;
    Triangle triI(verties[indices[i]], verties[indices[i + 1]], verties[indices[i + 2]]);
    Plane plane = GetPlaneFromTriangle(triI);
    // Test against all other polygons
    for (size_t j = 0; j < indices.size(); j += 3)
    {
      // Ignore testing against self
      if (i == j) continue;

      Triangle triJ(verties[indices[j]], verties[indices[j + 1]], verties[indices[j + 2]]);

      // Keep standing count of the various poly-plane relationships
      switch (ClassifyTriangleToPlane(triJ, plane))
      {
      case POLYGON_COPLANAR_WITH_PLANE:
        //Coplanar polygons treated as being in front of plane 
      case POLYGON_IN_FRONT_OF_PLANE:
        numInFront++;
        break;
      case POLYGON_BEHIND_PLANE:
        numBehind++;
        break;
      case POLYGON_STRADDLING_PLANE:
        numStraddling++;
        break;
      }
    }
    // Compute score as a weighted combination (based on K, with K in range
    // 0..1) between balance and splits (lower score is better)
    float score = K * numStraddling + (1.0f - K) * std::abs(numInFront - numBehind);
    if (score < bestScore)
    {
      bestScore = score;
      bestPlane = plane;
    }
  }
  return bestPlane;
}

// Constructs BSP tree from an input vector of polygons. Pass ‘depth’ as 0 on entry
inline BSPNode* BuildBSPTree(const std::vector<glm::vec3>& verties, const std::vector<glm::uint>& indices, int depth)
{
  // Return NULL tree if there are no polygons
  if (verties.empty()) return NULL;
  // Get number of polygons in the input vector
  int numPolygons = (int)indices.size() / 3;
  // If criterion for a leaf is matched, create a leaf node from remaining polygons
  if (depth >= MAX_DEPTH || numPolygons <= MIN_LEAF_SIZE)
    return new BSPNode(verties, indices);


  // Select best possible partitioning plane based on the input geometry
  Plane splitPlane = PickSplittingPlane(verties, indices);

  std::vector<glm::vec3> frontVerties, backVerties;
  std::vector<glm::uint> frontIndices, backIndices;

  // Test each polygon against the dividing plane, adding them
  // to the front list, back list, or both, as appropriate
  for (size_t i = 0; i < indices.size(); i += 3)
  {
    Triangle triI(verties[indices[i]], verties[indices[i + 1]], verties[indices[i + 2]]);

    std::vector<glm::vec3> frontPartVerties, backPartVerties;
    std::vector<glm::uint> frontPartIndices, backPartIndices;

    switch (ClassifyTriangleToPlane(triI, splitPlane))
    {
    case COPLANAR_WITH_PLANE:
      // What’s done in this case depends on what type of tree is being
      // built. For a node-storing tree, the polygon is stored inside
      // the node at this level (along with all other polygons coplanar
      // with the plane). Here, for a leaf-storing tree, coplanar polygons
      // are sent to either side of the plane. In this case, to the front
      // side, by falling through to the next case
    case IN_FRONT_OF_PLANE:
      UniqueAddTriangle(triI, frontVerties, frontIndices);
      break;
    case BEHIND_PLANE:
      UniqueAddTriangle(triI, backVerties, backIndices);
      break;
    case STRADDLING_PLANE:
      /*
      // Split polygon to plane and send a part to each side of the plane
      SplitTriangle(triI, splitPlane, frontPartVerties, frontPartIndices, backPartVerties, backPartIndices);
      UniqueAddVerties(frontPartVerties, frontPartIndices, frontVerties, frontIndices);
      UniqueAddVerties(backPartVerties, backPartIndices, backVerties, backIndices);
      */
      break;
    }
  }
  // Recursively build child subtrees and return new tree root combining them
  BSPNode* frontTree = BuildBSPTree(frontVerties, frontIndices, depth + 1);
  BSPNode* backTree = BuildBSPTree(backVerties, backIndices, depth + 1);
  BSPNode* returnTree = new BSPNode(frontTree, backTree);
  returnTree->splitPlane = splitPlane;
  return returnTree;
}
#endif