#version 430 core

out vec4 FragColor;
in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

// material parameters
// uniform vec3 albedo;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_ao;

layout(binding  = 3) uniform sampler2D albedoTex;
layout(binding  = 4) uniform sampler2D ormTex;
layout(binding  = 5) uniform sampler2D emissiveTex;

layout(binding  = 7) uniform sampler2D irradianceTex;
layout(binding  = 8) uniform sampler2D radianceTex;

//uniform sampler2D normal;
// mostly taken from https://learnopengl.com/code_viewer_gh.php?code=src/6.pbr/2.2.2.ibl_specular_textured/2.2.2.pbr.fs
// lights
uniform vec3 lightPosition;
uniform vec3 lightColor;

uniform vec3 camPos;

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.0;
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
  float a = roughness * roughness;
  float a2 = a * a;
  float NdotH = max(dot(N, H), 0.0);
  float NdotH2 = NdotH * NdotH;

  float nom = a2;
  float denom = (NdotH2 * (a2 - 1.0) + 1.0);
  denom = PI * denom * denom;

  return nom / max(denom, 0.0000001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
  float r = (roughness + 1.0);
  float k = (r * r) / 8.0;

  float nom = NdotV;
  float denom = NdotV * (1.0 - k) + k;

  return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
  float NdotV = max(dot(N, V), 0.0);
  float NdotL = max(dot(N, L), 0.0);
  float ggx2 = GeometrySchlickGGX(NdotV, roughness);
  float ggx1 = GeometrySchlickGGX(NdotL, roughness);

  return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
  return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 
// ----------------------------------------------------------------------------
void main()
{
  vec3 N = normalize(Normal);
  vec3 V = normalize(camPos - WorldPos);

  // Convert to spherical coordinates too lazy to create cubemaps
  vec3 R = reflect(V, N);
  float phi = atan(R.z, R.x) / (2.0 * 3.14159265359) + 0.5; // Longitude
  float theta = acos(R.y) / 3.14159265359; // Latitude

  vec3 irradiance = texture(irradianceTex, vec2(phi, theta)).rgb;
  vec3 albedo = pow(texture(albedoTex, TexCoords).rgb, vec3(2.2));
  vec3 orm    = (texture(ormTex, TexCoords)).rgb;
  vec3 emissive    = (texture(emissiveTex, TexCoords)).rgb;

  float ao = orm.r;
  float roughness = orm.b;
  float metallic = orm.g;
  vec3 specular = textureLod(radianceTex, vec2(phi, theta),roughness * MAX_REFLECTION_LOD).rgb;

  // calculate reflectance at normal incidence; if dia-electric (like plastic) use F0 
  // of 0.04 and if it's a metal, use the albedo color as F0 (metallic workflow)    
  vec3 F0 = vec3(0.04);
  F0 = mix(F0, albedo, metallic);

  // reflectance equation
  vec3 Lo = vec3(0.0);
  for(int i = 0; i < 1; ++i) 
  {
      // calculate per-light radiance
      vec3 L = normalize(lightPosition - WorldPos);
      vec3 H = normalize(V + L);
      float distance = length(lightPosition - WorldPos);
      float attenuation = 1.0 / (distance * distance);
      vec3 radiance = lightColor * attenuation;

      // Cook-Torrance BRDF
      float NDF = DistributionGGX(N, H, roughness);
      float G = GeometrySmith(N, V, L, roughness);
      vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);  

      vec3 nominator = NDF * G * F;
      float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
      vec3 specular_Lo = nominator / max(denominator, 0.001); // prevent divide by zero for NdotV=0.0 or NdotL=0.0
     
      // kS is equal to Fresnel
      vec3 kS = F;
      // for energy conservation, the diffuse and specular light can't
      // be above 1.0 (unless the surface emits light); to preserve this
      // relationship the diffuse component (kD) should equal 1.0 - kS.
      vec3 kD = vec3(1.0) - kS;
      // multiply kD by the inverse metalness such that only non-metals 
      // have diffuse lighting, or a linear blend if partly metal (pure metals
      // have no diffuse light).
      kD *= 1.0 - metallic;

      // scale light by NdotL
      float NdotL = max(dot(N, L), 0.0);

      // add to outgoing radiance Lo
      Lo += (kD * albedo / PI + specular_Lo) * radiance * NdotL;  // note that we already multiplied the BRDF by the Fresnel (kS) so we won't multiply by kS again
  }
   vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

   vec3 kS = F;
   vec3 kD = vec3(1.0) - kS;
   kD *= 1.0 - metallic;

  // this ambient lighting with environment lighting).
  vec3 diffuse      = irradiance * albedo;
  vec3 ambient = (kD * diffuse + (specular*roughness)) * ao;

  vec3 color = ambient + Lo + (emissive*20.0);

  // HDR tonemapping
  color = color / (color + vec3(1.0));

  // gamma correct
  color = pow(color, vec3(1.0 / 2.2));

  FragColor = vec4(color, 1.0);
}
