/* Start Header -------------------------------------------------------
Copyright (C) 2019 DigiPen Institute of Technology.
File Name: LightingPass.frag
Purpose: second pass of defered rendering for drawing the full screen quad
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
out vec4 FragColor;

in vec2 TexCoords;


uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gDiffuse;
uniform sampler2D gAmbient;
uniform sampler2D gEmissive;
uniform sampler2D gSpecular;

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

uniform vec4 EyePosition;

void main()
{             
  // retrieve data from gbuffer
  vec3 FragPos = texture(gPosition, TexCoords).rgb;
  if(length(FragPos) > 1000)
	{
		discard;
	}


  vec3 Normal = texture(gNormal, TexCoords).rgb;

	const float PI = 3.14159f;

	float SpecularExponent = texture(gSpecular, TexCoords).a;

	vec3 DiffuseColor = texture(gDiffuse, TexCoords).rgb;
  vec3 SpecularColor = texture(gSpecular, TexCoords).rgb;
	vec3 Emissive = texture(gEmissive, TexCoords).rgb;
	vec3 AmbiantColor = texture(gAmbient, TexCoords).rgb;
	vec3 LightGlobalAmbiant = GlobalAmbient * AmbiantColor;
	vec3 LightLocal = LightGlobalAmbiant.xyz;

    
    
	float debug0 = 0.0;
	float debug1 = 0.0;
	float debug3 = 0.0;

	for(int i = 0; i <  numberOfLights; ++i)
	{
		Light light = lights[i];
		vec3 L = normalize(light.position - FragPos.xyz);
		vec3 m = normalize(Normal.xyz);
		vec3 v = normalize(FragPos.xyz - EyePosition.xyz);
		float dl = distance(light.position.xyz, FragPos.xyz);
		if(light.type == 2) //direction
		{
			L = -light.direction;
			dl = 0.0;
		}
		float N_dot_L = max(0.0, dot(m,L));
		vec3 Rl = 2.0 * N_dot_L * m - L;
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

		float spec_angle = max(0.0, dot(Rl,v)); //* smoothstep(0.0, 0.1, N_dot_L);
		//spec_angle = distance(FragPos.xyz, EyePosition.xyz);
		
		vec3 LocalAmbient = AmbiantColor * light.ambiant;
		vec3 LocalDiffuse = DiffuseColor * N_dot_L * light.diffuse;
		vec3 LocalSpecular = max(pow(spec_angle, SpecularExponent) * light.specular.xyz * SpecularColor.xyz, vec3(0.0));

		vec3 TotalAmbient = Attenuation * LocalAmbient;
		vec3 TotalDiffuseSpecular = Attenuation * SpotLightEffect * (LocalDiffuse.xyz + LocalSpecular.xyz);

		LightLocal += (TotalAmbient + TotalDiffuseSpecular) * 0.000001 + spec_angle;
	}

	float S = max(0.0, min(1.0, (ZFar - distance(FragPos.xyz, EyePosition.xyz)) / (ZFar - ZNear)));

	FragColor = vec4(S * LightLocal + (1 - S) * IFog.xyz, 1.0);
	FragColor = vec4(LightLocal, 1.0);
}