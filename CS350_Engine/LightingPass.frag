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
uniform sampler2D gAlbedoSpec;

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
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 Diffuse = texture(gAlbedoSpec, TexCoords).rgb;
    float Specular = texture(gAlbedoSpec, TexCoords).a;
    
    // then calculate lighting as usual
    vec3 lighting  = Diffuse * 0.1; // hard-coded ambient component
    vec3 viewDir  = normalize(EyePosition.xyz - FragPos);
    for(int i = 0; i < numberOfLights; ++i)
    {
        // calculate distance between light source and current fragment
        float dl = length(lights[i].position - FragPos);
		float maxBrightness = max(max(lights[i].ambiant.r, lights[i].ambiant.g), lights[i].ambiant.b);
		float radius = (-linear + sqrt(linear * linear - 4 * quadratic * (constant - (256.0f / 5.0f) * maxBrightness))) / (2.0f * quadratic);
        if(dl < radius)
        {
            // diffuse
            vec3 lightDir = normalize(lights[i].position - FragPos);
            vec3 diffuse = max(dot(Normal, lightDir), 0.0) * Diffuse * lights[i].diffuse;
            // specular
            vec3 halfwayDir = normalize(lightDir + viewDir);  
            float spec = pow(max(dot(Normal, halfwayDir), 0.0), 16.0);
            vec3 specular = lights[i].specular * spec * Specular;
            // attenuation
            float attenuation = 1.0 / (1.0 + linear * dl + quadratic * dl * dl);
            diffuse *= attenuation;
            specular *= attenuation;
            lighting += diffuse + specular;
        }
    }    
    FragColor = vec4(lighting, 1.0);
}