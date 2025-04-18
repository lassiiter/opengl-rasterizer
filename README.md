# OpenGL Real-time Renderer
This is a hobby project I did to learn and implement a C++/OpenGL renderer. It supports basic artist authored PBR inputs following a metallic/roughness workflow.  

![full3](https://github.com/user-attachments/assets/26917a94-be94-4bc1-9808-cbda11512f44)

## Features
- Cook-Torrance microfacet specular BRDF
- Lambertian diffuse BRDF
- HDR/linear lighting + Tonemapping
- Metallic workflow
- Normal mapping & ambient occlusion mapping
- Image-based lighting and analytical lighting
- Model support (.fbx/.gltf)
- PBR support using Color, Roughness, Metal, and Normals
- Realtime model, texture, and material editing

### Libraries
- Glew, http://glew.sourceforge.net/  
- GLFW, https://www.glfw.org/  
- GLM, https://glm.g-truc.net/0.9.9/index.html  
- Assimp, https://github.com/assimp/assimp  
- ImGUI, https://github.com/ocornut/imgui  

### References
- Implementation - https://learnopengl.com/
- Theory - https://google.github.io/filament/
- BRDF - https://boksajak.github.io/blog/BRDF
- UI/Project Structure - https://github.com/jayanam/jgl_demos
- Models - https://github.com/KhronosGroup/glTF-Sample-Models
