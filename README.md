### A C++ OpenGL renderer. 
Supports modern roughness/metallic PBR workflows & ubiquitous model formats  
  
  
  

![full3](https://github.com/user-attachments/assets/26917a94-be94-4bc1-9808-cbda11512f44)

## Rendering Features
This renderer implements a physically-based rendering (PBR) pipeline, attempting to adhere to the principles of energy conservation and photorealistic material behavior.
#### Physically Based Shading
- Cook-Torrance Microfacet Specular BRDF: A sophisticated specular model utilizing:
  - NDF (D): Trowbridge-Reitz (GGX) for realistic specular distribution and "long tails."
  - Geometry (G): Smith’s method with Schlick-GGX approximation for accurate shadowing and masking.
  - Fresnel (F): Fresnel-Schlick approximation for view-dependent reflectivity.
- Lambertian Diffuse: Energy-conserving diffuse reflectance coupled with the specular microfacet framework.
- Standardized Workflow: Full support for the modern Roughness/Metallic workflow seen in games.

#### Lighting & Post-Processing
- Image-Based Lighting (IBL): Support for HDR environment maps.
  - Diffuse: Pre-computed irradiance maps.
  - Specular: Pre-convolved radiance maps utilizing the Epic Games Split-Sum approximation and a 2D BRDF Lookup Texture (LUT).
  - Analytical Lighting: Support for Point and Directional lights with physical $1/r^2$ attenuation.
- HDR Pipeline: High-dynamic range rendering with linear color space processing.
- Tonemapping: Implementation of ACES and Reinhard operators for mapping HDR values to LDR displays.

#### Surface Mapping & Assets
- Texture Mapping:
  - Comprehensive PBR Maps: Albedo (BaseColor), Metallic, Roughness.
  - Normal Mapping: Tangent-space perturbation for high-frequency surface details.
  - Ambient Occlusion (AO): Integration of occlusion maps to modulate indirect ambient lighting.
- Model Ingestion: Robust scene loading via Assimp, supporting .gltf, and .fbxs.
- Live Material System: Real-time manipulation of material properties, textures, and transformations via an integrated UI.

## Technical References
#### The implementation of this renderer is based on the following seminal research:

- GGX Distribution: Walter et al. (2007), “Microfacet Models for Refraction through Rough Surfaces”

- Cook-Torrance: Cook & Torrance (1982), “A Reflectance Model for Computer Graphics”

- Fresnel-Schlick: Schlick (1994), “An Inexpensive BRDF Model for Physically-Based Rendering”

- Geometry Smith: Smith (1967), “Geometrical Shadowing of a Random Rough Surface”

- Split-Sum Approximation: Karis (2013), “Real Shading in Unreal Engine 4”





### Libraries
- Glew, http://glew.sourceforge.net/  
- GLFW, https://www.glfw.org/  
- GLM, https://glm.g-truc.net/0.9.9/index.html  
- Assimp, https://github.com/assimp/assimp  
- ImGUI, https://github.com/ocornut/imgui  
