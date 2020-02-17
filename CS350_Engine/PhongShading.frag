/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior written
consent of DigiPen Institute of Technology is prohibited.
File Name: PhongShading.frag
Purpose: forward phong shading
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

#version 430 core

uniform vec4  EyePosition;
uniform vec3  AmbiantColor;

struct Light
{
  vec3 position;
  float InnerAngle;

  vec3 direction;
  float OuterAngle;

  vec3 diffuse;
  float falloff;

  vec3 specular;
  int type;

  vec3 ambiant;
  int pad;
};

layout (std430, binding = 0) buffer LightData
{
	Light lights[16];
	
	int numberOfLights;
	float constant;
	float linear;
	float quadratic;

	vec3 GlobalAmbient;
	float ZFar;

	vec3 Atmospheric;
	float ZNear;

	vec4 IFog;
};

in vec4 world_normal;
in vec4 world_position;
in vec3 model_position;
in vec3 ModelNormal;
out vec4 FragColor;

void main()
{
	float PI = 3.14159f;

	float SpecularExponent = 1.0;

	vec3 DiffuseColor = vec3(1.0);
    vec3 SpecularColor = vec3(1.0);
	vec3 Emissive = vec3(0);
	vec3 LightGlobalAmbiant = GlobalAmbient * AmbiantColor;
	vec3 LightLocal = LightGlobalAmbiant.xyz;

	float debug0 = 0.0;
	float debug1 = 0.0;
	float debug3 = 0.0;

	for(int i = 0; i <  numberOfLights; ++i)
	{
		Light light = lights[i];
		vec3 L = normalize(light.position - world_position.xyz);
		vec3 m = normalize(world_normal.xyz);
		vec3 v = normalize(world_position.xyz - EyePosition.xyz);
		float dl = distance(light.position.xyz, world_position.xyz);
		if(light.type == 2) //direction
		{
			L = -light.direction;
			dl = 0.0;
		}
		float N_dot_L = max(0.0, dot(m,L));
		vec3 Rl = L - 2.0 * N_dot_L * m;
		debug0 = dl;
		float Attenuation = min(1.0, 1.0 / (constant + linear * dl + quadratic * dl * dl));
		
		float SpotLightEffect = 1;

		if(light.type == 1)
		{
			float Alpha = max(0.0, dot(L, -light.direction));
			float Phi = light.OuterAngle;
			float Theta = light.InnerAngle;
		
			SpotLightEffect = max(0.0, pow((Alpha - cos(Phi))/(cos(Theta) - cos(Phi)), light.falloff));
		}

		float spec_angle = max(0.0, dot(Rl,v)) * smoothstep(0.0, 0.1, N_dot_L);

		vec3 LocalAmbient = AmbiantColor * light.ambiant;
		vec3 LocalDiffuse = DiffuseColor * N_dot_L * light.diffuse;
		vec3 LocalSpecular = max(pow(spec_angle, SpecularExponent) * light.specular.xyz * SpecularColor.xyz, vec3(0.0));

		vec3 TotalAmbient = Attenuation * LocalAmbient;
		vec3 TotalDiffuseSpecular = Attenuation * SpotLightEffect * (LocalDiffuse.xyz + LocalSpecular.xyz);

		LightLocal += TotalAmbient + TotalDiffuseSpecular;
	}

	float S = max(0.0, min(1.0, (ZFar - distance(world_position.xyz, EyePosition.xyz)) / (ZFar - ZNear)));

	FragColor = vec4(S * LightLocal + (1 - S) * IFog.xyz, 1.0);
}
