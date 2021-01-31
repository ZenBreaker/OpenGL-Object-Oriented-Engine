/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: Scene.cpp
Purpose: holds scene data
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
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/color_space.hpp>

#include "Scene.h"
#include "Engine.h"


Scene::Scene(SceneIndex number)
{
  m_Camera.m_Position = glm::vec3(0.0f,0.0f,1.0f);
  m_BoundingHierarchy.m_Construction = BoundingHierarchy::Construction::BottomUp;
  m_BoundingHierarchy.m_Method = BoundingHierarchy::Method::AABB;
  m_IsDrawingBoundingHierarchy = false;

	switch (number)
	{
  case ZERO:
    break;
  case ONE:
  {
    Object& object1 = AddObject();
    object1.m_Model = Engine::get().m_AssetManager.GetModel(Model::Bunny);
    object1.SetShader(Engine::get().m_AssetManager.GetShader(ShaderIndex::DeferredFirstPassShader));
    object1.SetCentroid(glm::vec3(0.0f, 0.0f, 0.0f));
    object1.m_Material.ambiant_color = glm::vec3(1.0f, 0.3f, 1.0f);

    Object& object2 = AddObject();
    //object2.SetScaleVector({ 0,0,0 });
    object2.m_Model = Engine::get().m_AssetManager.GetModel(Model::Sphere);
    object2.SetShader(Engine::get().m_AssetManager.GetShader(ShaderIndex::DeferredFirstPassShader));
    object2.SetCentroid(glm::vec3(2.17f, -0.590f, 0.51f));
    object2.m_Material.ambiant_color = glm::vec3(0.4f, 1.0f, 1.0f);


    Object& object3 = AddObject();
    //object3.SetScaleVector({ 0,0,0 });
    object3.m_Model = Engine::get().m_AssetManager.GetModel(Model::FourSphere);
    object3.SetShader(Engine::get().m_AssetManager.GetShader(ShaderIndex::DeferredFirstPassShader));
    object3.SetCentroid(glm::vec3(-1.690f, 0.88f, -0.870f));
    object3.m_Material.ambiant_color = glm::vec3(1.0f, 1.0f, 0.0f);

    Object& object4 = AddObject();
    //object4.SetScaleVector({ 0,0,0 });
    object4.m_Model = Engine::get().m_AssetManager.GetModel(Model::BunnyHighPoly);
    object4.SetShader(Engine::get().m_AssetManager.GetShader(ShaderIndex::DeferredFirstPassShader));
    object4.SetCentroid(glm::vec3(-0.60f, -0.430f, 0.310f));
    object4.m_Material.ambiant_color = glm::vec3(1.0f, 1.0f, 1.0f);

    for (int i = 0; i < 6; i++)
    {
      float hue = 360.0f * ((float)rand() / (float)RAND_MAX - 0.5f) * 8;
      Object& light1 = AddLight();
      light1.m_Model = Engine::get().m_AssetManager.GetModel(Model::Sphere);
      light1.SetShader(Engine::get().m_AssetManager.GetShader(ShaderIndex::LightShader));
      light1.SetCentroid(glm::vec3(0.0f, 0.0f, 0.0f));
      light1.SetScaleVector(glm::vec3(.1f));
      light1.m_Material.ambiant_color = glm::rgbColor(glm::vec3(hue, 1.0f, 0.5f));
      light1.m_RotationAmount = 3.14159f / 4.0f;
      light1.m_Light = new Light();
      light1.m_Light->ambiant = glm::rgbColor(glm::vec3(hue, 1.0f, 0.5f));
      light1.m_Light->diffuse = glm::rgbColor(glm::vec3(hue, 1.0f, 1.0f));
      light1.m_Light->type = 0;
      light1.m_IsRotating = true;
    }
    
    break;
  }
  default:
    break;
	}
}

Scene::~Scene()
{
  m_Lights.clear();
  m_Objects.clear();
}

void Scene::Update(float deltaTime)
{
  for(Object& object : m_Objects)
  {
    object.Update(deltaTime);
  }

  for (int i = 0; i < m_Lights.size(); ++i)
  {
    m_Lights[i].Update(deltaTime);
    float x = 2 * cos((i * 360.0f / m_Lights.size() * 3.1415f / 180.0f) + m_Lights[i].RotationAngle());
    float y = 2 * sin((i * 360.0f / m_Lights.size() * 3.1415f / 180.0f) + m_Lights[i].RotationAngle());

    m_Lights[i].SetCentroid(glm::vec3(x, 0.0f, y));
    m_Lights[i].m_Light->position = m_Lights[i].Centroid();
    m_Lights[i].m_Light->direction = glm::rotate(m_Lights[i].RotationAngle(), m_Lights[i].RotationVector()) * glm::vec4(1.0f);
    m_Lights[i].m_Light->ambiant = m_Lights[i].m_Material.ambiant_color;
  }

  if(m_IsDrawingBoundingHierarchy)
  {
    m_BoundingHierarchy.Draw(m_Objects);
  }
}

Object& Scene::AddObject()
{
  m_Objects.emplace_back("Object");

  return m_Objects.back();
}

Object& Scene::AddLight()
{
  m_Lights.emplace_back("Light");

  return m_Lights.back();
}
