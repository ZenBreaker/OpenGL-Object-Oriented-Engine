/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: Editor.cpp
Purpose: gui interface with dear imgui
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
Project: michael.ngo_CS350_1
Author: Michael Ngo, michael.ngo, 90003217
Creation date: 2/2/2020
End Header --------------------------------------------------------*/

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include <algorithm>

#include "Editor.h"
#include "RenderingManager.h"

/**
 * @brief 
 *   Constructor a new Editor object
 */
Editor::Editor()
{
}

/**
 * @brief 
 *   initialize Editor
 * 
 * @param window 
 *   GLFW window
 * 
 * @param glsl_version 
 *   GLSL version number
 */
void Editor::Init(GLFWwindow* window, const char* glsl_version)
{
  // initialize imgui 
  ImGui::CreateContext();
  bool show_demo_window = true;
  ImGuiIO& io = ImGui::GetIO();
  ImGui::StyleColorsDark();
  ImGui_ImplGlfw_InitForOpenGL(window, true);
  ImGui_ImplOpenGL3_Init(glsl_version);
}

/**
 * @brief 
 *   shutdown editor 
 */
void Editor::Shutdown()
{
  // shutdown imgui
  ImGui_ImplOpenGL3_Shutdown();
  ImGui::DestroyContext();
}

/**
 * @brief 
 *   Pre Render of the editor
 */
void Editor::PreRender()
{
}

/**
 * @brief 
 *   Render of the editor
 */
void Editor::Render()
{
}

void OctTreeRecursion_aux(OctTreeNode* node, std::string boxstring)
{
  if (!node) 
    return;

  ImGui::PushStyleColor(0, ImColor::HSV(180, node->level * 1.0f / node->maxDepth ,1).Value);

  for (size_t i = 0; i < 8; i++)
  {
    if (node->childrenList[i])
    {
      if (ImGui::TreeNode(node->childrenList[i], (boxstring + " [%i]").c_str(), (int)i))
      {
        node->childrenList[i]->showBoxes = true;

        std::string nodeString = boxstring + " [";
        nodeString.append(std::to_string(i));
        nodeString.append("]");

        if (node->childrenList[i]->childrenList[0])
        {
          OctTreeRecursion_aux(node->childrenList[i], nodeString);
        }

        ImGui::TreePop();
      }
      else
      {
        node->childrenList[i]->showBoxes = false;
      }
    }
  }
  ImGui::PopStyleColor();

}

void OctTreeRecursion()
{
  OctTreeNode* head = Engine::get().m_SceneManager.m_CurrentScene->m_OctTreeHead;
  head->showBoxes = true;
  std::string boxstring = "Oct Tree Child";
  OctTreeRecursion_aux(head, boxstring);
}

/**
 * @brief 
 *   Post Render for the editor
 */
void Editor::PostRender()
{
  // imgui new frame
  ImGui_ImplOpenGL3_NewFrame();
  ImGui_ImplGlfw_NewFrame();
  ImGui::NewFrame();

  //imgui begin
  ImGui::Begin("OpenGL-Object-Oriented-Engine");

  //frame rate
  ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

  //time scale
  if (ImGui::DragFloat("Time Scale", &Engine::get().m_TimeScale)) {}

  // geometry buffer display
  {
    bool has_changed = false;

    // get current g buffers
    int currentindex = (int)Engine::get().m_RenderingManager.m_FSQ;

    // g bufer drop down menu
    if (ImGui::BeginCombo("Geometry Buffer Name", GeometryBufferNames[currentindex]))
    {
      for (std::size_t j = 0; j < sizeof(GeometryBufferNames) / sizeof(char *); ++j)
      {
        if (ImGui::Selectable(GeometryBufferNames[j], currentindex == j))
        {
          currentindex = (int)j;
          has_changed = true;
        }
      }
      ImGui::EndCombo();
    }

    if (has_changed)
    {
      // update what g buffer to render
      Engine::get().m_RenderingManager.m_FSQ = (GeometryBuffer)currentindex;
    }
  }


  if (ImGui::Button("Update Oct Tree"))
  {
    Engine::get().m_SceneManager.m_CurrentScene->UpdateOctTree();
  }

  if (Engine::get().m_SceneManager.m_CurrentScene->m_OctTreeHead)
  {

    if (ImGui::TreeNode("Oct Tree"))
    {
      OctTreeRecursion();
      ImGui::TreePop();

    }
  }

  if (ImGui::Button("Update BSP"))
  {
    Engine::get().m_SceneManager.m_CurrentScene->UpdateBSP();
  }
  
  if (Engine::get().m_SceneManager.m_CurrentScene->m_BSPHead)
  {
    if (ImGui::Checkbox("Show BSP", &Engine::get().m_SceneManager.m_CurrentScene->m_BSPHead->showLines)) {}
  }

  if (ImGui::ColorEdit3("Background Color", &Engine::get().m_RenderingManager.m_BackgroundColor[0])) {}

  if(ImGui::Checkbox("Depth Copy", &Engine::get().m_RenderingManager.m_DepthCopy)) {}

  if(ImGui::TreeNode("Camera"))
  {
    if (ImGui::DragFloat3("Position Vector", &Engine::get().m_SceneManager.m_CurrentScene->m_Camera.m_Position[0])) {}
    if (ImGui::DragFloat3("Direction Vector", &Engine::get().m_SceneManager.m_CurrentScene->m_Camera.m_Foward[0])) {}
    if (ImGui::DragFloat("Speed", &Engine::get().m_SceneManager.m_CurrentScene->m_Camera.m_BaseSpeed)) {}
    if (ImGui::Checkbox("Speed Time Dependent", &Engine::get().m_SceneManager.m_CurrentScene->m_Camera.m_TimeDependent)) {}

    ImGui::TreePop();
  }

  if(ImGui::Checkbox("Bounding Hierarchy", &Engine::get().m_SceneManager.m_CurrentScene->m_IsDrawingBoundingHierarchy))
  {
  }

  if (ImGui::Button("Reload Light Shader"))
  {
    Engine::get().m_RenderingManager.ReloadShader(ShaderIndex::LightingPass);
  }

  if (ImGui::TreeNode("Lights"))
  {
    // loop through all light objects
    for (int i = 0; i < Engine::get().m_SceneManager.m_CurrentScene->m_Lights.size(); i++)
    {
      Object& light = Engine::get().m_SceneManager.m_CurrentScene->m_Lights[i];

      if (ImGui::TreeNode(&light, "Light Object: [%i]", i))
      {
        if (ImGui::Checkbox("Rotates", &light.m_IsRotating))
        {

        }
        //if (ImGui::TreeNode("Model"))
        {
          bool has_changed = false;
          int currentindex = (int)light.m_Model->m_ModelIndex;
          if (ImGui::BeginCombo("Model Name", Model::IndexNames[currentindex].c_str()))
          {
            for (std::size_t j = 0; j < Model::IndexNames.size(); ++j)
            {
              if (ImGui::Selectable(Model::IndexNames[j].c_str(), currentindex == j))
              {
                currentindex = (int)j;
                has_changed = true;
              }
            }
            ImGui::EndCombo();
          }

          if (has_changed)
          {
            light.m_Model = Engine::get().m_AssetManager.GetModel((Model::Index)currentindex);
          }

          //ImGui::TreePop();
        }

        //if (ImGui::TreeNode("Shader"))
        {
          bool has_changed = false;
          int currentindex = (int)light.m_Shader->m_Index;
          if (ImGui::BeginCombo("Shader Name", ShaderNames[currentindex]))
          {
            for (std::size_t j = 0; j < sizeof(ShaderNames) / sizeof(char *); ++j)
            {
              if (ImGui::Selectable(ShaderNames[j], currentindex == j))
              {
                currentindex = (int)j;
                has_changed = true;
              }
            }
            ImGui::EndCombo();
          }

          if (has_changed)
          {
            light.m_Shader = Engine::get().m_AssetManager.GetShader((ShaderIndex)currentindex);
          }
          //ImGui::TreePop();
        }

        ImGui::Text("Object Variables");
        {
          float tempfloat = light.ScaleVector().x;
          if (ImGui::DragFloat("Scale Vector", &tempfloat))
          {
            light.SetScaleVector({ tempfloat, tempfloat, tempfloat });
          }

          glm::vec3 tempVector = light.Centroid();

          if (ImGui::DragFloat3("Position Vector", &tempVector[0]))
          {
            light.SetCentroid(tempVector);
          }

          tempfloat = light.RotationAngle();
          if (ImGui::DragFloat("Rotation Angle", &tempfloat))
          {
            light.SetRotationAngle(tempfloat);
          }

          if (ImGui::DragFloat("Rotation Amount", &light.m_RotationAmount))
          {
          }

          tempVector = light.RotationVector();
          if (ImGui::DragFloat3("Rotation Vector", &tempVector[0]))
          {
            light.SetRotationVector(tempVector);
          }
        }

        ImGui::Text("Light Variables"); 
        {
          bool has_changed = false;
          int& currentindex = light.m_Light->type;
          if (ImGui::BeginCombo("Type", LightTypeNames[currentindex]))
          {
            for (std::size_t j = 0; j < sizeof(LightTypeNames) / sizeof(char *); ++j)
            {
              if (ImGui::Selectable(LightTypeNames[j], currentindex == j))
              {
                currentindex = (int)j;
                has_changed = true;
              }
            }
            ImGui::EndCombo();
          }

          if (has_changed)
          {
            light.m_Light->type = currentindex;
          }

          if (ImGui::ColorEdit3("Diffuse", &light.m_Light->diffuse.x)) {}
          if (ImGui::ColorEdit3("Specular", &light.m_Light->specular.x)) {}

          if (ImGui::ColorEdit3("Ambiant", &light.m_Light->ambiant.x))
          {
            light.m_Material.ambiant_color = light.m_Light->ambiant;
          }

          switch (light.m_Light->type)
          {
          case 0: // point
          {
            //ImGui::DragFloat3("Position", &light.m_Centroid.x);
            break;
          }
          case 1: // spotlight
          {
            //ImGui::DragFloat3("Position", &light.m_Centroid.x);
            //ImGui::DragFloat3("Rotation Vector", &light.m_RotationVector.x);

            float angledegress = light.RotationAngle();
            angledegress = glm::degrees(angledegress);
            if (ImGui::DragFloat("Rotation Angle", &angledegress))
            {
              // not sure using deg or rad internally
              angledegress = glm::radians(angledegress);
              light.SetRotationAngle(angledegress);
            }

            angledegress = glm::degrees(light.m_Light->InnerAngle);
            if (ImGui::DragFloat("Inner Angle", &angledegress))
            {
              light.m_Light->InnerAngle = std::min(glm::radians(20.0f), std::max(0.0f, glm::radians(angledegress)));
            }

            angledegress = glm::degrees(light.m_Light->OuterAngle);
            if (ImGui::DragFloat("Outer Angle", &angledegress))
            {
              light.m_Light->OuterAngle = std::max(glm::radians(20.0f), std::min(glm::radians(40.0f), glm::radians(angledegress)));
            }

            ImGui::DragFloat("Falloff", &light.m_Light->falloff);
            break;
          }
          case 2: //direction
          {
            glm::vec3 rotVec = light.RotationVector();
            if(ImGui::DragFloat3("Rotation Vector", &rotVec.x))
            {
              light.SetRotationVector(rotVec);
            }

            float angledegress = light.RotationAngle();
            angledegress = glm::degrees(angledegress);
            if (ImGui::DragFloat("Rotation Angle", &angledegress))
            {
              // not sure using deg or rad internally
              angledegress = glm::radians(angledegress);
              light.SetRotationAngle(angledegress);
            }
            break;
          }
          default:
          {
            break;
          }
          }
        }
        ImGui::TreePop();
      }
    }

    ImGui::Text("Global Light Variables");
    {
      LightData &LightData = Engine::get().m_RenderingManager.m_Lights;

      ImGui::DragFloat("Attenuation Constant", &LightData.constant);
      ImGui::DragFloat("Attenuation Linear", &LightData.linear);
      ImGui::DragFloat("Attenuation Quadratic", &LightData.quadratic);

      ImGui::ColorEdit3("Global Ambient", &LightData.GlobalAmbient.x);
      ImGui::ColorEdit3("Global Atmospheric", &LightData.Atmospheric.x);

      if (ImGui::DragFloat("Z Far", &LightData.ZFar))
      {
        LightData.ZFar = std::max(LightData.ZFar, LightData.ZNear);
      }

      if (ImGui::DragFloat("Z Near", &LightData.ZNear))
      {
        LightData.ZNear = std::max(0.0001f, std::min(LightData.ZFar, LightData.ZNear));
      }

      if (ImGui::ColorEdit4("Fog", &LightData.IFog.x))
      {
      }
    }
    

    ImGui::TreePop();
  }

  if (ImGui::ColorEdit3("Debug Normal Face Color", &Engine::get().m_RenderingManager.m_NormalColorFace[0])){}
  if (ImGui::ColorEdit3("Debug Normal Vertex Color", &Engine::get().m_RenderingManager.m_NormalColorVertex[0])) { }
  if (ImGui::DragFloat("Debug Normal Length", &Engine::get().m_RenderingManager.m_NormalLength)) { }

  if (ImGui::TreeNode("Objects"))
  {
    // loop through all objects
    for (int i = 0; i < Engine::get().m_SceneManager.m_CurrentScene->m_Objects.size(); i++)
    {
      Object& object = Engine::get().m_SceneManager.m_CurrentScene->m_Objects[i];

      if (ImGui::TreeNode(&object, "Object: [%i]", i))
      {
        if (ImGui::Checkbox("Draw Vertex Normals", &object.m_DrawVertexNormals))
        {
        }
        if (ImGui::Checkbox("Draw Face Normals", &object.m_DrawFaceNormals))
        {
        }

        //if (ImGui::TreeNode("Model"))
        {
          bool has_changed = false;
          int currentindex = (int)object.m_Model->m_ModelIndex;
          if (ImGui::BeginCombo("Model Name", Model::IndexNames[currentindex].c_str()))
          {
            for (std::size_t j = 0; j < Model::IndexNames.size(); ++j)
            {
              if (ImGui::Selectable(Model::IndexNames[j].c_str(), currentindex == j))
              {
                currentindex = (int)j;
                has_changed = true;
              }
            }
            ImGui::EndCombo();
          }

          if (has_changed)
          {
            object.m_Model = Engine::get().m_AssetManager.GetModel((Model::Index)currentindex);
          }

          //ImGui::TreePop();
        }

        //if (ImGui::TreeNode("Shader"))
        {
          bool has_changed = false;
          int currentindex = (int)object.m_Shader->m_Index;
          if (ImGui::BeginCombo("Shader Name", ShaderNames[currentindex]))
          {
            for (std::size_t j = 0; j < sizeof(ShaderNames) / sizeof(char *); ++j)
            {
              if (ImGui::Selectable(ShaderNames[j], currentindex == j))
              {
                currentindex = (int)j;
                has_changed = true;
              }
            }
            ImGui::EndCombo();
          }

          if (has_changed)
          {
            object.m_Shader = Engine::get().m_AssetManager.GetShader((ShaderIndex)currentindex);
          }
        }

        ImGui::Text("Object Variables");
        {
          glm::vec3 tempVector;
          float tempfloat;

          tempfloat = object.ScaleVector().x;
          if (ImGui::DragFloat("Scale Vector", &tempfloat))
          {
            object.SetScaleVector({ tempfloat, tempfloat, tempfloat });
          }

          tempVector = object.Centroid();
          if (ImGui::DragFloat3("Position Vector", &tempVector[0]))
          {
            object.SetCentroid(tempVector);
          }

          tempfloat = object.RotationAngle();
          if (ImGui::DragFloat("Rotation Angle", &tempfloat))
          {
            object.SetRotationAngle(tempfloat);
          }

          if (ImGui::DragFloat("Rotation Amount", &object.m_RotationAmount))
          {
          }

          tempVector = object.RotationVector();
          if (ImGui::DragFloat3("Rotation Vector", &tempVector[0]))
          {
            object.SetRotationVector(tempVector);
          }
        }

        ImGui::Text("Material Variables");
        {
          if (ImGui::DragFloat("Specular Exponent", &object.m_Material.specular_exponent))
          {
          }

          if (ImGui::ColorEdit3("Ambiant Color", &object.m_Material.ambiant_color[0]))
          {
          }

          if (ImGui::ColorEdit3("diffuse Color", &object.m_Material.diffuse_color[0]))
          {
          }

          if (ImGui::ColorEdit3("specular Color", &object.m_Material.specular_color[0]))
          {
          }

          if (ImGui::DragFloat("specular exponent", &object.m_Material.specular_exponent))
          {
          }

          if (ImGui::ColorEdit3("emissive Color", &object.m_Material.emissive_color[0]))
          {
          }
        }

        ImGui::Text("Debug Variables");
        {
          if (ImGui::Checkbox("Draw AABB Bounds", &object.m_DrawAABB))
          {
          }
          /*
          if (ImGui::Checkbox("Draw Bounding Sphere", &object.m_DrawBoundingSphere))
          {
          }
          */
        }
        
        ImGui::TreePop();
      }
    }


    ImGui::TreePop();
  }

  ImGui::End();
  ImGui::Render();
  ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}
