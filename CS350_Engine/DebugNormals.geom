/* Start Header -------------------------------------------------------
Copyright (C) 2021 DigiPen Institute of Technology.
File Name: DebugNormal.geom
Purpose: 
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
Creation date: 1/30/2021
End Header --------------------------------------------------------*/

#version 430 core

layout(triangles) in;

layout(line_strip, max_vertices=8) out;

uniform int DrawMode; // 0 vert, 1 face, 2 both
uniform mat4 MVPMatrix;
uniform mat4 NormalMatrix;
uniform vec4 ColorVertex;
uniform vec4 ColorFace;
uniform float length;

in Vertex
{
  vec4 normal;
} vertex[];

out vec4 Color;

void main()
{
  if(DrawMode == 0 || DrawMode == 2)
  {
    for (int i = 0; i < gl_in.length(); ++i)
    {
      vec3 P = gl_in[i].gl_Position.xyz;
      vec3 N = vertex[i].normal.xyz;
    
      gl_Position = MVPMatrix * vec4(P, 1.0);
      Color = ColorVertex;
      EmitVertex();
    
      gl_Position = MVPMatrix * vec4(P + N * length, 1.0);
      Color = ColorVertex;
      EmitVertex();
    
      EndPrimitive();
    }
  }
  if(DrawMode == 1 || DrawMode == 2)
  {
    vec3 P0 = gl_in[0].gl_Position.xyz;
    vec3 P1 = gl_in[1].gl_Position.xyz;
    vec3 P2 = gl_in[2].gl_Position.xyz;
  
    vec3 V0 = P0 - P1;
    vec3 V1 = P2 - P1;
  
    vec3 N = cross(V1, V0);
    N = normalize(N);
  
    // Center of the triangle
    vec3 P = (P0+P1+P2) / 3.0;
  
    gl_Position = MVPMatrix * vec4(P, 1.0);
    Color = ColorFace;
    EmitVertex();
  
    gl_Position = MVPMatrix * vec4(P + N * length, 1.0);
    Color = ColorFace;
    EmitVertex();
    EndPrimitive();
  }
}