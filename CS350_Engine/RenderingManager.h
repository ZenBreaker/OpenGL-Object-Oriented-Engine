/* Start Header -------------------------------------------------------
Copyright (C) 2020 DigiPen Institute of Technology.
File Name: RenderingManager.h
Purpose: Manages the rendering of objects. Uses Deferred Rendering
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
Author: Michael Ngo
Creation date: 2/2/2020
End Header --------------------------------------------------------*/
#ifndef RENDERINGMANAGER_H
#define RENDERINGMANAGER_H

#include "Scene.h"
#include "Object.h"

/**
 * @brief 
 *  Geometry Buffer Names
 */
enum GeometryBuffer
{
  All,
  Position,
  Normal,
  Diffuse,
  Ambient,
  Specular,
  Emissive,
};

// String version of Geometry Buffer
static const char* const GeometryBufferNames[] =
{
  "All",
  "Position",
  "Normal",
  "Diffuse",
  "Ambient",
  "Specular",
  "Emissive",
};

/**
 * @brief 
 *   Light Data struct
 */
struct LightData
{
  Light lights[16]; //!< array of lights that maxes out at 16 lights

  int numberOfLights; //!< current number of lights 
  float constant;     //!< constant light falloff
  float linear;       //!< linear light falloff
  float quadratic;    //!< quadratic light falloff

  glm::vec3 GlobalAmbient; //!< color of the global ambiant
  float ZFar;              //!< Z far distance

  glm::vec3 Atmospheric; //!< not yet implemented
  float ZNear;           //!< Z near distance

  glm::vec4 IFog; //!< not yet implemented
};

/**
 * @brief 
 *   Rendering Manager, handles deferred Rendering 
 */
class RenderingManager
{
public:
  // Default constructor for new Rendering Manager
  RenderingManager();

  /**
   * @brief
   *   Destructure for Rendering Manager
   */
  ~RenderingManager() { Shutdown(); }

  void GenerateGeometryBuffers(int width, int height);
  void LoadDebugShader();
  // initialize for the Rendering Manager
  void Init();

  // Delete buffers
  void Shutdown();

  // Pre Renders the scene, does first pass of deferred shading
  void PreRender(Scene* scene);

  // Renders the scene, does the second pass of deferred shading
  void Render(const Scene* scene);

  // Renders the scene, renders forward objects, like light positions
  void PostRender(Scene* scene);

  void ReloadShader(ShaderIndex index);


  GLuint m_SSBOUniform; //!< SSBO uniform location
  LightData m_Lights;   //!< light data 
  GeometryBuffer m_FSQ; //!< what Geometry buffer to draw
  bool m_DepthCopy;     //!< copy over depth

  glm::mat4 m_Projection; //!< projection matrix
  glm::mat4 m_View;       //!< view matrix 

  glm::vec3 m_NormalColorVertex;
  glm::vec3 m_NormalColorFace;
  float m_NormalLength;
  glm::vec3 m_BackgroundColor;
private:
  // Render a single object into the current framebuffer
  void RenderObject(const Scene* scene, Object& object, GLuint& lastBindedProgramID);
  void RenderNormals(const Scene* scene, Object& object, GLuint& lastBindedProgramID);
  
  // Render a full screen quad
  void RenderQuad() const;

  glm::vec4 m_Eye; //!< eye's position to render from

  GLuint m_GeometryBuffer;          //!< geometry framebuffer
  GLuint m_GeometryPosition;        //!< position texture
  GLuint m_GeometryNormal;          //!< normal texture
  GLuint m_GeometryDiffuse;      //!< color texture
  GLuint m_GeometrySpecular;      //!< color texture
  GLuint m_GeometryEmissive;      //!< color texture
  GLuint m_GeometryAmbient;      //!< color texture
  GLuint m_RenderBufferObjectDepth; //!< render buffer object for depth

  GLuint m_GeometryPositionUniform;   //!< position uniform location
  GLuint m_GeometryNormalUniform;     //!< normal uniform location
  GLuint m_GeometryDiffuseUniform; //!< color uniform location
  GLuint m_GeometrySpecularUniform; //!< color uniform location
  GLuint m_GeometryAmbientUniform; //!< color uniform location
  GLuint m_GeometryEmissiveUniform;
  GLuint m_EyeUniform; //!< color uniform location

  GLuint m_FullQuadVAO = 0; //!< screen quad VAO
  GLuint m_FullQuadVBO;     //!< screen quad VBO

  GLuint m_DebugShader;
  GLuint m_NormalModeUniform;
  GLuint m_NormalMVPUniform;
  GLuint m_NormalInverseMVPUniform;
  GLuint m_NormalColorVertexUniform;
  GLuint m_NormalColorFaceUniform;
  GLuint m_NormalLengthUniform;

};

#endif
