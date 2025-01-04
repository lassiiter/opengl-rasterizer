#version 430 core

out vec4 FragColor;
in vec3 WorldPos;
in vec3 Normal;
in vec2 TexCoords;

// Material parameters
uniform vec3 u_tint;
uniform float u_metallic;
uniform float u_roughness;
uniform float u_enable_IBL_specular;
uniform float u_enable_IBL_diffuse;

layout(binding  = 3) uniform sampler2D albedoTex;
layout(binding  = 4) uniform sampler2D ormTex;
layout(binding  = 5) uniform sampler2D emissiveTex;

layout(binding  = 7) uniform sampler2D irradianceTex;
layout(binding  = 8) uniform sampler2D radianceTex;

// Light parameters
uniform vec3 lightPosition;
uniform vec3 lightColor;

// Camera parameters
uniform vec3 camPos;

const float PI = 3.14159265359;
const float MAX_REFLECTION_LOD = 4.0;
// ----------------------------------------------------------------------------
// http://holger.dammertz.org/stuff/notes_HammersleyOnHemisphere.html
// efficient VanDerCorpus calculation.
float RadicalInverse_VdC(uint bits) 
{
     bits = (bits << 16u) | (bits >> 16u);
     bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
     bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
     bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
     bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);
     return float(bits) * 2.3283064365386963e-10; // / 0x100000000
}
// ----------------------------------------------------------------------------
vec2 Hammersley(uint i, uint N)
{
	return vec2(float(i)/float(N), RadicalInverse_VdC(i));
}
// ----------------------------------------------------------------------------
vec3 ImportanceSampleGGX(vec2 Xi, vec3 N, float roughness)
{
	float a = roughness*roughness;
	
	float phi = 2.0 * PI * Xi.x;
	float cosTheta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sinTheta = sqrt(1.0 - cosTheta*cosTheta);
	
	// from spherical coordinates to cartesian coordinates - halfway vector
	vec3 H;
	H.x = cos(phi) * sinTheta;
	H.y = sin(phi) * sinTheta;
	H.z = cosTheta;
	
	// from tangent-space H vector to world-space sample vector
	vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
	vec3 sampleVec = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sampleVec);
}
// ----------------------------------------------------------------------------
// Geometry function (Schlick-GGX)
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r * r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
// Geometry function (Smith method)
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
// Splitsum BRDF
vec2 IntegrateBRDF(float NdotV, float roughness)
{
    vec3 V;
    V.x = sqrt(1.0 - NdotV*NdotV);
    V.y = 0.0;
    V.z = NdotV;

    float A = 0.0;
    float B = 0.0;

    vec3 N = vec3(0.0, 0.0, 1.0);

    const uint SAMPLE_COUNT = 1024u;
    for(uint i = 0u; i < SAMPLE_COUNT; ++i)
    {
        vec2 Xi = Hammersley(i, SAMPLE_COUNT);
        vec3 H  = ImportanceSampleGGX(Xi, N, roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if(NdotL > 0.0)
        {
            float G = GeometrySmith(N, V, L, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);
    return vec2(A, B);
}
// ----------------------------------------------------------------------------
// Cook-Torrance microfacet distribution (GGX)
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
// Fresnel-Schlick approximation
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
// ----------------------------------------------------------------------------
// Fresnel-Schlick approximation with roughness support
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
} 
// ----------------------------------------------------------------------------
void main()
{
    vec3 N = normalize(Normal);
    vec3 V = normalize(camPos - WorldPos);

    // Convert to spherical coordinates for irradiance and radiance mapping
    vec3 R = reflect(V, N);
    float phi = atan(R.z, R.x) / (2.0 * 3.14159265359) + 0.5; // Longitude
    float theta = acos(R.y) / 3.14159265359; // Latitude

    // Sampling textures
    vec3 irradiance = texture(irradianceTex, vec2(phi, theta)).rgb;
    vec3 albedo = pow(texture(albedoTex, TexCoords).rgb, vec3(2.2)); // Gamma correction
    vec3 orm    = texture(ormTex, TexCoords).rgb;
    vec3 emissive = texture(emissiveTex, TexCoords).rgb;

    float ao = orm.r; // Ambient occlusion
    float roughness = orm.g; // Surface roughness
    float metallic = orm.b; // Metalness

    // Apply uniform tint and material parameters
    albedo *= u_tint; 
    roughness = clamp(roughness * u_roughness, 0.0, 1.0);
    metallic = clamp(metallic * u_metallic, 0.0, 0.9);

    // Calculate the reflectance at normal incidence (F0)
    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic); // For metals, F0 = albedo, for dielectrics F0 = 0.04

    // Initialize kS and kD based on Fresnel-Schlick
    vec3 kS = fresnelSchlick(max(dot(N, V), 0.0), F0);
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    // Cook-Torrance BRDF calculations
    vec3 Lo = vec3(0.0);
    for (int i = 0; i < 1; ++i) // Iterate over lights (this can be expanded)
    {
        // Calculate per-light radiance
        vec3 L = normalize(lightPosition - WorldPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPosition - WorldPos);
        float attenuation = 1.0 / (distance * distance); // Simple attenuation
        vec3 radiance = lightColor * attenuation;

        // Cook-Torrance BRDF components
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 nominator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0);
        vec3 specular_Lo = nominator / max(denominator, 0.001); // Prevent division by zero

        // Diffuse lighting component (scaled by kD)
        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular_Lo) * radiance * NdotL;
    }

    // Fresnel with roughness consideration
    vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);

    // Ambient lighting contributions
    irradiance = mix(vec3(1.0), irradiance, u_enable_IBL_diffuse); // Mix in irradiance based on IBL diffuse flag

    // Diffuse and ambient lighting based on irradiance and AO
    vec3 prefilteredColor = textureLod(radianceTex, vec2(phi, theta), roughness * MAX_REFLECTION_LOD).rgb;
    vec2 brdf = IntegrateBRDF(max(dot(N, V), 0.0), roughness);
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);
    specular *= u_enable_IBL_specular; // Turn off specular if required


    vec3 diffuse = irradiance * albedo;
    vec3 ambient = (kD * diffuse + specular) * ao;
    
    // Final color: ambient + direct lighting (Lo)
    vec3 color = ambient + Lo;

    // Tonemapping (for HDR rendering)
    color = color / (color + vec3(1.0));

    // Add emissive lighting (scaled for better visibility)
    color += (emissive * 3.0);

    // Gamma correction (2.2)
    color = pow(color, vec3(1.0 / 2.2));

    FragColor = vec4(color, 1.0);
}