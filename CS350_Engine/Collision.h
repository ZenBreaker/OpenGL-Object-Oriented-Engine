/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
File Name: Collision.h
Purpose: 
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
Project: michael.ngo_CS350_2
Author: Michael Ngo, michael.ngo, 90003217
Creation date: 2/21/2020
End Header --------------------------------------------------------*/

#ifndef COLLISION_H
#define COLLISION_H
#include <glm/glm.hpp>


#include "AABB.h"
#include "Plane.h"
#include "Point3D.h"
#include "Ray.h"
#include "Sphere.h"
#include "Triangle.h"

const float EPSILON = 0.0001f;
const float MAX_RAY_DISTANCE = 10000;

template <class T>
static void SwapTwo(T a, T b)
{
  const float temp = a;
  a = b;
  b = temp;
}

class Collision
{
public:
  //Basic Intersection
  static bool SphereSphere(const Sphere& a, const Sphere& b)
  {
    return PointSphere(b.m_Position,
      Sphere(a.m_Position, a.m_Radius + b.m_Radius));
  }

  static bool AABBAABB(const AABB& a, const AABB& b)
  {
    // for each axis { X, Y, Z }
    for (unsigned int i = 0; i < 3; ++i)
    {
      // if no overlap for the axis, no overlap overall
      if (a.m_Max[i] < b.m_Min[i] || b.m_Max[i] < a.m_Min[i])
        return false;
    }
    return true;
  }

  static bool AABBSphere(const AABB& aabb, const Sphere& sphere)
  {
    const glm::vec3 point = glm::vec3(glm::max(aabb.m_Min.x, glm::min(sphere.m_Position.x, aabb.m_Min.x)),
                                      glm::max(aabb.m_Min.y, glm::min(sphere.m_Position.y, aabb.m_Min.y)),
                                      glm::max(aabb.m_Min.z, glm::min(sphere.m_Position.z, aabb.m_Min.z)));

    // this is the same as isPointInsideSphere
    const float distance = glm::length(point - sphere.m_Position);

    return distance < sphere.m_Radius;
  }

  static bool SphereAABB(const Sphere& a, const AABB& b)
  {
    return AABBSphere(b, a);
  }

  //Point-based
  static bool PointSphere(const Point3D& p, const Sphere& S)
  {
    const float d = glm::distance(p.m_Position, S.m_Position);
    const float r_sq = (S.m_Radius * S.m_Radius);
    return ((d * d) <= r_sq);
  }

  static bool PointAABB(const Point3D& point, const AABB& aabb)
  {
    //    !(f || f || f || f || f || f)
    return (point.m_Position.x > aabb.m_Max.x  && point.m_Position.x < aabb.m_Min.x &&
            point.m_Position.y > aabb.m_Max.y  && point.m_Position.y < aabb.m_Min.y &&
            point.m_Position.z > aabb.m_Max.z  && point.m_Position.z < aabb.m_Min.z);
  }

  static bool PointPlane(const Point3D& point, const Plane& plane)
  {
    return plane.DistanceFromPlane(point.m_Position) < EPSILON;
  }

  static bool PointTriangle(const Point3D& point, const Triangle& triangle)
  {
    glm::vec3 u = triangle.v2 - triangle.v1;
    glm::vec3 v = triangle.v3 - triangle.v1;
    glm::vec3 w = point.m_Position - triangle.v1;

    float a = (glm::dot(u, v) * glm::dot(w, v) - glm::dot(v, v) * glm::dot(w, u))
            / (glm::dot(u, v) * glm::dot(u, v) - glm::dot(u, u) * glm::dot(v, v));


    float b = (glm::dot(u, v) * glm::dot(w, v) - glm::dot(u, u) * glm::dot(w, v))
            / (glm::dot(u, v) * glm::dot(u, v) - glm::dot(u, u) * glm::dot(v, v));

    return a >= 0 && b >= 0 && a + b <= 1;
  }

  //Ray-based
  static bool RayPlane(const Ray& ray, const Plane& plane)
  {
    const float position = glm::dot(ray.m_Position, (glm::vec3)plane.m_Normal) + plane.m_Normal.w;

    if(position >= EPSILON) // in front of the plane
    {
      return glm::dot(ray.m_Direction, glm::vec3(plane.m_Normal)) < 0;
    }
    if(position <= -EPSILON) // behind the plane
    {
      return glm::dot(ray.m_Direction, glm::vec3(plane.m_Normal)) > 0;
    }

    //on the plane
    return true;
  }

  static bool RayPlane(const Ray& ray, const Plane& plane, float& t)
  {
    // assuming vectors are all normalized
    float denom = glm::dot((glm::vec3)plane.m_Normal, ray.m_Direction);
    if (denom > 1e-6) {
      glm::vec3 p0l0 = plane.m_RandomPointOnPlane - ray.m_Position;
      t = glm::dot(p0l0, (glm::vec3)plane.m_Normal) / denom;
      return (t >= 0);
    }

    return false;
  }

  static bool RayAAABB(const Ray& ray, const AABB& aabb)
  {
    float tmin = (aabb.m_Min.x - ray.m_Position.x) / ray.m_Direction.x;
    float tmax = (aabb.m_Max.x - ray.m_Position.x) / ray.m_Direction.x;

    if (tmin > tmax) SwapTwo(tmin, tmax);

    float tymin = (aabb.m_Min.y - ray.m_Position.y) / ray.m_Direction.y;
    float tymax = (aabb.m_Max.y - ray.m_Position.y) / ray.m_Direction.y;

    if (tymin > tymax) SwapTwo(tymin, tymax);

    if ((tmin > tymax) || (tymin > tmax))
      return false;

    if (tymin > tmin)
      tmin = tymin;

    if (tymax < tmax)
      tmax = tymax;

    float tzmin = (aabb.m_Min.z - ray.m_Position.z) / ray.m_Direction.z;
    float tzmax = (aabb.m_Max.z - ray.m_Position.z) / ray.m_Direction.z;

    if (tzmin > tzmax) SwapTwo(tzmin, tzmax);

    if ((tmin > tzmax) || (tzmin > tmax))
      return false;

    if (tzmin > tmin)
      tmin = tzmin;

    if (tzmax < tmax)
      tmax = tzmax;

    return true;
  }

  static bool RaySphere(const Ray& ray, const Sphere& sphere, float &t, Point3D &q)
  {
    glm::vec3 m = ray.m_Position - sphere.m_Position;
    float b = glm::dot(m, ray.m_Direction);
    float c = dot(m, m) - sphere.m_Radius * sphere.m_Radius;
    // Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
    if (c > 0.0f && b > 0.0f) return false;
    float discr = b * b - c;
    // A negative discriminant corresponds to ray missing sphere
    if (discr < 0.0f) return false;
    // Ray now found to intersect sphere, compute smallest t value of intersection
    t = -b - sqrt(discr);
    // If t is negative, ray started inside sphere so clamp t to zero
    if (t < 0.0f) t = 0.0f;
    q = ray.m_Position + t * ray.m_Direction;
    return true;
  }

  bool RayTriangle(const Ray& ray, const Triangle& triangle, float& u, float& v, float& w)
  {
    glm::vec3 pq = (ray.m_Position + MAX_RAY_DISTANCE * ray.m_Direction) - ray.m_Position;
    glm::vec3 pa = triangle.v1 - ray.m_Position;
    glm::vec3 pb = triangle.v2 - ray.m_Position;
    glm::vec3 pc = triangle.v3 - ray.m_Position;

    // Test if pq is inside the edges bc, ca and ab. Done by testing
    // that the signed tetrahedral volumes, computed using scalar triple
    // products, are all positive
    glm::vec3 m = glm::cross(pq, (ray.m_Position + MAX_RAY_DISTANCE * ray.m_Direction));
    float s = glm::dot(m, triangle.v3 - triangle.v2);
    float t = glm::dot(m, triangle.v1 - triangle.v3);
    u = glm::dot(pq, glm::cross(triangle.v3, triangle.v2)) + s;
    if (u < 0.0f) return false;
    v = glm::dot(pq, glm::cross(triangle.v1, triangle.v3)) + t;
    if (v < 0.0f) return false;
    w = glm::dot(pq, glm::cross(triangle.v2, triangle.v1)) - s - t;
    if (w < 0.0f) return false;

    // Compute the barycentric coordinates (u, v, w) determining the
    // intersection point r, r = u*a + v*b + w*c
    float denom = 1.0f / (u + v + w);
    u *= denom;
    v *= denom;
    w *= denom; // w = 1.0f - u - v;

    return true;
  }

  //Plane-based
  bool PlaneAABB(const Plane& plane, const AABB& aabb)
  {
    glm::vec3 points[8];

    points[0] = glm::vec3(aabb.m_Max.x, aabb.m_Max.y, aabb.m_Max.z);
    points[1] = glm::vec3(aabb.m_Max.x, aabb.m_Min.y, aabb.m_Max.z);
    points[2] = glm::vec3(aabb.m_Max.x, aabb.m_Max.y, aabb.m_Min.z);
    points[3] = glm::vec3(aabb.m_Max.x, aabb.m_Min.y, aabb.m_Min.z);
    points[4] = glm::vec3(aabb.m_Min.x, aabb.m_Max.y, aabb.m_Max.z);
    points[5] = glm::vec3(aabb.m_Min.x, aabb.m_Min.y, aabb.m_Max.z);
    points[6] = glm::vec3(aabb.m_Min.x, aabb.m_Max.y, aabb.m_Min.z);
    points[7] = glm::vec3(aabb.m_Min.x, aabb.m_Min.y, aabb.m_Min.z);

    int inFront = 0;

    for(const glm::vec3& point : points)
    {
      if(glm::dot(point, (glm::vec3)plane.m_Normal) - plane.m_Normal.w > EPSILON)
      {
        ++inFront;
      }
    }
    //                  F
    //             T    &&   F
    //           8 != 0 && 8 != 8
    //                  T
    //             T    &&   T
    //           7 != 0 && 7 != 8
    return inFront != 0 && inFront != 8;
  }

  bool PlaneTriangle(const Plane& plane, const Triangle& triangle)
  {
    bool inFront[3];

    inFront[0] = glm::dot(triangle.v1,(glm::vec3)plane.m_Normal) - plane.m_Normal.w < EPSILON;
    inFront[1] = glm::dot(triangle.v2,(glm::vec3)plane.m_Normal) - plane.m_Normal.w < EPSILON;
    inFront[2] = glm::dot(triangle.v3,(glm::vec3)plane.m_Normal) - plane.m_Normal.w < EPSILON;
    
    return !(inFront[0] && inFront[1] && inFront[2] || !inFront[0] && !inFront[1] && !inFront[2]);
  }
};

#endif